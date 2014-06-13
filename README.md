s3eAndroidFullscreen
====================

This is an extension for the Marmalade SDK to support Android's fullscreen and immersive
modes. It supports *fullscreen*, *immersive* and *immersive sticky* modes. Built using
Marmalade 7.2.0 with android.jar copied from SDK level19 (see below).


What is fullscreen/immersive mode?
----------------------------------

"Vanilla" Android OS builds running 4.0 (Ice Cream Sandwich) display an on-screen touch
navigation bar (back/home/task) make. If users have to do touch/gestures near the
screen edge to play your game then it is too easy to accidentally hit one of those buttons
and make your game unplayable!

Versions 4.0+ have a "fullscreen" API which lets an app hide the nav bar, but it gets
re-displayed as soon as the user touches the screen. Useful for books, useless for games.

In version 4.4 (KitKat) Android added "immersive" mode, which lets the app hide the
nav bar and keep it hidden. Users can then swipe onto the screen to re-show the bar.
"sticky" mode can be used to make the bar auto-hide itself.

Android docs: https://developer.android.com/training/system-ui/immersive.html


To use
------

Put extension in <Marmalade>/extensions

In MKB add:
	subproject s3eAndroidFullscreen

In game code just call this to enter sticky immersive mode:
	s3eAndroidFullscreenOn();

If user is running 40-4.3 OS:
	too bad :'(

By default, the screen view hierarchy (inc GL view) will:
- start adjusted to fit the nav bar (not fullscreen)
- resize to fullscreen when the nav bar is hidden
- *not* resize back when the bar is re-shown (still fullscreen, now obscured by nav bar)
- You can (maybe) control this with the staticLayout flag - implemented but not tested!

Sticky immersive mode is on by default. Use immersive and stickyNavBar flags for other
modes.


To rebuild
----------

You needs the Android level 19 (4.4) library - android.jar - in order to rebuild the
Java part of the extension.

NB: Marmalade does not use your installed the Android SDK (e.g. adt-bundle-etc folder)
to build extensions. Instead, it uses a bundled version that lives at
<Marmalade>/s3e/deploy/plugins/android/android.jar

In Marmalade 7.3, the shipped version is:
 level 15 / 4.0.3 / android.os.Build.VERSION_CODESICE_CREAM_SANDWICH_MR1

Until the SDK gets updated, you can work around this by:
1) Get a copy of the Android SDK
2) From that, copy: <adt-bundle-folder>/sdk/platforms/android-19/android.jar
3) Paste it over: <Marmalade>/s3e/deploy/plugins/android/android.jar


Marmalade Quick wrapper
-----------------------

The quick folder contains C++ wrappers that tolua++ should be able to process to generate
Quick Lua bindings. Not yet tested. You should need to do:
1) Leave the wrapper files where they are (dont copy into quick/include etc.)
2) Add the relative path to QAndroidFullscreen.h to quickuser_tolua.pkg
3) In quickuser.mkf, add:
 - QAndroidFullscreen.h and QAndroidFullscreen.cpp to the files section
 - relative/path/to/s3eAndroidFullscreen to the subprojects section
4) Run quickuser_tolua.bat
5) Re-build quick_prebuilt.mkb for all architectures needed
6) Let me know if it doesn't work!


Issues
------

- Only tested on Nexus 4 running OS 4.4.3 - your mileage may vary
- Sticky mode does NOT re-hide the nav bar every time the OS re-shows it. Currently it
  uses a bit of a hack to re-show in the Marmalade S3E_DEVICE_UNPAUSE event. This will
  usually be adequate. If you are displaying native UI in your own app, you will prob
  need to call s3eAndroidFullscreenOn() again at various points.
- Older builds of 4.4 may re-show the nav bar on volume key presses You can work around by
  recalling s3eAndroidFullscreenOn in S3E_DEVICE_VOLUMEUP etc. I might add that to the
  extension if it proves true.
- There are params to control re-showing the status bar (if you use the MKB option to
  show that) and to control view resizing but I've not tested them!

------------------------------------------------------------------------------------------
(C) 2013 Nick Smith.

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
