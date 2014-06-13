/*
Generic implementation of the s3eAndroidFullscreen extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "s3eAndroidFullscreen_internal.h"
s3eResult s3eAndroidFullscreenInit()
{
    //Add any generic initialisation code here
    return s3eAndroidFullscreenInit_platform();
}

void s3eAndroidFullscreenTerminate()
{
    //Add any generic termination code here
    s3eAndroidFullscreenTerminate_platform();
}

s3eBool s3eAndroidFullscreenIsImmersiveSupported()
{
	return s3eAndroidFullscreenIsImmersiveSupported_platform();
}

void s3eAndroidFullscreenOn(s3eBool immersive, s3eBool stickyNavBar, s3eBool staticLayout)
{
	s3eAndroidFullscreenOn_platform(immersive, stickyNavBar, staticLayout);
}

void s3eAndroidFullscreenOff(s3eBool showStatusBar, s3eBool showNavBar, s3eBool staticLayout)
{
	s3eAndroidFullscreenOff_platform(showStatusBar, showNavBar, staticLayout);
}
