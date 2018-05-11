#include "ControllerDevice.h"



ControllerDevice::ControllerDevice(ETrackedControllerRole role)
{
	m_role = role;
}
/*
enum EVRButtonId
{
	k_EButton_System = 0,
	k_EButton_ApplicationMenu = 1,
	k_EButton_Grip = 2,
	k_EButton_DPad_Left = 3,
	k_EButton_DPad_Up = 4,
	k_EButton_DPad_Right = 5,
	k_EButton_DPad_Down = 6,
	k_EButton_A = 7,

	k_EButton_ProximitySensor = 31,

	k_EButton_Axis0 = 32,
	k_EButton_Axis1 = 33,
	k_EButton_Axis2 = 34,
	k_EButton_Axis3 = 35,
	k_EButton_Axis4 = 36,

	// aliases for well known controllers
	k_EButton_SteamVR_Touchpad = k_EButton_Axis0,
	k_EButton_SteamVR_Trigger = k_EButton_Axis1,

	k_EButton_Dashboard_Back = k_EButton_Grip,

	k_EButton_Max = 64
};*/
EVRInitError ControllerDevice::Activate(uint32_t unObjectId)
{
	DriverLog("ControllerDevice::Activate\n");
	m_unObjectId = unObjectId;
	m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_ModelNumber_String, "mycontroller");
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_RenderModelName_String, "vr_tracker_vive_1_0");
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_InputProfilePath_String, "{sample}/input/vive_controller_profile.json");
	VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_ManufacturerName_String, "sample");
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_UserIpdMeters_Float, 0.5);
	vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, Prop_CurrentUniverseId_Uint64, 3);
	vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, Prop_ControllerRoleHint_Int32, m_role);
	vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, Prop_SupportedButtons_Uint64, 28);
	vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, Prop_Axis0Type_Int32, 1);
	initComponent();
	initPose();
	return EVRInitError::VRInitError_None;
}
void ControllerDevice::Deactivate()
{
	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
}
void ControllerDevice::EnterStandby()
{
}
void *ControllerDevice::GetComponent(const char *pchComponentNameAndVersion)
{
	DriverLog("ControllerDevice::GetComponent::%s\n", pchComponentNameAndVersion);
	return NULL;
}
void ControllerDevice::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
{
}
DriverPose_t ControllerDevice::GetPose()
{
	return m_pose;
}

ControllerDevice::~ControllerDevice()
{
}
void ControllerDevice::initComponent()
{
	DriverLog("ControllerDevice::initComponent:%d\n", vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/system/click", &m_steam));
	DriverLog("ControllerDevice::initComponent:%d\n", vr::VRDriverInput()->CreateScalarComponent(m_ulPropertyContainer, "/input/trigger/value", &m_trigger, VRScalarType_Absolute, VRScalarUnits_NormalizedOneSided));
}
void ControllerDevice::updatePose(DriverPose_t& headPose, float ms = 0.01)
{
	color += 0.014f;
	if (color >= 0.5)
		color = -0.5;
	m_pose.vecPosition[1] = headPose.vecPosition[1] + color;
	m_pose.vecPosition[0] = headPose.vecPosition[0];
	m_pose.vecPosition[2] = headPose.vecPosition[2];
}
void ControllerDevice::setKeyDown(char key, bool isdown)
{
	switch (key)
	{
	case 10:
		DriverLog("ControllerDevice::setKeyDown:1:%d\n", isdown);
		vr::VRDriverInput()->UpdateScalarComponent(m_trigger, 1 - isdown, 0.014);

	case 11:
		DriverLog("ControllerDevice::setKeyDown:10:%d\n", isdown);
		vr::VRDriverInput()->UpdateBooleanComponent(m_steam, isdown, 0.014);

	}
}
void ControllerDevice::initPose()
{
	m_pose = { 0 };
	m_pose.poseIsValid = true;
	m_pose.result = TrackingResult_Running_OK;
	m_pose.deviceIsConnected = true;

	m_pose.vecPosition[0] = 0.0f;
	m_pose.vecPosition[1] = 1.0f;
	m_pose.vecPosition[0] = 0.0f;

	m_pose.qWorldFromDriverRotation.w = 1.0f;
	m_pose.qWorldFromDriverRotation.x = 0.0f;
	m_pose.qWorldFromDriverRotation.y = 0.0f;
	m_pose.qWorldFromDriverRotation.z = 0.0f;
	m_pose.qDriverFromHeadRotation.w = 1.0f;
	m_pose.qDriverFromHeadRotation.x = 0.0f;
	m_pose.qDriverFromHeadRotation.y = 0.0f;
	m_pose.qDriverFromHeadRotation.z = 0.0f;
}