#include "QAndroidFullscreen.h"
#include "s3eAndroidFullscreen.h"
namespace androidFullscreen {

//------------------------------------------------------------------------------
bool isAvailable()
{
    return s3eAndroidFullscreenAvailable() == S3E_RESULT_SUCCESS;
}

bool isImmersiveSupported()
{
    return s3eAndroidFullscreenIsImmersiveSupported() == S3E_TRUE;
}

void turnOn(bool immersive=true, bool stickyNavBar=true, bool staticLayout=true)
{
    s3eAndroidFullscreenOn(immersive ? S3E_TRUE : S3E_FALSE, staticLayout ? S3E_TRUE : S3E_FALSE);
}

void turnOff(showStatusBar=false, bool showNavBar=true, bool staticLayout=true)
{
    s3eAndroidFullscreenOff(showNavBar ? S3E_TRUE : S3E_FALSE, showStatusBar ? S3E_TRUE : S3E_FALSE, staticLayout ? S3E_TRUE : S3E_FALSE);
}

} // namespace androidFullscreen
