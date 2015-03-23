// Example code for s3eAndroidFullScreen Marmalade extension by Nick Smith
// http://github.com/nickchops
// ExamplesMain is copied without change from the Marmalade SDK and included
// with this example for convenience (paths are annoying)

#include "ExamplesMain.h"
#include "s3eAndroidFullscreen.h"
#include "s3eDevice.h"
#include "IwDebug.h"
#include "s3eDialog.h"

static Button*     g_ButtonHidebarSticky;
static Button*     g_ButtonHideBarNonSticky;
static Button*     g_ButtonHideBarNonImmersive;
static Button*     g_ButtonShow;
static Button*     g_ButtonShowBars;
static Button*     g_ButtonCheck;

//static Button*     g_LastButton = NULL;

int32 _resumeCallback(void* sys, void* user)
{
    IwTrace(FULLSCREENTEST, ("App resumed, re-setting fullscreen"));
	return 0;
}

int32 _pauseCallback(void* sys, void* user)
{
	IwTrace(FULLSCREENTEST, ("App paused"));
	return 0;
}

void ExampleInit()
{
    g_ButtonHidebarSticky = NewButton("Hide nav bar - sticky mode");
    g_ButtonHideBarNonSticky = NewButton("Hide nav bar - non-sticky");
    g_ButtonHideBarNonImmersive = NewButton("Hide nav bar - non-immersive");
    g_ButtonShow = NewButton("Show nav bar");
    g_ButtonShowBars = NewButton("Show nav & status bars if supported");
	g_ButtonCheck = NewButton("Check available");

	s3eDeviceRegister(S3E_DEVICE_PAUSE, _pauseCallback, NULL);
	s3eDeviceRegister(S3E_DEVICE_UNPAUSE, _resumeCallback, NULL);
}

void ExampleTerm()
{
}

bool ExampleUpdate()
{
    Button* pressed = GetSelectedButton();
    
    if (pressed == g_ButtonHidebarSticky)
    {
        s3eAndroidFullscreenOn();
    }
    
    if (pressed == g_ButtonHideBarNonSticky)
    {
        s3eAndroidFullscreenOn(S3E_TRUE, S3E_FALSE);
    }
    
    if (pressed == g_ButtonHideBarNonImmersive)
    {
        s3eAndroidFullscreenOn(S3E_FALSE);
    }

    if (pressed == g_ButtonShow)
    {
        s3eAndroidFullscreenOff();
    }

    if (pressed == g_ButtonShowBars)
    {
        s3eAndroidFullscreenOff(S3E_TRUE);
    }

	if (pressed == g_ButtonCheck)
    {
        s3eDialogAlertInfo alert;
        alert.m_Title = "Android Fullscreen Support";
        
        if (s3eAndroidFullscreenAvailable())
        {
            if (s3eAndroidFullscreenIsImmersiveSupported())
                alert.m_Message = "fully supported";
            else
                alert.m_Message = "only non-immersive mode is supported";
        }
        else
            alert.m_Message =  "not supported at all";
        
        s3eDialogAlertBox(&alert);
        
        // Tip: try showing the s3eDialog when in fullscreen mode...
        // a good example of a situation where you would need to manually
        // re-call s3eAndroidFullscreenOn()!
    }

    return true;
}

void ExampleRender()
{
}
