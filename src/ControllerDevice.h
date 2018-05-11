#pragma once
#include <openvr_driver.h>
#include "driverlog.h"
using namespace vr;
class ControllerDevice :public ITrackedDeviceServerDriver//,public IVRDriverInput
{
public:
	ControllerDevice(ETrackedControllerRole role);
	~ControllerDevice();
	virtual EVRInitError Activate(uint32_t unObjectId);

	/** This is called when The VR system is switching from this Hmd being the active display
	* to another Hmd being the active display. The driver should clean whatever memory
	* and thread use it can when it is deactivated */
	virtual void Deactivate();

	/** Handles a request from the system to put this device into standby mode. What that means is defined per-device. */
	virtual void EnterStandby();

	/** Requests a component interface of the driver for device-specific functionality. The driver should return NULL
	* if the requested interface or version is not supported. */
	virtual void *GetComponent(const char *pchComponentNameAndVersion);

	/** A VR Client has made this debug request of the driver. The set of valid requests is entirely
	* up to the driver and the client to figure out, as is the format of the response. Responses that
	* exceed the length of the supplied buffer should be truncated and null terminated */
	virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize);

	// ------------------------------------
	// Tracking Methods
	// ------------------------------------
	virtual DriverPose_t GetPose();
	void initComponent();
	void updatePose(DriverPose_t& headPose, float ms = 0.01);
	void setKeyDown(char key, bool isdown);
	void initPose();
	vr::TrackedDeviceIndex_t m_unObjectId=0;
private:
	ETrackedControllerRole m_role;

	float color = -1.0f;
	float color2 = -1.0f;
	DriverPose_t m_pose = {0};
	VRInputComponentHandle_t m_trigger = 0;
	VRInputComponentHandle_t m_steam = 0;
	vr::PropertyContainerHandle_t m_ulPropertyContainer=0;
	float m_flIPD=0;
};

