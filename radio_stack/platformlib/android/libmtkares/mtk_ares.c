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

#include <mtk_ares.h>

int mtk_aes_getrecords_free(struct records_naptr* head) { return aes_getrecords_free(head); }

int mtk_aes_getrecords(const char* hostname, const char* service, const struct query_type* hints,
                       struct records_naptr** result) {
    return aes_getrecords(hostname, service, hints, result);
}
