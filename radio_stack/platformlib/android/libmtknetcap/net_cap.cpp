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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <android/log.h>
#include <log/log.h>
#include <inttypes.h>

#include "net_cap.h"
#include <mtk_log.h>

#include <sys/poll.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#undef LOG_TAG
#define LOG_TAG "NET_CAP"
#define WA_LOG_TAG "wpfa_iptable_android"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

int wpfaExecIptable(char** args) {
    /* This API is implemented for wpfa in AP.
     * The parameter args is the filter rules from MD, and wants to register to Linux.
     */
    char returns[1000];
    int pipefd[2];

    sigset_t blockset;
    sigset_t oldset;
    sigemptyset(&blockset);
    sigaddset(&blockset, SIGINT);
    sigaddset(&blockset, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &blockset, &oldset);

    int pipeRet = pipe(pipefd);

    if (pipeRet != 0) {
        mtkLogD(WA_LOG_TAG, "***ERROR: pipe failed, but no need to return to keep original flow.");
    }

    pid_t pid;
    int status;
    memset(returns, '\0', 1000);

    mtkLogD(WA_LOG_TAG, "forking");
    if ((pid = fork()) < 0) { /* fork a child process           */
        // puts("*** ERROR: forking child process failed\n");
        mtkLogD(WA_LOG_TAG, "***ERROR: forking child process failed");
        exit(1);
    } else if (pid == 0) { /* for the child process: */
        pthread_sigmask(SIG_SETMASK, &oldset, NULL);
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        dup2(pipefd[1], 2);
        close(pipefd[1]);

        if (execvp(args[0], args) < 0) { /* execute the command  */
            mtkLogD(WA_LOG_TAG, "***ERROR: exec failed");
            exit(1);
        }
        mtkLogD(WA_LOG_TAG, "***ERROR: exec failed, so child process return");
        return WPFA_IPTABLE_POLL_WAIT_ERR;
    } else { /* for the parent:      */
        close(pipefd[1]);
        returns[0] = '\0';
        int parent_read = pipefd[0];
        int b = 0, sz = 0;
        int rc = 0;
        int found_child = 0;
        struct pollfd poll_fds[] = {
                [0] =
                        {
                                .fd = parent_read,
                                .events = POLLIN,
                        },
        };
        mtkLogD(WA_LOG_TAG, "wait polling");

        while (!found_child) {
            if (TEMP_FAILURE_RETRY(poll(poll_fds, ARRAY_SIZE(poll_fds), -1)) < 0) {
                mtkLogD(WA_LOG_TAG, "poll failed");
                goto err_poll;
            }
            if (poll_fds[0].revents & POLLIN) {
                sz = TEMP_FAILURE_RETRY(read(parent_read, &returns[b], sizeof(returns) - 1 - b));

                // sz += b;
                b += sz;  // fix sz to single line
            }

            if (poll_fds[0].revents & POLLHUP) {
                int ret;
                mtkLogD(WA_LOG_TAG, "poll() recv POLLHUP, pid=%d", pid);
                ret = TEMP_FAILURE_RETRY(waitpid(pid, &status, 0));
                if (ret < 0) {
                    rc = errno;
                    mtkLogD(WA_LOG_TAG, "waitpid failed with %s\n", strerror(errno));
                    goto err_waitpid;
                }
                if (ret > 0) {
                    found_child = 1;
                }
            }
        }  // end-of-while

        mtkLogD(WA_LOG_TAG, "poll()done, b = %d,  and executeIptable returns=%s", b, returns);
        if (b != 0) {
            close(pipefd[0]);
            mtkLogD(WA_LOG_TAG, "error in executeIptable, return");
            return WPFA_IPTABLE_EXEC_FALED;
        }
        close(pipefd[0]);
        return WPFA_IPTABLE_EXEC_OVER;
    err_poll:
    err_waitpid:
        close(pipefd[0]);
        return WPFA_IPTABLE_POLL_WAIT_ERR;
    }
}
