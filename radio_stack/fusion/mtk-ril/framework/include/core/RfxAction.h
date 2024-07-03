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

#ifndef __RFX_ACTION_H__
#define __RFX_ACTION_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <utils/RefBase.h>
#include <utils/Vector.h>
#include "RfxSignal.h"
#include "RfxDebugInfo.h"
#include "RfxDefs.h"
#include "RfxLog.h"

using ::android::RefBase;
using ::android::Vector;

/*****************************************************************************
 * Class RfxAction
 *****************************************************************************/

class RfxAction : public virtual RefBase
#ifdef RFX_OBJ_DEBUG
    ,
                  public IRfxDebugLogger
#endif
{
  public:
    virtual void act() = 0;

#ifdef RFX_OBJ_DEBUG
  public:
    RfxAction();

    virtual ~RfxAction();

    virtual void dump(int level = 0) const = 0;

    static void dumpActionList();

    static void dumpActionConstructionCallStack(void* action_address);

  private:
    RfxDebugInfo* m_debug_info;
    static Vector<RfxDebugInfo*>* s_root_action_debug_info;
#endif  //#ifdef RFX_OBJ_DEBUG
};

class RfxAction0 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxAction0(RfxObject* obj,  // [IN] the callback object
               _MemFunc func    // [IN] the callback object member function
               )
        : m_invoke(obj, func) {}

  public:
    virtual void act() {
        RfxPostInvoke0* invoke;
        invoke = new RfxPostInvoke0(m_invoke.m_obj_ptr.promote().get(), m_invoke.m_callback);
        RFX_OBJ_GET_INSTANCE(RfxAsyncSignalUtil)->callback0(invoke);
    }

  private:
    RfxPostInvoke0 m_invoke;

#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxAction0 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

class RfxPostAction0 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxPostAction0(RfxObject* obj,  // [IN] the callback object
                   _MemFunc func    // [IN] the callback object member function
                   )
        : m_invoke(obj, func) {}

  public:
    virtual void act() { rfxPostInvoke0(m_invoke.m_obj_ptr.promote().get(), m_invoke.m_callback); }

  private:
    RfxPostInvoke0 m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxPostAction0 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1>
class RfxAction1 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxAction1(RfxObject* obj,  // [IN] the callback object
               _MemFunc func,   // [IN] the callback object member function
               _Arg1 arg1       // [IN] the 1'st argument
               )
        : m_invoke(obj, func, arg1) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke1<_Arg1> PostInvoke;
        PostInvoke* invoke;
        invoke = new PostInvoke(m_invoke.m_obj_ptr.promote().get(),
                                (typename PostInvoke::ObjMemFunc)m_invoke.m_callback,
                                m_invoke.m_args.arg1);
        RFX_OBJ_GET_INSTANCE(RfxAsyncSignalUtil)->callback1<_Arg1>(invoke);
    }

  private:
    RfxPostInvoke1<_Arg1> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxAction1 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1>
class RfxPostAction1 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxPostAction1(RfxObject* obj,  // [IN] the callback object
                   _MemFunc func,   // [IN] the callback object member function
                   _Arg1 arg1       // [IN] the 1'st argument
                   )
        : m_invoke(obj, func, arg1) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke1<_Arg1> PostInvoke;
        rfxPostInvoke1(m_invoke.m_obj_ptr.promote().get(),
                       (typename PostInvoke::ObjMemFunc)m_invoke.m_callback, m_invoke.m_args.arg1);
    }

  private:
    RfxPostInvoke1<_Arg1> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxPostAction1 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1, class _Arg2>
class RfxAction2 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxAction2(RfxObject* obj,  // [IN] the callback object
               _MemFunc func,   // [IN] the callback object member function
               _Arg1 arg1,      // [IN] the 1'st argument
               _Arg2 arg2       // [IN] the 2'nd argument
               )
        : m_invoke(obj, func, arg1, arg2) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke2<_Arg1, _Arg2> PostInvoke;
        PostInvoke* invoke;
        invoke = new PostInvoke(m_invoke.m_obj_ptr.promote().get(),
                                (typename PostInvoke::ObjMemFunc)m_invoke.m_callback,
                                m_invoke.m_args.arg1, m_invoke.m_args.arg2);
        RFX_OBJ_GET_INSTANCE(RfxAsyncSignalUtil)->callback2<_Arg1, _Arg2>(invoke);
    }

  private:
    RfxPostInvoke2<_Arg1, _Arg2> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxAction2 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1, class _Arg2>
class RfxPostAction2 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxPostAction2(RfxObject* obj,  // [IN] the callback object
                   _MemFunc func,   // [IN] the callback object member function
                   _Arg1 arg1,      // [IN] the 1'st argument
                   _Arg2 arg2       // [IN] the 2'nd argument
                   )
        : m_invoke(obj, func, arg1, arg2) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke2<_Arg1, _Arg2> PostInvoke;
        rfxPostInvoke2(m_invoke.m_obj_ptr.promote().get(),
                       (typename PostInvoke::ObjMemFunc)m_invoke.m_callback, m_invoke.m_args.arg1,
                       m_invoke.m_args.arg2);
    }

  private:
    RfxPostInvoke2<_Arg1, _Arg2> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxPostAction2 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1, class _Arg2, class _Arg3>
