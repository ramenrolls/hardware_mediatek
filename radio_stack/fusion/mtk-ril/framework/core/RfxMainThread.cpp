/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxAsyncSignal.h"
#include "RfxBasics.h"
#include "RfxControllerFactory.h"
#include "RfxLog.h"
#include "RfxMainThread.h"
#include "RfxRootController.h"
#include "RfxTestSuitController.h"
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include "utils/Timers.h"
#include "utils/Mutex.h"

using ::android::Mutex;

#define RFX_LOG_TAG "RfxMainThread"

/*****************************************************************************
 * Main thread watch dog
 *****************************************************************************/
#define RFX_MAIN_THREAD_WATCHDOG_TIME (10 * 60 * 1000)

static timer_t sWatchDogTimer;
static struct timespec sStartReal;
static struct timespec sStartMono;

static void _watch_dog(sigval_t sig) {
    RFX_UNUSED(sig);

    struct timespec endReal;
    struct timespec endMono;

    RFX_LOG_I("RFX_WATCHDOG", "timeout start real tv_s:%ld, tv_ns:%ld", sStartReal.tv_sec,
              sStartReal.tv_nsec);

    RFX_LOG_I("RFX_WATCHDOG", "timeout start mono tv_s:%ld, tv_ns:%ld", sStartMono.tv_sec,
              sStartMono.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &endReal);
    clock_gettime(CLOCK_MONOTONIC, &endMono);

    RFX_LOG_I("RFX_WATCHDOG", "timeout end real tv_s:%ld, tv_ns:%ld", endReal.tv_sec,
              endReal.tv_nsec);

    RFX_LOG_I("RFX_WATCHDOG", "timeout end mono tv_s:%ld, tv_ns:%ld", endMono.tv_sec,
              endMono.tv_nsec);

    // asserts here means one message cost
    // over 10s or main thread is blocked
    RFX_ASSERT(0);
}

static void _init_watch_dog(void) {
    struct sigevent sevp;
    memset(&sevp, 0, sizeof(sevp));
    sevp.sigev_value.sival_int = 0;
    sevp.sigev_notify = SIGEV_THREAD;
    sevp.sigev_notify_function = _watch_dog;

    if (timer_create(CLOCK_MONOTONIC, &sevp, &sWatchDogTimer) == -1) {
        RFX_LOG_E("RFX_WATCHDOG", "timer_create  failed reason=[%s]", strerror(errno));
        RFX_ASSERT(0);
    }
}

static void _start_watch_dog(int milliseconds) {
    struct itimerspec expire;
    expire.it_interval.tv_sec = 0;
    expire.it_interval.tv_nsec = 0;
    expire.it_value.tv_sec = milliseconds / 1000;
    expire.it_value.tv_nsec = (milliseconds % 1000) * 1000000;
    int ret = timer_settime(sWatchDogTimer, 0, &expire, NULL);
    if (ret != 0) {
        RFX_LOG_E("RFX_WATCHDOG", "Couldn't set timer: %s", strerror(errno));
    }
    clock_gettime(CLOCK_REALTIME, &sStartReal);
    clock_gettime(CLOCK_MONOTONIC, &sStartMono);
}

static void _stop_watch_dog() { _start_watch_dog(0); }

/*****************************************************************************
 * Class RfxMainHandler
 *****************************************************************************/
static nsecs_t sMsgProcessTime = 0;

void RfxMainHandler::handleMessage(const Message& message) {
    RFX_UNUSED(message);
    nsecs_t start_time = systemTime(SYSTEM_TIME_MONOTONIC);

    // watchdog for main handler
    _start_watch_dog(RFX_MAIN_THREAD_WATCHDOG_TIME);

    // Call to sub-class
    onHandleMessage(message);

    // process async events
    // RFX_LOG_D(RFX_LOG_TAG, "process async queue begin, mainHandler = %p", this);

    RfxAsyncSignalQueue* async_queue = RFX_OBJ_GET_INSTANCE(RfxAsyncSignalQueue);
    RfxRootController* root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    do {
        // Process Post Event
        async_queue->processEmit();

        // enqueue the suspended messages if needed
        root->processSuspendedMessage();
    } while (!async_queue->isEmpty());

    // RFX_LOG_D(RFX_LOG_TAG, "process async queue end, mainHandler = %p", this);

    // Just For test
    RfxTestSuitController* testSuitController = RFX_OBJ_GET_INSTANCE(RfxTestSuitController);
    if (testSuitController->isEnableTest()) {
        testSuitController->checkSuccessAndEnqueueNext();
    }
    // RFX_LOG_D(RFX_LOG_TAG, "handle msg end, mainHandler = %p", this);

#ifdef RFX_OBJ_DEBUG
    RfxDebugInfo::dumpIfNeed();
#endif

    _stop_watch_dog();

    nsecs_t time = systemTime(SYSTEM_TIME_MONOTONIC) - start_time;

    if (time > sMsgProcessTime) {
        sMsgProcessTime = time;
        RFX_LOG_D(RFX_LOG_TAG, "Process time %ldms", (long)ns2ms(time));
    }
}

/*****************************************************************************
 * Class RfxMessageHandler
 *****************************************************************************/
static Mutex sMsgIgnoreMutex;

class RfxMessageHandler : public RfxMainHandler {
  public:
    explicit RfxMessageHandler(const sp<RfxMessage>& msg) : m_msg(msg) {}
    virtual ~RfxMessageHandler() {}

    static void setIgnoreTimeStamp(nsecs_t time) {
        sMsgIgnoreMutex.lock();
        if (s_ignore_time_stamp != time) {
            s_ignore_time_stamp = time;
            s_new_ignore = true;
        }
        sMsgIgnoreMutex.unlock();
    }

