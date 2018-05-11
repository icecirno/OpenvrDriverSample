#pragma once
#include "openvr\openvr_driver.h"
#include "driverlog.h"
#include "TrackedDevice.h"
#include "EventReceiver.h"
using namespace vr;
class ControllerDevice :public TrackedDevice//,public IVRDriverInput
{
public:
	ControllerDevice(std::string serialNumber, std::string modelNumber, std::string inputProfilePath,ETrackedControllerRole role);
	~ControllerDevice();
	virtual EVRInitError Activate();
	virtual void EnterStandby();
	virtual void *GetComponent(const char *pchComponentNameAndVersion);
	
	virtual void keyEvent(int key, bool isdown);
	virtual void mouseEvent(int x, int y);
	virtual void updatePoseEvent(float deltatime = 0.01);
	virtual void updatePoseEvent(vr::DriverPose_t base, float deltatime = 0.01);
private:
	void initComponent();
	ETrackedControllerRole m_role;

	float color = -1.0f;
	float color2 = -1.0f;
	DriverPose_t m_pose = {0};
	VRInputComponentHandle_t m_trigger = 0;
	VRInputComponentHandle_t m_steam = 0;
	float m_flIPD=0;
};

