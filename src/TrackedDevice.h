#pragma once
#include "openvr\openvr_driver.h"
using namespace vr;
class TrackedDevice :public ITrackedDeviceServerDriver
{
/*enum ETrackedDeviceClass
{
	TrackedDeviceClass_Invalid = 0,				// the ID was not valid.
	TrackedDeviceClass_HMD = 1,					// Head-Mounted Displays
	TrackedDeviceClass_Controller = 2,			// Tracked controllers
	TrackedDeviceClass_GenericTracker = 3,		// Generic trackers, similar to controllers
	TrackedDeviceClass_TrackingReference = 4,	// Camera and base stations that serve as tracking reference points
	TrackedDeviceClass_DisplayRedirect = 5,		// Accessories that aren't necessarily tracked themselves, but may redirect video output from other tracked devices
};
*/
public:
	TrackedDevice(std::string serialNumber, std::string modelNumber, std::string inputProfilePath, vr::ETrackedDeviceClass DeviceType)
	{
		this->serialNumber = serialNumber;
		this->modelNumber = modelNumber;
		this->inputProfilePath = inputProfilePath;
		DriverPose_t pose = { 0 };
		TrackedDeviceIndex_t deviceId = k_unTrackedDeviceIndexInvalid;
		PropertyContainerHandle_t propertyContainer = vr::k_ulInvalidPropertyContainer;
		vr::VRServerDriverHost()->TrackedDeviceAdded(serialNumber.c_str(),DeviceType, this);
	}
	~TrackedDevice(){}
	virtual EVRInitError Activate(uint32_t unObjectId)
	{
		deviceId = unObjectId;
		propertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(unObjectId);
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_SerialNumber_String, serialNumber.c_str());
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_ModelNumber_String, modelNumber.c_str());
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_RenderModelName_String, modelNumber.c_str());
		vr::VRProperties()->SetStringProperty(propertyContainer, Prop_InputProfilePath_String, inputProfilePath.c_str());
		DriverLog("ControllerDevice::Activate\n");
		return Activate(unObjectId, propertyContainer);
	}
	virtual void Deactivate() { deviceId = k_unTrackedDeviceIndexInvalid; };
	virtual void EnterStandby() {};
	virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize) {}
	virtual DriverPose_t GetPose() { return getPose(); };
	
	
	virtual EVRInitError Activate(TrackedDeviceIndex_t deviceId, PropertyContainerHandle_t propertyContainer) = 0;
	virtual void *GetComponent(const char *pchComponentNameAndVersion)=0;
	TrackedDeviceIndex_t getId(){return deviceId;}
protected:
	virtual DriverPose_t getPose() = 0;
	PropertyContainerHandle_t getPropertyContainer(){return propertyContainer;}
	void updatePoseToSteam(DriverPose_t pose){vr::VRServerDriverHost()->TrackedDevicePoseUpdated(deviceId, pose, sizeof(DriverPose_t));}
private:

	TrackedDeviceIndex_t deviceId = k_unTrackedDeviceIndexInvalid;
	PropertyContainerHandle_t propertyContainer = vr::k_ulInvalidPropertyContainer;
	std::string inputProfilePath;
	std::string serialNumber;
	std::string modelNumber;
};

