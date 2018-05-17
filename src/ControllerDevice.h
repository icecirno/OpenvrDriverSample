#pragma once
#include "openvr\openvr_driver.h"
#include "driverlog.h"
#include "TrackedDevice.h"
#include "EventReceiver.h"
using namespace vr;
class ControllerDevice :public TrackedDevice ,public EventReceiver
{
public:
	ControllerDevice(std::string serialNumber, std::string modelNumber, std::string inputProfilePath,ETrackedControllerRole role);
	ControllerDevice::~ControllerDevice(){}
	//TrackedDevice
	virtual EVRInitError Activate(TrackedDeviceIndex_t deviceId, PropertyContainerHandle_t propertyContainer);
	virtual DriverPose_t getPose() { return pose; };
	void *GetComponent(const char *pchComponentNameAndVersion){return NULL;}
	//EventReceiver
	virtual void keyEvent(int key, bool isdown);
	virtual void mouseEvent(int x, int y);
	virtual void updatePoseEvent(float deltatime = 0.01);
	virtual void updatePoseEvent(vr::DriverPose_t base, float deltatime = 0.01);
	virtual void InitPoseEvent();
private:
	void InitInputComponent();
	float color = -1.0f;
	float color2 = -1.0f;
	DriverPose_t pose = {0};
	ETrackedControllerRole m_role;
	VRInputComponentHandle_t m_trigger = 0;
	VRInputComponentHandle_t m_steam = 0;

	float m_flIPD=0;
};

