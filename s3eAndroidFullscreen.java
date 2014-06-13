/*
java implementation of the s3eAndroidFullscreen extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import com.ideaworks3d.marmalade.LoaderAPI;
// Comment in the following line if you want to use ResourceUtility
// import com.ideaworks3d.marmalade.ResourceUtility;

import android.os.Build;
import android.view.View;

class s3eAndroidFullscreen
{
    public boolean s3eAndroidFullscreenIsImmersiveSupported()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
            return true;
        else
            return false;
    }
    public void s3eAndroidFullscreenUseStickyNavBar(boolean on)
    {
        
    }
    public void s3eAndroidFullscreenOn(boolean immersive, boolean fullScreenUI)
    {
        /*
        final View decorView = LoaderAPI.getActivity().getWindow().getDecorView();
        
        //    decorView.setOnSystemUiVisibilityChangeListener (new View.OnSystemUiVisibilityChangeListener()
        //    {
        //        @Override
        //        public void onSystemUiVisibilityChange(....)
        //
        //    }
            
        // Set the IMMERSIVE flag.
        // Set the content to appear under the system bars so that the content
        // doesn't resize when the system bars hide and show.
        decorView.setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
            | View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
            | View.SYSTEM_UI_FLAG_IMMERSIVE);
        */
    }
    public void s3eAndroidFullscreenOff(boolean navBar, boolean statusBar, boolean fullScreenUI)
    {
        final View decorView = LoaderAPI.getActivity().getWindow().getDecorView();
        decorView.setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }
    public int s3eAndroidFullscreenSetFlags(int flags)
    {
        return 0;
    }
}
