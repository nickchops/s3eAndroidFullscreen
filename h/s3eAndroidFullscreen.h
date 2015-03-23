/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */
/*
 * WARNING: this is an autogenerated file and will be overwritten by
 * the extension interface script.
 */
#ifndef S3E_EXT_ANDROIDFULLSCREEN_H
#define S3E_EXT_ANDROIDFULLSCREEN_H


// \cond HIDDEN_DEFINES
S3E_BEGIN_C_DECL
// \endcond

/**
 * Returns S3E_TRUE if the AndroidFullscreen extension is available.
 */
s3eBool s3eAndroidFullscreenAvailable();

/* Returns true if OS version supports immersive mode (can hide navigation bar
 * and wont re-show it on user touches) Should return true only for Android 4.4 or newer.
 * The extension will fail to load and s3eAndroidFullscreenAvailable() will return
 * S3E_FALSE if the device doesn't support hiding the nav bar at all - i.e. on devices
 * running Android versions older than 4.0.
 */
s3eBool s3eAndroidFullscreenIsImmersiveSupported();

/* Hide navigation bar. Immersive mode will only be set if
 * s3eAndroidFullscreenIsImmersiveSupported returns true. If immersive is not used,
 * any touch will cause the nav bar to be displayed again. The user can always re-show the
 * nav bar by swiping onto the screen from the side where the bar normally is.
 * stickyNavBar causes the re-shown bar to be semi-transparent and to re-hide itself. In
 * sticky mode, the bar also re-hides itself when the app gains focus again. The extension
 * doesn't yet re-hide the bar after other events e.g. after alerts like s3eDialog. You'll
 * have to re-call s3eAndroidFullscreenOn after these show if possible.
 * staticLayout causes any native UI to fill the whole screen and not to try and
 * change its layout if the nav bar then shows or hides.
 */
void s3eAndroidFullscreenOn(s3eBool immersive S3E_DEFAULT(S3E_TRUE), s3eBool stickyNavBar S3E_DEFAULT(S3E_TRUE), s3eBool staticLayout S3E_DEFAULT(S3E_TRUE));

/* Re-show the nav bar. Optionally use showStatusBar to try and show the status bar if
 * the app supports it. Optionally set staticLayout to false to make the UI/GL view re-size
 * to not be under the nav bar anymore.
 */
void s3eAndroidFullscreenOff(s3eBool showStatusBar S3E_DEFAULT(S3E_FALSE), s3eBool showNavBar S3E_DEFAULT(S3E_TRUE), s3eBool staticLayout S3E_DEFAULT(S3E_TRUE));

// \cond HIDDEN_DEFINES
S3E_END_C_DECL
// \endcond

#endif /* !S3E_EXT_ANDROIDFULLSCREEN_H */
