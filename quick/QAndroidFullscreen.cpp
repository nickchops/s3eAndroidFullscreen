#include "QAndroidFullscreen.h"
#include "s3eAndroidFullscreen.h"
namespace androidFullscreen {

//------------------------------------------------------------------------------
bool isAvailable()
{
    return s3eAndroidFullscreenAvailable() == S3E_TRUE;
}

bool isImmersiveSupported()
{
    return s3eAndroidFullscreenIsImmersiveSupported() == S3E_TRUE;
}

void turnOn(bool immersive, bool stickyNavBar, bool staticLayout)
{
    s3eAndroidFullscreenOn(immersive ? S3E_TRUE : S3E_FALSE, stickyNavBar ? S3E_TRUE : S3E_FALSE, staticLayout ? S3E_TRUE : S3E_FALSE);
}

void turnOff(bool showStatusBar, bool showNavBar, bool staticLayout)
{
    s3eAndroidFullscreenOff(showStatusBar ? S3E_TRUE : S3E_FALSE, showNavBar ? S3E_TRUE : S3E_FALSE, staticLayout ? S3E_TRUE : S3E_FALSE);
}

} // namespace androidFullscreen
