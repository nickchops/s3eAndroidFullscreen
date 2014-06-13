/*
 * android-specific implementation of the s3eAndroidFullscreen extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eAndroidFullscreen_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

static jobject g_Obj;
static jmethodID g_s3eAndroidFullscreenIsImmersiveSupported;
static jmethodID g_s3eAndroidFullscreenOn;
static jmethodID g_s3eAndroidFullscreenOff;
static jmethodID g_onWindowFocusChanged;

// Callback to re-enable sticky navbar
// s3eEdkThreadRunOnOS needed to call UI functions
static void* _s3eAndroidFullscreenResume()
{
	JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_onWindowFocusChanged, true);
}
static int32 _s3eAndroidFullscreenResumeCallback(void* systemData, void* userData)
{
    IwTrace(ANDROID_FULLSCREEN, ("re-hiding nav bar on resume (if sticky mode is on)"));
	s3eEdkThreadRunOnOS(_s3eAndroidFullscreenResume, 0);
    return 0;
}

s3eResult s3eAndroidFullscreenInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("s3eAndroidFullscreen");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_s3eAndroidFullscreenIsImmersiveSupported = env->GetMethodID(cls, "s3eAndroidFullscreenIsImmersiveSupported", "()Z");
    if (!g_s3eAndroidFullscreenIsImmersiveSupported)
        goto fail;

    g_s3eAndroidFullscreenOn = env->GetMethodID(cls, "s3eAndroidFullscreenOn", "(ZZZ)V");
    if (!g_s3eAndroidFullscreenOn)
        goto fail;

    g_s3eAndroidFullscreenOff = env->GetMethodID(cls, "s3eAndroidFullscreenOff", "(ZZZ)V");
    if (!g_s3eAndroidFullscreenOff)
        goto fail;

	//non-public callback
	g_onWindowFocusChanged = env->GetMethodID(cls, "onWindowFocusChanged", "(Z)V");
    if (!g_onWindowFocusChanged)
        goto fail;

    IwTrace(ANDROIDFULLSCREEN, ("ANDROIDFULLSCREEN init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    if (s3eAndroidFullscreenIsImmersiveSupported() == S3E_TRUE)
    {
    	// Using device resume callback to re-apply sticky mode fullscreen as OS will have turned
		// it off (see java code for why we can't use android activity interface for this)
		// Only want to do this for sticky mode - in non sticky mode the nav bar should always be
		// shown on resume as there's no way of telling if the OS or the user has shown it
        if (s3eDeviceRegister(S3E_DEVICE_UNPAUSE, _s3eAndroidFullscreenResumeCallback, NULL) != S3E_RESULT_SUCCESS)
            IwTrace(ANDROID_FULLSCREEN, ("failed to register callback to re-hide nav bar on app resume"));
	}

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(s3eAndroidFullscreen, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;
}

void s3eAndroidFullscreenTerminate_platform()
{
	if (s3eAndroidFullscreenIsImmersiveSupported() == S3E_TRUE)
    {
    	s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, _s3eAndroidFullscreenResumeCallback);
    }
}

//-----------------

s3eBool s3eAndroidFullscreenIsImmersiveSupported_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (s3eBool)env->CallBooleanMethod(g_Obj, g_s3eAndroidFullscreenIsImmersiveSupported);
}

void s3eAndroidFullscreenOn_platform(s3eBool immersive, s3eBool stickyNavBar, s3eBool staticLayout)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eAndroidFullscreenOn, immersive, stickyNavBar, staticLayout);
}

void s3eAndroidFullscreenOff_platform(s3eBool showStatusBar, s3eBool showNavBar, s3eBool staticLayout)
{
	JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eAndroidFullscreenOff, showStatusBar, showNavBar, staticLayout);
}
