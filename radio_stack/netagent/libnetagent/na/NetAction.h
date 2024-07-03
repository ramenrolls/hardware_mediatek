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

#ifndef __NET_ACTION_H__
#define __NET_ACTION_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/RefBase.h>

using ::android::RefBase;
using ::android::sp;

/*****************************************************************************
 * Class NetObject
 *****************************************************************************/
class NetObject : public virtual RefBase {
  public:
    NetObject() {}

    virtual ~NetObject() {}
};

/*****************************************************************************
 * Class NetBase
 *****************************************************************************/
typedef void (NetObject::*NetObjMemFuncPtr)();
class NetBase {
  public:
    NetBase(NetObject* obj, NetObjMemFuncPtr callCallback)
        : m_obj_ptr(obj), m_callback(callCallback) {}
    virtual ~NetBase() {}

  public:
    NetObject* m_obj_ptr;
    NetObjMemFuncPtr m_callback;
};

/*****************************************************************************
 * Class NetActionArg
 *****************************************************************************/
template <typename... Ts>
class NetActionArg {
  public:
    std::tuple<Ts...> args;

    NetActionArg(Ts... args) : args(std::tuple<Ts...>(args...)) {}
    virtual ~NetActionArg() {}
};

/*****************************************************************************
 * Class NetPostInvoke
 *****************************************************************************/
template <typename... Ts>
class NetPostInvoke : public NetBase {
  public:
    typedef void (NetObject::*ObjMemFunc)(bool bSuccess, Ts... args);

  public:
    NetActionArg<Ts...> m_args;

  public:
    template <class _MemFunc>
    NetPostInvoke(NetObject* obj, _MemFunc func, Ts... args)
        : NetBase(obj, (NetObjMemFuncPtr) static_cast<ObjMemFunc>(func)), m_args(args...) {}
    virtual ~NetPostInvoke() {}
};

/*****************************************************************************
 * Class NetAction
 *****************************************************************************/
class NetActionBase : public virtual RefBase {
  public:
    virtual void ack(bool bSuccess = true) = 0;

  public:
    NetActionBase() {}
    virtual ~NetActionBase() {}
};

/*****************************************************************************
 * Class NetAction
 *****************************************************************************/
template <typename... Ts>
class NetAction : public virtual NetActionBase {
  public:
    template <class _MemFunc>
    NetAction(NetObject* obj,  // [IN] the callback object
              _MemFunc func,   // [IN] the callback object member function
              Ts... args       // [IN] n parameters
              )
        : m_invoke(obj, func, args...) {}
    virtual ~NetAction() {}

  public:
    virtual void ack(bool bSuccess = true) { callback(bSuccess, std::index_sequence_for<Ts...>()); }

  private:
    template <std::size_t... Is>
    void callback(bool bSuccess, std::index_sequence<Is...>) {
        typedef NetPostInvoke<Ts...> PostInvoke;
        NetObject* obj = m_invoke.m_obj_ptr;
        if (obj != NULL) {
            typename PostInvoke::ObjMemFunc memFunc =
                    (typename PostInvoke::ObjMemFunc)m_invoke.m_callback;
            (obj->*memFunc)(bSuccess, std::get<Is>(m_invoke.m_args.args)...);
        }
    }

  private:
    NetPostInvoke<Ts...> m_invoke;
};

#endif /* __NET_ACTION_H__ */
