/*
 * Internal header for the s3eAndroidFullscreen extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef S3EANDROIDFULLSCREEN_INTERNAL_H
#define S3EANDROIDFULLSCREEN_INTERNAL_H

#include "s3eTypes.h"
#include "s3eAndroidFullscreen.h"
#include "s3eAndroidFullscreen_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult s3eAndroidFullscreenInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult s3eAndroidFullscreenInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void s3eAndroidFullscreenTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void s3eAndroidFullscreenTerminate_platform();
s3eBool s3eAndroidFullscreenIsImmersiveSupported_platform();

void s3eAndroidFullscreenOn_platform(s3eBool immersive, s3eBool stickyNavBar, s3eBool staticLayout);

void s3eAndroidFullscreenOff_platform(s3eBool showStatusBar, s3eBool showNavBar, s3eBool staticLayout);


#endif /* !S3EANDROIDFULLSCREEN_INTERNAL_H */