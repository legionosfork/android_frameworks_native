/*
 * Copyright 2017 The Android Open Source Project
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

#ifndef ANDROID_UI_GRAPHICS_ENV_H
#define ANDROID_UI_GRAPHICS_ENV_H 1

#include <string>

struct android_namespace_t;

namespace android {

class NativeLoaderNamespace;

class GraphicsEnv {
public:
    static GraphicsEnv& getInstance();

    // Set a search path for loading graphics drivers. The path is a list of
    // directories separated by ':'. A directory can be contained in a zip file
    // (drivers must be stored uncompressed and page aligned); such elements
    // in the search path must have a '!' after the zip filename, e.g.
    //     /data/app/com.example.driver/base.apk!/lib/arm64-v8a
    void setDriverPath(const std::string path);
    android_namespace_t* getDriverNamespace();

    // Set a search path for loading ANGLE libraries. The path is a list of
    // directories separated by ':'. A directory can be contained in a zip file
    // (libraries must be stored uncompressed and page aligned); such elements
    // in the search path must have a '!' after the zip filename, e.g.
    //     /system/app/ANGLEPrebuilt/ANGLEPrebuilt.apk!/lib/arm64-v8a
    void setAngleInfo(const std::string path, const std::string appName, const bool optIn);
    android_namespace_t* getAngleNamespace();
    const char* getAngleAppName();
    bool getAngleDeveloperOptIn();

    void setLayerPaths(NativeLoaderNamespace* appNamespace, const std::string layerPaths);
    NativeLoaderNamespace* getAppNamespace();

    const std::string getLayerPaths();

    void setDebugLayers(const std::string layers);
    const std::string getDebugLayers();

private:
    GraphicsEnv() = default;
    std::string mDriverPath;
    std::string mAnglePath;
    std::string mAngleAppName;
    bool mAngleDeveloperOptIn;
    std::string mDebugLayers;
    std::string mLayerPaths;
    android_namespace_t* mDriverNamespace = nullptr;
    android_namespace_t* mAngleNamespace = nullptr;
    NativeLoaderNamespace* mAppNamespace = nullptr;
};

} // namespace android

/* FIXME
 * Export an un-mangled function that just does
 *     return android::GraphicsEnv::getInstance().getDriverNamespace();
 * This allows libEGL to get the function pointer via dlsym, since it can't
 * directly link against libgui. In a future release, we'll fix this so that
 * libgui does not depend on graphics API libraries, and libEGL can link
 * against it. The current dependencies from libgui -> libEGL are:
 *  - the GLConsumer class, which should be moved to its own library
 *  - the EGLsyncKHR synchronization in BufferQueue, which is deprecated and
 *    will be removed soon.
 */
extern "C" {
android_namespace_t* android_getDriverNamespace();
android_namespace_t* android_getAngleNamespace();
const char* android_getAngleAppName();
bool android_getAngleDeveloperOptIn();
}

#endif // ANDROID_UI_GRAPHICS_ENV_H
