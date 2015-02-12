s3eAndroidFullscreen
====================

This is an extension for the Marmalade SDK to support Android's fullscreen and immersive
modes. It supports *fullscreen*, *immersive* and *immersive sticky* modes. Built using
Marmalade 7.3.0 with android.jar copied from SDK level19 (see below).

Updated: 16/06/14

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


Setup for C++ and Quick
-----------------------

Add s3eAndroidFullscreen's parent folder to your modules search path.

Recommended: add your github root to global search. Then all SDK installs
can pick up all your github projects! Put the following in
< marmalade-root >/s3e/s3e-default.mkf:

        options { module_path="path/to/my/github/projects/root" }

Lazy but hard to maintain option: Put whole s3eAndroidFullscreen folder in
< Marmalade >/extensions


To use from C++
---------------

In project MKB add:

    subproject s3eAndroidFullscreen

In game code just call this to enter sticky immersive mode:

    s3eAndroidFullscreenOn();

If user is running 4.0-4.3 Android OS:

    too bad :'(

By default, the screen view hierarchy (inc GL view) will:

- start adjusted to fit the nav bar (not fullscreen)
- resize to fullscreen when the nav bar is hidden
- **not** resize back when the bar is re-shown (still fullscreen, now obscured by nav bar)
- You can (maybe) control this with the staticLayout flag - implemented but not tested!

Sticky immersive mode is used by default when you call s3eAndroidFullscreenOn(). Use
immersive and stickyNavBar flags for other modes.


To rebuild for C++ if you edit extension source
-----------------------------------------------

You need the Android level 19 (4.4) library - android.jar - in order to rebuild the
Java part of the extension.

NB: Marmalade does not use your installed Android SDK (e.g. adt-bundle-etc folder)
to build extensions. Instead, it uses a bundled version that lives at
< Marmalade >/s3e/deploy/plugins/android/android.jar

In Marmalade 7.3, the shipped version is:
 level 15 / 4.0.3 / android.os.Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1

Until the SDK gets updated, you can work around this by doing:

1. Download the latest Android SDK
2. From that, copy: < adt-bundle-folder >/sdk/platforms/android-19/android.jar
3. Paste it over: < Marmalade >/s3e/deploy/plugins/android/android.jar


Marmalade Quick wrapper
-----------------------

The *quick* folder contains C++ wrappers that tolua++ can process in order to generate
Quick Lua bindings. To use the Quick version you'll need to update some of the Quick
installation's files and re-build quick binaries. All paths
below are within the Marmalade sdk folder (or .app on Mac) This also assumes you've
put the extension on global modules path or copied to sdk/extensions.


### Prerequisites

1. Marmalade SDK 7.4 or newer is needed for Quick extension improvemenmts.
   
2. You need scripts for rebuilding Quick binaries. Get these from
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
   

To use the Quick APIs
~~~~~~~~~~~~~~~~~~~~~

**NB:** If updating an existing project, you'll likely need to delete the
project's 'build_temp' folder. This is so that the Hub will regenerate all the
necessary deployment scripts and include the new extension.

- Look in s3eAndroidFullscreen/quick/QAndroidFullscreen.h
- Use namespace.function() to call functions, with true/false for params, e.g.
  androidFullscreen.turnOn(true, false, true)


Issues
------

- Only tested on Nexus 4 running OS 4.4.3 - your mileage may vary
- Binaries are only included for Android ARM extension and for ARM and Mac in Quick.
  You'll need to build for other architectures until I reinstall Windows!
- Sticky mode does NOT re-hide the nav bar every time the OS re-shows it. Currently it
  uses a bit of a hack to re-show in the Marmalade S3E_DEVICE_UNPAUSE event. This will
  usually be adequate. If you are displaying native UI in your own app, you will prob
  need to call s3eAndroidFullscreenOn() again at various points.
- Builds running 4.4.0 -> 4.4.2 may re-show the nav bar on volume key presses You can work
  around by recalling s3eAndroidFullscreenOn in S3E_DEVICE_VOLUMEUP etc. I might add that
  to the extension if it proves true.
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
