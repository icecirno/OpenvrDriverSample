#pragma once
#include "openvr\openvr_driver.h"
#include "HmdDevice.h"
#include "ControllerDevice.h"

#include "HookDll.h"
using namespace vr;

class ServerDriver : public IServerTrackedDeviceProvider
{
public:
	ServerDriver()
		:  m_bEnableNullDriver(false)
	{
	}

	virtual EVRInitError Init(vr::IVRDriverContext *pDriverContext);
	virtual void Cleanup();
	virtual const char * const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
	virtual void RunFrame();
	virtual bool ShouldBlockStandbyMode() { return true; }
	virtual void EnterStandby() {}
	virtual void LeaveStandby() {}
	void debug(const char* str, int i);
	void debug(const char* str);
	void setHookThread(DWORD threadId);
	void setKeyDown(char key, bool isdown);
	void setMouseMove(int x, int y);
	
	static void updatePostThread();
	static bool isWorking;
	static bool enableMouse;
private:
	DWORD threadId = 0;
	static HmdDevice *m_pNullHmdLatest;
	static ControllerDevice *m_rightHand;
	
	std::thread * m_Thread = NULL;
	bool m_bEnableNullDriver;
};



