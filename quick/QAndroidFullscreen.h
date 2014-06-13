#ifndef __Q_ANDROIDFULLSCREEN_H
#define __Q_ANDROIDFULLSCREEN_H

#include <string>

// tolua_begin

namespace androidFullscreen {
bool isAvailable();
bool isImmersiveSupported();
void turnOn(bool immersive=true, bool stickyNavBar=true, bool staticLayout=true);
void turnOff(bool showStatusBar=false, bool showNavBar=true, bool staticLayout=true);
}

// tolua_end
#endif // __Q_ANDROIDFULLSCREEN_H
