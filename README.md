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


To use
------

Put extension in < Marmalade >/extensions

In MKB add:

    subproject s3eAndroidFullscreen

In game code just call this to enter sticky immersive mode:

    s3eAndroidFullscreenOn();

If user is running 40-4.3 OS:

    too bad :'(

By default, the screen view hierarchy (inc GL view) will:
- start adjusted to fit the nav bar (not fullscreen)
- resize to fullscreen when the nav bar is hidden
- **not** resize back when the bar is re-shown (still fullscreen, now obscured by nav bar)
- You can (maybe) control this with the staticLayout flag - implemented but not tested!

Sticky immersive mode is used by default when you call s3eAndroidFullscreenOn(). Use
immersive and stickyNavBar flags for other modes.


To rebuild
----------

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
installation's files and either re-build quick_prebuilt or use my binaries. All paths
below are within the Marmalade sdk folder (or .app on Mac) This also assumes you've
put the extension in the SDK's *extensions* folder - that's the easiest way to manage
it but make sure you back it all up when updating the SDK!

**To include and build Quick version of the extension**:

1. Leave the wrapper files where they are (dont copy them into sdk/quick/include etc.)
2. To allow tolua++ to generate wrappers, add the following to quick/quickuser_tolua.pkg:

        $cfile "../extensions/s3eAndroidFullscreen/quick/QAndroidFullscreen.h"

3. Run < Marmalade >/quick/quickuser_tolua(.bat) to generate lua->C++ bindings
4. To allow the extension to be built into Quick's binaries, add the following to the end
   of quickuser.mkf:

        #http://github.com/nickchops/s3eAndroidFullscreen
        files
        {
            ("$MARMALADE_ROOT/extensions/s3eAndroidFullscreen/quick")
            QAndroidFullscreen.h
            QAndroidFullscreen.cpp
        }
        subprojects
        {
            s3eAndroidFullscreen
        }

5. Re-build quick_prebuilt.mkb for all architectures to build the bindings and C++
   extension auto-generated parts into the binaries (quick/target/arm/quick_prebuilt.s3e
   etc.)
6. Make the C++/Java libs get included in Android deployments by adding the following to
   quick/quick_prebuilt.mkf:

        subproject "$MARMALADE_ROOT/extensions/s3eAndroidFullscreen/s3eAndroidFullscreen_libs"
7. If updating an existing project, you'll likely need to delete the project's 'build_temp' folder. This is so that the Hub will regenerate all the necessary deployment scripts and include the new fullscreen extension.

**NB:**

If running marmalade 7.3 or older, you might find steps 3 & 5 not so trivial!

For step (3), if you've never used tolua++ before you'll need to set it up:

- On Mac:
 1. Extract modules/third_party/cocos2dx/tools/tolua++/tolua++.Mac.zip (you'll need to do
    "Show Package Contents" on Applications/Marmalade.app)
 2. Copy tolua.++ to < Marmalade >/quick/tools
 3. Edit quickuser_tolua and make sure it points to tools/tolua++
- On PC:
 1. create a new text file called quickuser_tolua.bat in the root quick folder
 2. add the following to the file:
    tools\tolua++ -o "quickuser_tolua.cpp" "quickuser_tolua.pkg"

For step (5), in order to build quick binaries the easiest option is:

- Open the MKB with Xcode/Visual Studio
- Build for ARM and x86 debug and release
- This will output both the ARM and x86 versions to quick/target/win or quick/target/osx..
  but you want the ARM ones to be in the arm folder!
- So, manually move the ARM builds (ending in .s3e) from win or osx into quick/target/arm
- You'll need to use a PC to build Windows, Windows Phone and Android x86/MIPS targets
- You'll need to use a Mac to build OSX

I've supplied builds of the Mac x86 and cross-platform ARM Quick binaries in
quick/target.zip No Windows ones yet as my Windows install is bodged atm. There's an odd
bug with these binaries that displaying any label nodes (director:createLabel) without
explicitly setting a font will cause the Simulator to crash! Note that I'm explicitly
setting font="fonts/Default.fnt" in the example. Weird.

**To use the Quick APIs**:

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
