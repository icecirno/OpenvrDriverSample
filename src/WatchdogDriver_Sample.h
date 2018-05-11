#pragma once

#include <openvr_driver.h>
#include "driverlog.h"

#include <thread>
#include <chrono>
#include <Windows.h>

using namespace vr;
class WatchdogDriver_Sample : public IVRWatchdogProvider
{
public:
	WatchdogDriver_Sample()
	{
		m_pWatchdogThread = nullptr;
	}

	virtual EVRInitError Init(vr::IVRDriverContext *pDriverContext);
	virtual void Cleanup();
	static void WatchdogThreadFunction();
private:
	static bool g_bExiting;
	std::thread *m_pWatchdogThread;
};
