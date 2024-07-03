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

#ifndef BASE64_H
#define BASE64_H
#include <stdio.h>
#include <stdlib.h>

unsigned char* base64_encode(const unsigned char* src, size_t len, size_t* out_len);
unsigned char* base64_decode(const unsigned char* src, size_t len, size_t* out_len);
unsigned char* base64_decode_to_str(const unsigned char* src, size_t len);
unsigned char* base64_encode_to_str(const unsigned char* src, size_t len);
unsigned char* byteArrayToHexStringB64(unsigned char* array, size_t length);
size_t hexStringToByteArrayB64(const unsigned char* hexString, unsigned char** byte);

#define BASE64_UNUSED(x) (x)

#endif /* BASE64_H */
