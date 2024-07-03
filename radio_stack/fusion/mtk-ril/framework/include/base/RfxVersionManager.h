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

#ifndef __RFX_VERSION_MANAGER_H__
#define __RFX_VERSION_MANAGER_H__

#include "utils/Vector.h"
#include "utils/String8.h"
#include "RfxAtLine.h"

using ::android::String8;
using ::android::Vector;
using namespace std;

class FeatureVersion {
  public:
    FeatureVersion() : mFeature(String8()), mVersion(-1) {}

    FeatureVersion(String8 feature, int version) : mFeature(feature), mVersion(version) {}

    String8 getFeature() { return mFeature; }

    int getVersion() { return mVersion; }

  private:
    String8 mFeature;
    int mVersion;
};

class RfxVersionManager {
  public:
    static RfxVersionManager* init();

    static RfxVersionManager* getInstance();

  public:
    void initVersion(RfxAtLine* line);

    int getFeatureVersion(char* feature, int defaultVaule);

    // if no corresponding feature name, return 0
    int getFeatureVersion(char* feature);

  private:
    void waitVersion();

    static RfxVersionManager* sSelf;

    Vector<FeatureVersion> mVersionList;
};

#endif