class RfxAction3 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxAction3(RfxObject* obj,  // [IN] the callback object
               _MemFunc func,   // [IN] the callback object member function
               _Arg1 arg1,      // [IN] the 1'st argument
               _Arg2 arg2,      // [IN] the 2'nd argument
               _Arg3 arg3       // [IN] the 3'rd argument
               )
        : m_invoke(obj, func, arg1, arg2, arg3) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke3<_Arg1, _Arg2, _Arg3> PostInvoke;
        PostInvoke* invoke;
        invoke = new PostInvoke(m_invoke.m_obj_ptr.promote().get(),
                                (typename PostInvoke::ObjMemFunc)m_invoke.m_callback,
                                m_invoke.m_args.arg1, m_invoke.m_args.arg2, m_invoke.m_args.arg3);
        RFX_OBJ_GET_INSTANCE(RfxAsyncSignalUtil)->callback3<_Arg1, _Arg2, _Arg3>(invoke);
    }

  private:
    RfxPostInvoke3<_Arg1, _Arg2, _Arg3> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxAction3 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1, class _Arg2, class _Arg3>
class RfxPostAction3 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxPostAction3(RfxObject* obj,  // [IN] the callback object
                   _MemFunc func,   // [IN] the callback object member function
                   _Arg1 arg1,      // [IN] the 1'st argument
                   _Arg2 arg2,      // [IN] the 2'nd argument
                   _Arg3 arg3       // [IN] the 3'rd argument
                   )
        : m_invoke(obj, func, arg1, arg2, arg3) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke3<_Arg1, _Arg2, _Arg3> PostInvoke;
        rfxPostInvoke3(m_invoke.m_obj_ptr.promote().get(),
                       (typename PostInvoke::ObjMemFunc)m_invoke.m_callback, m_invoke.m_args.arg1,
                       m_invoke.m_args.arg2, m_invoke.m_args.arg3);
    }

  private:
    RfxPostInvoke3<_Arg1, _Arg2, _Arg3> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxPostAction3 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1, class _Arg2, class _Arg3, class _Arg4>
class RfxAction4 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxAction4(RfxObject* obj,  // [IN] the callback object
               _MemFunc func,   // [IN] the callback object member function
               _Arg1 arg1,      // [IN] the 1'st argument
               _Arg2 arg2,      // [IN] the 2'nd argument
               _Arg3 arg3,      // [IN] the 3'rd argument
               _Arg4 arg4       // [IN] the 4'th argument
               )
        : m_invoke(obj, func, arg1, arg2, arg3, arg4) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke4<_Arg1, _Arg2, _Arg3, _Arg4> PostInvoke;
        PostInvoke* invoke;
        invoke = new PostInvoke(m_invoke.m_obj_ptr.promote().get(),
                                (typename PostInvoke::ObjMemFunc)m_invoke.m_callback,
                                m_invoke.m_args.arg1, m_invoke.m_args.arg2, m_invoke.m_args.arg3,
                                m_invoke.m_args.arg4);
        RFX_OBJ_GET_INSTANCE(RfxAsyncSignalUtil)->callback4<_Arg1, _Arg2, _Arg3, _Arg4>(invoke);
    }

  private:
    RfxPostInvoke4<_Arg1, _Arg2, _Arg3, _Arg4> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxAction4 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

template <class _Arg1, class _Arg2, class _Arg3, class _Arg4>
class RfxPostAction4 : public RfxAction {
  public:
    template <class _MemFunc>
    RfxPostAction4(RfxObject* obj,  // [IN] the callback object
                   _MemFunc func,   // [IN] the callback object member function
                   _Arg1 arg1,      // [IN] the 1'st argument
                   _Arg2 arg2,      // [IN] the 2'nd argument
                   _Arg3 arg3,      // [IN] the 3'rd argument
                   _Arg4 arg4       // [IN] the 4'th argument
                   )
        : m_invoke(obj, func, arg1, arg2, arg3, arg4) {}

  public:
    virtual void act() {
        typedef RfxPostInvoke4<_Arg1, _Arg2, _Arg3, _Arg4> PostInvoke;
        rfxPostInvoke4(m_invoke.m_obj_ptr.promote().get(),
                       (typename PostInvoke::ObjMemFunc)m_invoke.m_callback, m_invoke.m_args.arg1,
                       m_invoke.m_args.arg2, m_invoke.m_args.arg3, m_invoke.m_args.arg4);
    }

  private:
    RfxPostInvoke4<_Arg1, _Arg2, _Arg3, _Arg4> m_invoke;
#ifdef RFX_OBJ_DEBUG
  public:
    virtual void dump(int level = 0) const {
        RfxObject* obj = m_invoke.m_obj_ptr.promote().get();
        RFX_LOG_D(RFX_DEBUG_INFO_TAG, "RfxPostAction4 = %p, obj = %p, %s", this, obj,
                  obj == NULL ? "" : obj->getClassInfo()->getClassName());
        RFX_UNUSED(level);
    }
#endif  //#ifdef RFX_OBJ_DEBUG
};

#endif /* __RFX_ACTION_H__ */
