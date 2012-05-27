#include <jni.h>

#include "android_native_app_glue.h"

extern "C" jint
_android_show_keyboard (struct android_app *mApplication, jboolean show, jint flags)
{
  jint lResult;
  jint lFlags = flags;
  jboolean pShow = show;

  JavaVM* lJavaVM = mApplication->activity->vm;
  JNIEnv* lJNIEnv;

  bool attached = false;
  switch (lJavaVM->GetEnv ((void**) &lJNIEnv, JNI_VERSION_1_6))
    {
    case JNI_OK:
      break;
    case JNI_EDETACHED:
      if (lJavaVM->AttachCurrentThread (&lJNIEnv, NULL) != 0)
        {
          return 1;
        }
      attached = true;
      break;
    case JNI_EVERSION:
      return 2;
    }

  // Retrieves NativeActivity.
  jobject lNativeActivity = mApplication->activity->clazz;
  jclass ClassNativeActivity = lJNIEnv->GetObjectClass (lNativeActivity);

  // Retrieves Context.INPUT_METHOD_SERVICE.
  jclass ClassContext = lJNIEnv->FindClass ("android/content/Context");
  jfieldID FieldINPUT_METHOD_SERVICE =
    lJNIEnv->GetStaticFieldID (ClassContext,
                               "INPUT_METHOD_SERVICE",
                               "Ljava/lang/String;");
  jobject INPUT_METHOD_SERVICE =
    lJNIEnv->GetStaticObjectField (ClassContext,
                                   FieldINPUT_METHOD_SERVICE);
  if (INPUT_METHOD_SERVICE == NULL)
    return 4;

  // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
  jclass ClassInputMethodManager =
    lJNIEnv->FindClass ("android/view/inputmethod/InputMethodManager");
   jmethodID MethodGetSystemService =
     lJNIEnv->GetMethodID (ClassNativeActivity, "getSystemService",
                           "(Ljava/lang/String;)Ljava/lang/Object;");
   jobject lInputMethodManager =
     lJNIEnv->CallObjectMethod (lNativeActivity,
                                MethodGetSystemService,
                                INPUT_METHOD_SERVICE);

   // Runs getWindow().getDecorView().
   jmethodID MethodGetWindow = lJNIEnv->GetMethodID (ClassNativeActivity,
                                                     "getWindow",
                                                     "()Landroid/view/Window;");
   jobject lWindow = lJNIEnv->CallObjectMethod (lNativeActivity,
                                                MethodGetWindow);
   jclass ClassWindow = lJNIEnv->FindClass ("android/view/Window");
   jmethodID MethodGetDecorView = lJNIEnv->GetMethodID (ClassWindow,
                                                        "getDecorView",
                                                        "()Landroid/view/View;");
   jobject lDecorView = lJNIEnv->CallObjectMethod (lWindow,
                                                   MethodGetDecorView);

   jint retval = 0;

   if (pShow) {
     // Runs lInputMethodManager.showSoftInput(...).
     jmethodID MethodShowSoftInput =
       lJNIEnv->GetMethodID (ClassInputMethodManager, "showSoftInput",
                             "(Landroid/view/View;I)Z");
     jboolean lResult = lJNIEnv->CallBooleanMethod (lInputMethodManager,
                                                    MethodShowSoftInput,
                                                    lDecorView, lFlags);

     retval = lResult;
   } else {
     // Runs lWindow.getViewToken()
     jclass ClassView = lJNIEnv->FindClass ("android/view/View");
     jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID (ClassView,
                                                            "getWindowToken",
                                                            "()Landroid/os/IBinder;");
     jobject lBinder = lJNIEnv->CallObjectMethod (lDecorView,
                                                  MethodGetWindowToken);

     // lInputMethodManager.hideSoftInput(...).
     jmethodID MethodHideSoftInput =
       lJNIEnv->GetMethodID (ClassInputMethodManager,
                             "hideSoftInputFromWindow",
                             "(Landroid/os/IBinder;I)Z");
     jboolean lRes = lJNIEnv->CallBooleanMethod (lInputMethodManager,
                                                 MethodHideSoftInput,
                                                 lBinder, lFlags);

     retval = lRes;
   }

   // Finished with the JVM.
   lJavaVM->DetachCurrentThread();

   return retval;
}
