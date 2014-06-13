/*
 * This file is part of the Marmalade SDK Code Samples.
 *
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This source code is intended only as a supplement to the Marmalade SDK.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#include "ExamplesMain.h"
#include <malloc.h>
#include "IwDebug.h"
#include "IwGx.h"
#include "IwGxPrint.h"
#include "IwTexture.h"
#include "IwMaterial.h"

#define FRAMETIME  30

static void CursorRender();
static void SoftkeysRender();

//Will be set to true if the device has a keyboard or number pad
bool                g_DeviceHasKeyboard = false;
static bool         g_ClearScreen = true;
static bool         g_DrawCursor = true;
bool                g_HideDisabledButtons = false;
static CIwMaterial* g_CursorMaterial = NULL;

// Attempt to lock to 25 frames per second
#define MS_PER_FRAME (1000 / 25)

bool ExamplesMainUpdate()
{
    s3eDeviceYield(0);
    s3eKeyboardUpdate();
    s3ePointerUpdate();

    int64 start = s3eTimerGetMs();

    if (!ExampleUpdate() || ExampleCheckQuit())
    {
        s3eDebugTracePrintf("ExampleUpdate returned false, exiting..");
        return false;
    }

    // Clear the screen
    if (g_ClearScreen)
        IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

    ButtonsRender();

    if (g_DrawCursor)
        CursorRender();

    SoftkeysRender();

    // User code render
    ExampleRender();

    // Attempt frame rate
    while ((s3eTimerGetMs() - start) < MS_PER_FRAME)
    {
        int32 yield = (int32) (MS_PER_FRAME - (s3eTimerGetMs() - start));
        if (yield<0)
            break;
        s3eDeviceYield(yield);
    }

    IwGxFlush();
    IwGxSwapBuffers();

    return true;
}

void ExamplesMainTerm()
{
    TerminateMessages();

    // User code term
    ExampleTerm();

    delete g_CursorMaterial;
    DeleteButtons();

    //TODO: Switch to IwGx font so we can both measure the font size and scale it

    //IwGxFontTerminate();
    //IwResManagerTerminate();
    IwGxTerminate();
}

void ExamplesMainInit()
{
#ifdef EXAMPLE_DEBUG_ONLY
    // Test for Debug only examples
#ifndef IW_DEBUG
    DisplayMessage("This example is designed to run from a Debug build. Please build the example in Debug mode and run it again.");
    return 0;
#endif
#endif

    IwGxInit();

    //IwResManagerInit();
    //IwGxFontInit();

    // Set screen clear colour
    IwGxSetColClear(0xff, 0xff, 0xff, 0xff); // white background
    IwGxPrintSetColour(0, 0, 0);             // black font

    // Determine if the device has a keyboard
    if (s3eKeyboardGetInt(S3E_KEYBOARD_HAS_KEYPAD) || s3eKeyboardGetInt(S3E_KEYBOARD_HAS_ALPHA))
        g_DeviceHasKeyboard = true;

    // User code init
    ExampleInit();
}

bool ExampleCheckQuit()
{
    bool rtn = s3eDeviceCheckQuitRequest()
        || (s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_PRESSED)
        || (s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_PRESSED);
    if (rtn)
        s3eDebugTracePrintf("quiting example");
    return rtn;
}

void DontClearScreen()
{
    g_ClearScreen = false;
}

void DontDrawCursor()
{
    g_DrawCursor = false;
}

bool IsUsingPointerButtons()
{
    return !g_DeviceHasKeyboard;
}

void DrawRect(int x, int y, int width, int height, uint8 r, uint8 g, uint8 b)
{
    int right = x + width;
    int bottom = y + height;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (right > (int32)s3eSurfaceGetInt(S3E_SURFACE_WIDTH))
        right = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
    if (bottom > (int32)s3eSurfaceGetInt(S3E_SURFACE_HEIGHT))
        bottom = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

    // Draw the text
    IwGxSetScreenSpaceSlot(0);

    CIwMaterial *fadeMat = IW_GX_ALLOC_MATERIAL();
    fadeMat->SetAlphaMode(CIwMaterial::NONE);
    fadeMat->SetShadeMode(CIwMaterial::SHADE_FLAT);
    IwGxSetMaterial(fadeMat);

    CIwColour* cols = IW_GX_ALLOC(CIwColour, 4);
    for (int i = 0; i < 4; i++)
        cols[i].Set(r, g, b);

    CIwSVec2 xy(x, y);
    CIwSVec2 wh(width, height);
    IwGxDrawRectScreenSpace(&xy, &wh, cols);
}

static void SoftkeyRender(const char* text, s3eDeviceSoftKeyPosition pos, void(*handler)())
{
    // TODO: Hardocoded font width and height (boo!)
    int width = 7;
    int height = 30;

    width *= strlen(text) * 2;
    int x = 0;
    int y = 0;
    switch (pos)
    {
        case S3E_DEVICE_SOFTKEY_BOTTOM_LEFT:
            y = IwGxGetScreenHeight() - height;
            x = 0;
            break;
        case S3E_DEVICE_SOFTKEY_BOTTOM_RIGHT:
            y = IwGxGetScreenHeight() - height;
            x = IwGxGetScreenWidth() - width;
            break;
        case S3E_DEVICE_SOFTKEY_TOP_RIGHT:
            y = 0;
            x = IwGxGetScreenWidth() - width;
            break;
        case S3E_DEVICE_SOFTKEY_TOP_LEFT:
            x = 0;
            y = 0;
            break;
    }

    CIwMaterial *fadeMat = IW_GX_ALLOC_MATERIAL();
    fadeMat->SetAlphaMode(CIwMaterial::SUB);
    IwGxSetMaterial(fadeMat);

    IwGxPrintString(x + 10, y+10, text, false);

    CIwColour* cols = IW_GX_ALLOC(CIwColour, 4);
    memset(cols, 50, sizeof(CIwColour)*4);

    if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_PRESSED)
    {
        int pointerx = s3ePointerGetX();
        int pointery = s3ePointerGetY();
        if (pointerx >= x && pointerx <= x+width && pointery >=y && pointery <= y+height)
        {
            memset(cols, 15, sizeof(CIwColour)*4);
            handler();
        }
    }

    // Draw button area
    CIwSVec2 XY(x, y-2), dXY(width, height);
    IwGxDrawRectScreenSpace(&XY, &dXY, cols);
}

static void SoftkeysRender()
{
    static bool disableExit =
        s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WS8 ||
        s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WS81;

    if (!disableExit)
    {
        int back = s3eDeviceGetInt(S3E_DEVICE_BACK_SOFTKEY_POSITION);
        SoftkeyRender("`x666666Exit", (s3eDeviceSoftKeyPosition)back, s3eDeviceRequestQuit);
    }
}

void CursorRender()
{
    if (!s3ePointerGetInt(S3E_POINTER_AVAILABLE))
        return;

    if (!g_CursorMaterial)
    {
        g_CursorMaterial = new CIwMaterial();
        g_CursorMaterial->SetColAmbient(0, 0, 255, 255);
    }

    IwGxSetMaterial(g_CursorMaterial);
    int pointerx = s3ePointerGetX();
    int pointery = s3ePointerGetY();

    int cursor_size = 10;
    CIwSVec2 wh(cursor_size*2, 1);
    CIwSVec2 wh2(1, cursor_size*2);
    CIwSVec2 pos = CIwSVec2((int16)pointerx-cursor_size, (int16)pointery);
    CIwSVec2 pos2 = CIwSVec2((int16)pointerx, (int16)pointery-cursor_size);
    IwGxDrawRectScreenSpace(&pos, &wh);
    IwGxDrawRectScreenSpace(&pos2, &wh2);
}

// Helper functions print messages to the screen

static int g_NumMessages = 0;
static int g_MessageLen = 0;
static char** g_Messages = NULL;
static int g_MessageIdx = -1;
static bool g_LoopMessages = false;

void InitMessages(int numMessages, int messageLen)
{
    if (numMessages < 1)
        return;

    g_NumMessages = numMessages;
    g_MessageLen = messageLen;
    g_MessageIdx = -1;

    g_Messages = (char**)malloc(sizeof(char*)*numMessages);

    for (int i = 0; i < numMessages; i++)
    {
        g_Messages[i] = (char*)malloc(sizeof(char)*messageLen);
        g_Messages[i][0] = '\0';
        g_Messages[i][messageLen-1] = '\0'; // Make sure string is null terminated
    }
}

void ClearMessages()
{
    g_MessageIdx = -1;

    for (int i = 0; i < g_NumMessages; i++)
        g_Messages[i][0] = '\0';
}

void TerminateMessages()
{
    if (!g_NumMessages)
        return;

    for (int i = 0; i < g_NumMessages; i++)
        free(g_Messages[i]);

    free(g_Messages);
    g_Messages = NULL;
    g_NumMessages = 0;
}

static void AppendMessageColourV(Colour colour, const char* fmt, va_list args)
{
    if (!g_NumMessages)
        return;

    // Add message to the array in a circlular manner, overwriting older messages when array is full
    g_MessageIdx += 1;

    if (g_MessageIdx == g_NumMessages)
        g_LoopMessages = true;

    g_MessageIdx = g_MessageIdx % g_NumMessages;
    char* message = g_Messages[g_MessageIdx];

    const char* colourStr;
    switch (colour)
    {
        case BLUE:
            colourStr = "`x6666ee";
            break;
        case GREEN:
            colourStr = "`x66ee66";
            break;
        case RED:
            colourStr = "`xee6666";
            break;
        case WHITE:
            colourStr = "`x000000";
            break;
        case BLACK:
        default:
            colourStr = "`x666666";
            break;
    }

    strcpy(message, colourStr);
    vsnprintf(message+8, g_MessageLen-8-1, fmt, args);
    s3eDebugTracePrintf("Append Message: %s", message+8);
}

void AppendMessageColour(Colour colour, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    AppendMessageColourV(colour, fmt, args);
    va_end(args);
}

void AppendMessage(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    AppendMessageColourV(BLACK, fmt, args);
    va_end(args);
}

//TODO: I don't think this will render in GX
int PrintMessages(int x, int y)
{
    if (!g_NumMessages || g_MessageIdx == -1)
        return -1;

    char* currentMsg;

    for (int i=0, j = g_MessageIdx; i < g_NumMessages; i++, j = (j + (g_NumMessages-1)) % g_NumMessages)
    {
        // Avoid overlapping exit button
        if (y > s3eSurfaceGetInt(S3E_SURFACE_HEIGHT) - (s3eDebugGetInt(S3E_DEBUG_FONT_HEIGHT)*2))
            break;

        if (j > g_MessageIdx && !g_LoopMessages)
            continue;

        currentMsg = g_Messages[j];
        if (currentMsg[0])
        {
            s3eDebugPrintf(x, y, 0, "%s", currentMsg);

            if (i != g_NumMessages -1)
                y += s3eDebugGetInt(S3E_DEBUG_FONT_HEIGHT)+2;
        }
    }

    return y;
}
