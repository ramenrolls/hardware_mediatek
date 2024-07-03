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

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H 1

#if defined __has_cpp_attribute
#if __has_cpp_attribute(fallthrough)
#define TELEPHONYWARE_FALLTHROUGH [[fallthrough]]
#else
#define TELEPHONYWARE_FALLTHROUGH
#endif
#else
#define TELEPHONYWARE_FALLTHROUGH
#endif

#endif /*COMPILER_UTILS_H*/
