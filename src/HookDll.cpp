/****************************** Module Header ******************************\
* Module Name:	HookDll.cpp
* Project:		CppHookDll
* Copyright (c) Microsoft Corporation.
* 
* Defines the exported hook procedure.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 5/01/2009 11:04 PM RongChun Zhang Created
\***************************************************************************/


#include <windows.h>

#define HOOKDLL_EXPORTS
#include "HookDll.h"


// Shared data among all instances.
#pragma data_seg(".HOOKDATA")
HWND g_hWnd = NULL;	 
DWORD caller=NULL;
HHOOK g_hHook = NULL;		// Hook handle
HHOOK g_hHook_2 = NULL;
// Get module from address
HMODULE WINAPI ModuleFromAddress(PVOID pv) 
{
	MEMORY_BASIC_INFORMATION mbi;
	if (::VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
	{
		return (HMODULE)mbi.AllocationBase;
	}
	else
	{
		return NULL;
	}
}

// Hook callback
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || nCode == HC_NOREMOVE)
		return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
	
    if (lParam & 0x40000000)	// Check the previous key state
	{
		return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
	}
	PostThreadMessage(caller, WM_KEYINPUT, wParam, lParam);
	// Post private messages to Main window
	// wParam specifies the virtual key code
	// lParam specifies the key data
    //::PostMessage(g_hWnd, WM_KEYSTROKE, wParam, lParam);
	
    return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// Install or uninstall the hook
int WINAPI SetKeyboardHook(BOOL bInstall, DWORD dwThreadId, DWORD dwordCaller)
{
	int bOk=0;
	caller = dwordCaller;
	
	if (bInstall)
	{
		g_hHook = ::SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, 
			ModuleFromAddress(KeyboardHookProc), dwThreadId);
		if (g_hHook == NULL)
			bOk = GetLastError();
	}
	else 
	{
		bOk = ::UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
	return bOk;
}


// Hook callback
LRESULT CALLBACK LowKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	int bOK = 0;
    if (nCode < 0 || nCode == HC_NOREMOVE)
		bOK = 1;
	
    //if (lParam & 0x40000000)	// Check the previous key state
	//{
		//bOK = 2;
	//}

	KBDLLHOOKSTRUCT  *pkbhs = (KBDLLHOOKSTRUCT *)lParam;

	//check that the message is from keyboard or is synthesized by SendInput API
	if((pkbhs->flags & LLKHF_INJECTED))
		bOK = 3;

	if(wParam == WM_KEYDOWN&&bOK==0)
	{

		PostThreadMessage(caller, WM_KEYINPUT, pkbhs->vkCode, pkbhs->scanCode);
	}
	else if (wParam == WM_KEYUP&&bOK == 0)
	{
		PostThreadMessage(caller, WM_KEYINPUT_2, pkbhs->vkCode, pkbhs->scanCode);
	}
	if(bOK)
		PostThreadMessage(caller, WM_KEYINPUT, bOK, 0);
	return 1;// ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}
int WINAPI SetLowKeyboardHook(BOOL bInstall, DWORD dwThreadId, DWORD dwordCaller)
{
	int bOk = 0;
	caller = dwordCaller;
	if (bInstall)
	{
		g_hHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, LowKeyboardHookProc,
			ModuleFromAddress(LowKeyboardHookProc), dwThreadId);
		if (g_hHook == NULL)
		{
			bOk = GetLastError();
			bOk |= 8;
		}
	}
	else
	{
		bOk = ::UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}

	return bOk;
}

LRESULT CALLBACK LowMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (caller != NULL&&nCode == HC_ACTION)
	{
		PostThreadMessage(caller, WM_MOUSE, wParam, lParam);
	}
	return CallNextHookEx(g_hHook_2, nCode, wParam, lParam);
}
int WINAPI SetMouseHook(BOOL bInstall, DWORD dwThreadId, DWORD dwordCaller)
{
	int bOk = 0;
	caller = dwordCaller;
	if (bInstall)
	{
		g_hHook_2 = ::SetWindowsHookEx(WH_MOUSE_LL, LowMouseHookProc,
			ModuleFromAddress(LowMouseHookProc), dwThreadId);
		if (g_hHook_2 == NULL)
		{
			bOk = GetLastError();
			bOk |= 8;
		}
	}
	else
	{
		bOk = ::UnhookWindowsHookEx(g_hHook_2);
		g_hHook_2 = NULL;
	}

	return bOk;
}