  protected:
    /**
     * Handles a message.
     */
    virtual void onHandleMessage(const Message& message) {
        sMsgIgnoreMutex.lock();
        if (s_new_ignore) {
            RFX_OBJ_GET_INSTANCE(RfxRootController)->clearMessages();
            s_new_ignore = false;
        }
        if (s_ignore_time_stamp != -1 && m_msg->getTimeStamp() < s_ignore_time_stamp) {
            RFX_LOG_D(RFX_LOG_TAG, "Ignore message [%s]", m_msg->toString().string());
            // reply RIL_E_RADIO_NOT_AVAILABLE
            if (REQUEST == m_msg->getType()) {
                sp<RfxMessage> response =
                        RfxMessage::obtainResponse(RIL_E_RADIO_NOT_AVAILABLE, m_msg);
                RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->responseToRilj(response);
            }
            sMsgIgnoreMutex.unlock();
            return;
        }
        sMsgIgnoreMutex.unlock();
        RFX_UNUSED(message);
        // dispatch msg to root controller, it will
        // do further dispatch
        RfxRootController* root = RFX_OBJ_GET_INSTANCE(RfxRootController);
        root->processMessage(m_msg);
    }

  private:
    sp<RfxMessage> m_msg;
    static nsecs_t s_ignore_time_stamp;
    static bool s_new_ignore;
};

nsecs_t RfxMessageHandler::s_ignore_time_stamp = -1;
bool RfxMessageHandler::s_new_ignore = false;

/*****************************************************************************
 * Class RfxMainThread
 *****************************************************************************/

static sem_t sWaitLooperSem;
static bool sNeedWaitLooper = true;
static Mutex sWaitLooperMutex;

RfxMainThread* RfxMainThread::s_self = NULL;

RfxMainThread::RfxMainThread() : m_looper(NULL) {}

RfxMainThread::~RfxMainThread() {}

void RfxMainThread::init() {
    RFX_LOG_D(RFX_LOG_TAG, "init begin");
    sem_init(&sWaitLooperSem, 0, 0);
    _init_watch_dog();
    s_self = new RfxMainThread();
    s_self->run("Ril Proxy Main Thread");
    RFX_LOG_D(RFX_LOG_TAG, "init end");
}

sp<Looper> RfxMainThread::waitLooper() {
    RFX_ASSERT(s_self != NULL);
    sWaitLooperMutex.lock();
    if (sNeedWaitLooper) {
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() begin");
        sem_wait(&sWaitLooperSem);
        sNeedWaitLooper = false;
        sem_destroy(&sWaitLooperSem);
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() end");
    }
    sWaitLooperMutex.unlock();
    return getLooper();
}

void RfxMainThread::clearMessages() {
    RFX_LOG_D(RFX_LOG_TAG, "clearMessages()");
    RfxMessageHandler::setIgnoreTimeStamp(systemTime(SYSTEM_TIME_MONOTONIC));
}

void RfxMainThread::initControllers() {
    RFX_LOG_D(RFX_LOG_TAG, "initControllers start");
    // initialize controllers
    RfxControllerFactory::createControllers();

    // Create TestSuitController For test.
    RFX_OBJ_GET_INSTANCE(RfxTestSuitController);

    RFX_LOG_D(RFX_LOG_TAG, "initControllers end");

    RFX_LOG_D(RFX_LOG_TAG, "initControllers process async queue start");

    RfxAsyncSignalQueue* async_queue = RFX_OBJ_GET_INSTANCE(RfxAsyncSignalQueue);
    do {
        // Process Post Event
        async_queue->processEmit();
    } while (!async_queue->isEmpty());

    RFX_LOG_D(RFX_LOG_TAG, "initControllers process async queue end");
#ifdef RFX_OBJ_DEBUG
    RfxDebugInfo::dumpIfNeed();
#endif
}

bool RfxMainThread::threadLoop() {
#ifdef RFX_OBJ_DEBUG
    RfxDebugInfo::updateDebugInfoSwitcher();
#endif

    // start message loop
    m_looper = Looper::prepare(0);

    sem_post(&sWaitLooperSem);

    // watchdog for init controller
    _start_watch_dog(RFX_MAIN_THREAD_WATCHDOG_TIME);

    initControllers();

    _stop_watch_dog();

    int result;
    do {
        result = m_looper->pollAll(-1);
        RFX_LOG_D(RFX_LOG_TAG, "threadLoop, result = %d", result);
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);

    RFX_LOG_D(RFX_LOG_TAG, "threadLoop, m_looper.get() = %p", m_looper.get());

    RFX_ASSERT(0);  // Can't go here
    return true;
}

void RfxMainThread::enqueueMessage(const sp<RfxMessage>& message) {
    RFX_ASSERT(s_self != NULL && s_self->m_looper != NULL);
    sp<MessageHandler> handler = new RfxMessageHandler(message);
    RFX_LOG_D(RFX_LOG_TAG, "enqueueMessage(), mainHandler = %p, msg = [%s]", handler.get(),
              message->toString().string());
    s_self->m_looper->sendMessage(handler, s_self->m_dummy_msg);
}

void RfxMainThread::enqueueMessageFront(const sp<RfxMessage>& message) {
    RFX_ASSERT(s_self != NULL && s_self->m_looper != NULL);
    sp<MessageHandler> handler = new RfxMessageHandler(message);
    RFX_LOG_D(RFX_LOG_TAG, "enqueueMessageFront(), mainHandler = %p, msg = [%s]", handler.get(),
              message->toString().string());
    s_self->m_looper->sendMessageAtTime(0, handler, s_self->m_dummy_msg);
}

sp<Looper> RfxMainThread::getLooper() {
    RFX_ASSERT(s_self != NULL);
    return s_self->m_looper;
}
