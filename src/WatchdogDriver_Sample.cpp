#include "WatchdogDriver_Sample.h"

bool WatchdogDriver_Sample::g_bExiting = false;
EVRInitError WatchdogDriver_Sample::Init(vr::IVRDriverContext *pDriverContext)
{
	VR_INIT_WATCHDOG_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());

	g_bExiting = false;
	DriverLog("[sample]creating watchdog thread\n");


	return VRInitError_None;
}


void WatchdogDriver_Sample::Cleanup()
{
	DriverLog("[sample]Cleanup watchdog thread\n");
	g_bExiting = true;


	CleanupDriverLog();
}

void WatchdogDriver_Sample::WatchdogThreadFunction()
{

#if defined( _WINDOWS )

#else
		// for the other platforms, just send one every five seconds
		std::this_thread::sleep_for(std::chrono::seconds(5));
		vr::VRWatchdogHost()->WatchdogWakeUp();
#endif
	//}
}
