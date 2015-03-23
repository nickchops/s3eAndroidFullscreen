/*
java implementation of the s3eAndroidFullscreen extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */

package com.nickchops.s3eAndroidFullscreen;

import com.ideaworks3d.marmalade.LoaderAPI;
import android.os.Build;
import android.view.View;


public class s3eAndroidFullscreen
{
	private static boolean m_sticky = false;
    private static boolean m_wantFullscreen = false;
    private static int m_stickyFlags = 0x0;

	public boolean supported()
    {
    	if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH)
            return true;
        else
            return false;
    }

    public boolean s3eAndroidFullscreenIsImmersiveSupported()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
            return true;
        else
            return false;
    }

    public void s3eAndroidFullscreenOn(boolean immersive, boolean stickyNavBar, boolean staticLayout)
    {
        m_wantFullscreen = true;
        m_sticky = immersive && stickyNavBar;

        int flags = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
            | View.SYSTEM_UI_FLAG_FULLSCREEN; // hide status bar

        // Set the content to appear under the system bars so that the content
        // doesn't resize when the system bars hide and show.
        flags = flags |
            (staticLayout ?
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            : 0x0);

        // Immersive stops the nav bar coming back on user touch. Sticky is same but also makes bars
        // semi transparent and re-hide themselves if user swipes to show them
        flags = flags| (immersive? (m_sticky ? View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY : View.SYSTEM_UI_FLAG_IMMERSIVE) : 0x0);

        final View decorView = LoaderAPI.getActivity().getWindow().getDecorView();
        decorView.setSystemUiVisibility(flags);

        // UI visibility is a global thing that other apps can interfere with.

        // Store flags to re set them in onWindowFocusChanged
        m_stickyFlags = flags;
    }

    public void s3eAndroidFullscreenOff(boolean showStatusBar, boolean showNavBar, boolean staticLayout)
    {
        m_wantFullscreen = false;
        m_sticky = false;
        int flags = 0;

        if (!showNavBar)
            flags = flags | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | (m_sticky ? View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY : View.SYSTEM_UI_FLAG_IMMERSIVE);

        if (!showStatusBar) //use this if the app was showign status bar by default to make sure it comes back
            flags = flags | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;

        if (staticLayout)
            flags = flags | View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;

		final View decorView = LoaderAPI.getActivity().getWindow().getDecorView();
        decorView.setSystemUiVisibility(flags);
    }

    /* TODO?: add ability to manually set all android fullscreen flags
    public void s3eAndroidFullscreenSetFlags(int flags)
    {

    }
    */

    /* TODO?: add function to register UI visibility listener:
       This will allow IwUI or GL code to move things to avoid the nav bat when it comes back
       in non-sticky mode. Or you might want to add a "hide nav bar"/fullscreen button only
       when the bars are re-shown. Will need an s3e callback and an Quick event wrapper for it...
       Jamie reports that Nav bar can come back in situations like volume key presses on some
       platform versions. In this situation, you ought to be using sticky mode anyway!
    
    final View decorView = getWindow().getDecorView();
    decorView.setOnSystemUiVisibilityChangeListener (new View.OnSystemUiVisibilityChangeListener()
    {
        @Override
        public void onSystemUiVisibilityChange(int visibility) {
            // Note that system bars will only be "visible" if none of the
            // LOW_PROFILE, HIDE_NAVIGATION, or FULLSCREEN flags are set.
            if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                // TODO: The system bars are visible. Make any desired
                // adjustments to your UI, such as showing the action bar or
                // other navigational controls.
            } else {
                // TODO: The system bars are NOT visible. Make any desired
                // adjustments to your UI, such as hiding the action bar or
                // other navigational controls.
            }
        }
    });
    */

    //----------------------------------------------------------------------------------
    /*
    This needs to be called from the main activity.
    NB: Use my s3eAndroidUserActivity mechanism to do that. You need to edit
    and rebuild s3eAndroidUserActivity for this to work! 
    */
    public static void onWindowFocusChanged(boolean hasFocus)
    {
        // For sticky mode, set the layout whenever we get focus, in case some
        // other window has changed the flags while out of focus
        if (m_wantFullscreen && m_sticky && hasFocus)
        {
        	LoaderAPI.trace("re-hiding Android nav bar (sticky mode is on)");
            final View decorView = LoaderAPI.getActivity().getWindow().getDecorView();
            decorView.setSystemUiVisibility(m_stickyFlags);
        }
    }
}
