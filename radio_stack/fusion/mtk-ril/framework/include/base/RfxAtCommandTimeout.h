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

// {AT command, timeout} // mins
{(char*)"CGACT", 15}, {(char*)"CGATT", 15}, {(char*)"COPS", 15},
        {(char*)"CMGS", 10},  // SMS can over CS or IMS and will re-try on different domain. The
                              // worst case will be around 8 mins
        {(char*)"EFUN", 15}, {(char*)"EGACT", 15}, {(char*)"EAPNACT", 15}, {(char*)"EPDN", 15},
        {(char*)"EOPS", 15}, {(char*)"EDALLOW", 15},
