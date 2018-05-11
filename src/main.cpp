//============ Copyright (c) Valve Corporation, All rights reserved. ============

#include "openvr\openvr_driver.h"
#include "driverlog.h"
#include "HmdDevice.h"
#include "ServerDriver.h"
#include "WatchdogDriver_Sample.h"
#include <vector>
#include <thread>
#include <chrono>
#include <math.h>
#include "HookDll.h"
#if defined( _WINDOWS )
#include <windows.h>
#endif

using namespace vr;


#if defined(_WIN32)
#define isKeyDown(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C" 
#else
#error "Unsupported Platform."
#endif



WatchdogDriver_Sample g_watchdogDriverNull;
ServerDriver g_serverDriverNull;
UINT OnHookLowKeyboard(WPARAM wParam)
{
	//map native key code
	switch (wParam)
	{

	case 'W':
	case 'w':return 1;
	case 'S':
	case 's':return 2;
	case 'A':  // Page down
	case 'a':return 3;// Page up
	case 'D':
	case 'd':return 4;
	case 'E':
	case 'e':return 10;
	case 'n':
	case 'N':return 11;
	}
	//44 is prtscn key
	return 0;
}
static void KeyBoardEventThreadFunction()
{
	DWORD thisThreadId = GetCurrentThreadId();
	int e = SetLowKeyboardHook(1, 0, thisThreadId);//hook;
	g_serverDriverNull.debug("[debug]OnHookLowKeyboard OK ,error code:%d\n",e);
	g_serverDriverNull.setHookThread(thisThreadId);
	MSG msg;
	
	while (ServerDriver::isWorking&&GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_KEYINPUT)//get key pressed
		{
			if (msg.lParam == 0)//error
			{
				g_serverDriverNull.debug("get unknow message:%d\n",msg.wParam);
			}
			else if (msg.wParam == VK_ESCAPE)//esc key 
			{
				ServerDriver::enableMouse = 0;
				g_serverDriverNull.debug("[debug]msg.lParam == VK_ESCAPE\n");
			}
			else if (msg.wParam == VK_SPACE)//spacebar key
			{
				ServerDriver::enableMouse = 1;
				g_serverDriverNull.debug("[debug]msg.lParam == VK_SPACE\n");
			}
			else //other key
			{
				int key = OnHookLowKeyboard(msg.wParam);
				if(key)
					g_serverDriverNull.setKeyDown(key,1);
				else
					g_serverDriverNull.debug("get unknow Key:%d\n", msg.wParam);
			}
		}
		else if (msg.message == WM_KEYINPUT_2)//get key released
		{
			g_serverDriverNull.setKeyDown(OnHookLowKeyboard(msg.wParam),0);
		}
		else if (msg.message == WM_CLOSELOOP)//get exit message form ServerDriver.Cleanup();
			break;
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	SetLowKeyboardHook(0, 0, GetCurrentThreadId());//unhook;
}
static void MouseEventThreadFunction()
{
	POINT point;
	POINT screenSize;
	screenSize.x = GetSystemMetrics(SM_CXSCREEN);
	screenSize.y = GetSystemMetrics(SM_CYSCREEN);
	screenSize.x /= 2;
	screenSize.y /= 2;
	g_serverDriverNull.debug("[debug]getScreenSizeX %d\n", screenSize.x);
	g_serverDriverNull.debug("[debug]getScreenSizeY %d\n", screenSize.y);
	while (ServerDriver::isWorking)
	{
		if (ServerDriver::enableMouse)
		{
			GetCursorPos(&point);
			SetCursorPos(screenSize.x, screenSize.y);
			g_serverDriverNull.setMouseMove(point.x - screenSize.x, point.y - screenSize.y);
			Sleep(10);
		}
	}
}
//-----------------------------------------------------------------------------
HMD_DLL_EXPORT void *HmdDriverFactory( const char *pInterfaceName, int *pReturnCode )
{
	if( 0 == strcmp( IServerTrackedDeviceProvider_Version, pInterfaceName ) )
	{
		std::thread * m_Thread = new std::thread(KeyBoardEventThreadFunction);
		std::thread * m_Thread2 = new std::thread(MouseEventThreadFunction);
		return &g_serverDriverNull;
	}
	if( 0 == strcmp( IVRWatchdogProvider_Version, pInterfaceName ) )
	{
		return &g_watchdogDriverNull;
	}

	if( pReturnCode )
		*pReturnCode = VRInitError_Init_InterfaceNotFound;

	return NULL;
}
