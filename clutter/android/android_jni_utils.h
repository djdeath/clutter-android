#ifndef __ANDROID_JNI_UTILS_H__
#define __ANDROID_JNI_UTILS_H__

#include <jni.h>

#include "android_native_app_glue.h"

jint _android_show_keyboard (struct android_app *mApplication,
                             jboolean show, jint flags);


#endif /* __ANDROID_JNI_UTILS_H__ */
