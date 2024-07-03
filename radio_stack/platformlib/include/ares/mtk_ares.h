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

#ifndef __MTK_ARES_H
#define __MTK_ARES_H

#ifdef __cplusplus
extern "C" {
#endif

struct records_naptr {
    unsigned int order;
    unsigned int pref;
    char* flags;
    char* service;
    char* regexp;
    char* fqdn;
    struct records_naptr* next;
};

struct query_type {
    int ai_family;
    int ai_protocol;
    int ai_socktype;
    int ai_port;
    int ai_type;
};

extern int aes_getrecords(const char* hostname, const char* service, const struct query_type* hints,
                          struct records_naptr** result);
extern int aes_getrecords_free(struct records_naptr* head);

int mtk_aes_getrecords(const char* hostname, const char* service, const struct query_type* hints,
                       struct records_naptr** result);
int mtk_aes_getrecords_free(struct records_naptr* head);

#ifdef __cplusplus
}
#endif

#endif /* __MTK_ARES_H */
