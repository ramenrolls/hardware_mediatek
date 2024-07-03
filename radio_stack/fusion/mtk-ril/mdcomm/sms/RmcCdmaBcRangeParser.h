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

#ifndef __RMC_CDMA_BC_RANGE_PARSER_H__
#define __RMC_CDMA_BC_RANGE_PARSER_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include <Vector.h>
#include <SortedVector.h>

/*****************************************************************************
 * Name Space
 *****************************************************************************/
using android::SortedVector;
using android::Vector;

/*****************************************************************************
 * Typedef
 *****************************************************************************/
/*
 * The structure stands for cell broadcast ranges
 * such as 4096-4096, 4098-4101
 */
typedef struct {
    int start;
    int end;
} Range;

/*****************************************************************************
 * Define
 *****************************************************************************/
/*
 * The max cell broadcast cateogory range number
 * supported by MD
 */
#define MAX_RANGE 10

/*
 * The max character number of a range
 * include '\0'
 * range example: "65534-65535", total 14 character,
 * allow to include some spaces
 */
#define MAX_RANGE_CHARACTER 20

/* A range include a start endpoint and a end endpoint */
#define VALID_RANGE_ENDPOINT_NUM 2

/*****************************************************************************
 * Class RmcCdmaBcRangeParser
 *****************************************************************************/
class RmcCdmaBcRangeParser {
  public:
    static int getRangeFromModem(char* cateogry, Range* r);
    static void getRange(Vector<Range>& range, SortedVector<int>& input);

  private:
    static void split(char* src, const char* sep, char dest[][MAX_RANGE_CHARACTER], int* num);
    static char* trim(char* s);
    static char* skipQuote(char* s);
};
#endif /* __RMC_CDMA_BC_RANGE_PARSER_H__ */
