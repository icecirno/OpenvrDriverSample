#include "ControllerDevice.h"



ControllerDevice::ControllerDevice(std::string serialNumber, std::string modelNumber, std::string inputProfilePath,ETrackedControllerRole role):TrackedDevice(serialNumber, modelNumber,inputProfilePath)
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
};
*/
EVRInitError ControllerDevice::Activate()
{
	DriverLog("ControllerDevice::Activate\n");
	vr::VRProperties()->SetStringProperty(propertyContainer, Prop_ManufacturerName_String,"sample");
	vr::VRProperties()->SetInt32Property(propertyContainer, Prop_ControllerRoleHint_Int32, m_role);
	vr::VRProperties()->SetUint64Property(propertyContainer, Prop_SupportedButtons_Uint64, 28);
	vr::VRProperties()->SetInt32Property(propertyContainer, Prop_Axis0Type_Int32, 1);
	initComponent();
	return EVRInitError::VRInitError_None;
}
void ControllerDevice::EnterStandby()
{
}
void *ControllerDevice::GetComponent(const char *pchComponentNameAndVersion)
{
	return NULL;
}

ControllerDevice::~ControllerDevice()
{
}
void ControllerDevice::initComponent()
{
	vr::VRDriverInput()->CreateBooleanComponent(propertyContainer, "/input/system/click", &m_steam);
	vr::VRDriverInput()->CreateScalarComponent(propertyContainer, "/input/trigger/value", &m_trigger, VRScalarType_Absolute, VRScalarUnits_NormalizedOneSided);
}

void ControllerDevice::mouseEvent(int x, int y)
{
}
void ControllerDevice::updatePoseEvent(float deltatime)
{
	updatePoseEvent(pose, deltatime);
}
void ControllerDevice::updatePoseEvent(vr::DriverPose_t base, float deltatime)
{
	color += 0.014f;
	if (color >= 0.5)
		color = -0.5;
	pose.vecVelocity[1] = 0.014f;
	pose.vecVelocity[0] = 0.0f;
	pose.vecVelocity[2] = 0.0f;
	pose.vecPosition[1] = base.vecPosition[1] + color;
	pose.vecPosition[0] = base.vecPosition[0];
	pose.vecPosition[2] = base.vecPosition[2];
}
void ControllerDevice::keyEvent(int key, bool isdown)
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
