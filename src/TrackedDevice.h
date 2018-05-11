#pragma once
#include "openvr\openvr_driver.h"
using namespace vr;
class TrackedDevice :public ITrackedDeviceServerDriver
{
public:
	TrackedDevice(std::string serialNumber, std::string modelNumber, std::string inputProfilePath)
	{
		this->serialNumber = serialNumber;
		this->modelNumber = modelNumber;
		this->inputProfilePath = inputProfilePath;
		deviceId = vr::k_unTrackedDeviceIndexInvalid;
		propertyContainer = vr::k_ulInvalidPropertyContainer;
	}
	~TrackedDevice()
	{
	}
	virtual EVRInitError Activate(uint32_t unObjectId)
	{
		deviceId = unObjectId;
		propertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(deviceId);
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_SerialNumber_String, serialNumber.c_str());
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_ModelNumber_String, modelNumber.c_str());
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_RenderModelName_String, modelNumber.c_str());
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_InputProfilePath_String, inputProfilePath.c_str());
		pose = { 0 };
		pose.poseIsValid = true;
		pose.result = TrackingResult_Running_OK;
		pose.deviceIsConnected = true;

		pose.qWorldFromDriverRotation.w = 1.0f;
		pose.qWorldFromDriverRotation.x = 0.0f;
		pose.qWorldFromDriverRotation.y = 0.0f;
		pose.qWorldFromDriverRotation.z = 0.0f;
		pose.qDriverFromHeadRotation.w = 1.0f;
		pose.qDriverFromHeadRotation.x = 0.0f;
		pose.qDriverFromHeadRotation.y = 0.0f;
		pose.qDriverFromHeadRotation.z = 0.0f;
		return Activate();
	}
	virtual EVRInitError Activate() = 0;
	virtual void Deactivate()
	{
		deviceId = vr::k_unTrackedDeviceIndexInvalid;
	}
	virtual void EnterStandby()=0;
	virtual void *GetComponent(const char *pchComponentNameAndVersion)=0;
	virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize) {}
	virtual DriverPose_t GetPose() {return pose;};
	vr::TrackedDeviceIndex_t getDeviceId() {return deviceId;}
protected:
	std::string inputProfilePath;
	vr::TrackedDeviceIndex_t deviceId;
	vr::PropertyContainerHandle_t propertyContainer;
	std::string serialNumber;
	std::string modelNumber;
	DriverPose_t pose;
};

