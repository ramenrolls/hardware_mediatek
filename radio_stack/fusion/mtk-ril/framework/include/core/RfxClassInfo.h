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

#ifndef __RFX_CLASS_INFO_H__
#define __RFX_CLASS_INFO_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <sys/types.h>

/*****************************************************************************
 * Typedef
 *****************************************************************************/

// Pre-defined classes
class RfxObject;

// Prototype of RfxClassInfo create object function pointer
typedef RfxObject* (*RfxClassInfoCreateObjectFuncPtr)(RfxObject* parent);

/*****************************************************************************
 * Class RfxClassInfo
 *****************************************************************************/
typedef struct _rfx_class_info_struct {
    // pointer to the name of the class
    const char* m_class_name;
    // pointer to the parent class info of the class
    const struct _rfx_class_info_struct* m_parent;
    // Function pointer to create a object instance
    RfxClassInfoCreateObjectFuncPtr m_create_object_callback;
} rfx_class_info_struct;

/*
 * Class RfxClassInfo,
 *  prototype of class information, descript the class name, the parent class,
 *  and how to create instances of the class
 */
class RfxClassInfo : public _rfx_class_info_struct {
    // Constructor / Destructor
  public:
    // Constructor
    RfxClassInfo(const char* class_name, struct _rfx_class_info_struct* parent,
                 RfxClassInfoCreateObjectFuncPtr create_obj_instance_callback) {
        m_class_name = class_name;
        m_parent = parent;
        m_create_object_callback = create_obj_instance_callback;
    }

    // Method
  public:
    // Return the class name.
    //
    // RETURNS: Return the class name.
    const char* getClassName() const { return m_class_name; }

    // Return the parent class into.
    //
    // RETURNS: Return the parent class. Return NULL if there is no parent class
    RfxClassInfo* getParent() const { return (RfxClassInfo*)m_parent; }

    // Check the class whatever inherit from the given class.
    //
    // RETURNS: Return true if the class inherit from the given class.
    //  Otherwise, return false.
    bool isKindOf(const RfxClassInfo* classInfo  // [IN] The class to check
    ) const;

    // Create an object instance of the class.
    RfxObject* createObject(RfxObject* parent  // [IN] The parent object to create new object
    ) const {
        return m_create_object_callback ? m_create_object_callback(parent) : NULL;
    }
};

#endif /* __RFX_CLASS_INFO_H__ */
