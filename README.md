s3eAndroidFullscreen
====================

This is an extension for the Marmalade SDK to support Android's fullscreen and immersive
modes. It supports *fullscreen*, *immersive* and *immersive sticky* modes. Built using
Marmalade 7.6.0.

Updated 23/03/15 to extend android activity and fix issues with Android 5.0


What is fullscreen/immersive mode?
----------------------------------

"Vanilla" Android OS builds running 4.0 (Ice Cream Sandwich) display an on-screen touch
navigation bar (back/home/task) make. If users have to do touch/gestures near the
screen edge to play your game then it is too easy to accidentally hit one of those buttons
and that can make your game unplayable!

Versions 4.0+ have a "fullscreen" API which lets an app hide the nav bar, but it gets
re-displayed as soon as the user touches the screen. Useful for books, useless for games.

In version 4.4 (KitKat) Android added "immersive" mode, which lets the app hide the
nav bar and keep it hidden. Users can then swipe onto the screen to re-show the bar.
"sticky" mode can be used to make the bar auto-hide itself.

Android docs: http://developer.android.com/training/system-ui/immersive.html


Setup for C++ or Quick
----------------------

**You need Marmalade 7.6 or newer** since this extension needs the Android level 19
(4.4) library (android.jar) or newer. For older Marmalade versions, see
historical versions of this readme.

**Add s3eAndroidFullscreen's parent folder to your modules search path**.
Recommended way to do this: add your github root to global search. Then all SDK
installs can pick up all your github projects. Put the following in
< marmalade-root >/s3e/s3e-default.mkf:

        options { module_path="path/to/my/github/projects/root" }

**This extension requires you to extend the loader Activity**. To do that:

1) Grab www.github.com/nickchops/s3eAndroidUserActivity

2) In that extension, edit source/android/s3eAndroidUserActivity.java:

   - Above the class definition, add this:
   
        import com.nickchops.s3eAndroidFullscreen.s3eAndroidFullscreen;

   - In the class, add/edit so that it includes:
   
        @Override
        public void onWindowFocusChanged(boolean hasFocus)
        {
            super.onWindowFocusChanged(hasFocus);
        
            s3eAndroidFullscreen.onWindowFocusChanged(hasFocus);
        }

3) Edit s3eAndroidUserActivity_android_java.mkb to include:

        librarypath "../s3eAndroidFullscreen/lib/android/s3eAndroidFullscreeen.jar"
        
4) Rebuild that by running s3eAndroidUserActivity_android_java.mkb
    (no additional tools, or Android SDK needed to build that)


Extra setup for Marmalade Quick
-------------------------------

To use the Quick version you'll need to update some of the Quick installation's
files and re-build quick binaries. All paths below are within the Marmalade sdk
folder (or .app on Mac).

### Prerequisites
   
You need scripts for rebuilding Quick binaries. Get these from
https://github.com/nickchops/MarmaladeQuickRebuildScripts Copy those to the
root *quick* folder in the SDK.
   
### Setup: Add and build this wrapper into Quick

1. Edit quick/quickuser_tolua.pkg and add this new line:

        $cfile "path/to/you/copy/of/s3eAndroidFullscreen/quick/QAndroidFullscreen.h"

2. In quickuser.mkf, also add the following to the 'subprojects' block:

        subprojects
        {
            s3eAndroidFullscreen/quick/QAndroidFullscreen
        }
        
   This allows C++ parts of the actual extension plus the wrapper to be built
    into Quick's binaries.
   
3. Run quick/quickuser_tolua.bat to generate Lua bindings.

4. Rebuild the Quick binaries by running the scripts (build_quick_prebuilt.bat
   etc.)


To use from C++ or Quick
------------------------

In project MKB add:

    subproject s3eAndroidFullscreen
    
Clean/delete build/temp folders.
    
In game code just call this to enter sticky immersive mode:

    s3eAndroidFullscreenOn();   //C++
    
    androidFullscreen.turnOn(); --Quick

If user is running 4.0-4.3 Android OS:

    too bad :'(

By default, the screen view hierarchy (inc GL view) will:

- start adjusted to fit the nav bar (not fullscreen)
- resize to fullscreen when the nav bar is hidden
- **not** resize back when the bar is re-shown (still fullscreen, now obscured by nav bar)
- You can (maybe) control this with the staticLayout flag - implemented but not tested!

Sticky immersive mode is used by default when you call s3eAndroidFullscreenOn(). Use
immersive and stickyNavBar flags for other modes.


Issues
------

- There are params to control re-showing the status bar. You would need to edit the loader
  or manifest to allow status bar for those to work (Marmalade doesn't support that out
  of the box). Untested!

------------------------------------------------------------------------------------------
(C) 2014 Nick Smith.

All code is provided under the MIT license unless stated otherwise:

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
