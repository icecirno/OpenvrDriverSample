#include "HmdDevice.h"

HmdDevice::HmdDevice(std::string serialNumber, std::string modelNumber, std::string inputProfilePath):TrackedDevice(serialNumber, modelNumber, inputProfilePath,vr::TrackedDeviceClass_HMD)
{
	m_flIPD = vr::VRSettings()->GetFloat(k_pch_SteamVR_Section, k_pch_SteamVR_IPD_Float);
	m_nWindowX = vr::VRSettings()->GetInt32("driver_sample", "windowX");
	m_nWindowY = vr::VRSettings()->GetInt32("driver_sample", "windowY");
	m_nWindowWidth = vr::VRSettings()->GetInt32("driver_sample", "windowWidth");
	m_nWindowHeight = vr::VRSettings()->GetInt32("driver_sample", "windowHeight");
	m_nRenderWidth = vr::VRSettings()->GetInt32("driver_sample", "renderWidth");
	m_nRenderHeight = vr::VRSettings()->GetInt32("driver_sample", "renderHeight");
	m_flSecondsFromVsyncToPhotons = vr::VRSettings()->GetFloat("driver_sample", "secondsFromVsyncToPhotons");
	m_flDisplayFrequency = vr::VRSettings()->GetFloat("driver_sample", "displayFrequency");
}

 HmdDevice::~HmdDevice()
{
}

 void HmdDevice::InitPoseEvent()
 {

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
	 pose.vecPosition[1] = 1.0;
 }
 EVRInitError HmdDevice::Activate(TrackedDeviceIndex_t deviceId, PropertyContainerHandle_t propertyContainer)
{
	vr::VRProperties()->SetFloatProperty(propertyContainer, Prop_UserIpdMeters_Float, m_flIPD);
	vr::VRProperties()->SetFloatProperty(propertyContainer, Prop_UserHeadToEyeDepthMeters_Float, 0.f);
	vr::VRProperties()->SetFloatProperty(propertyContainer, Prop_DisplayFrequency_Float, m_flDisplayFrequency);
	vr::VRProperties()->SetFloatProperty(propertyContainer, Prop_SecondsFromVsyncToPhotons_Float, m_flSecondsFromVsyncToPhotons);
	vr::VRProperties()->SetUint64Property(propertyContainer, Prop_CurrentUniverseId_Uint64, 2);
	vr::VRProperties()->SetBoolProperty(propertyContainer, Prop_IsOnDesktop_Bool, false);
	
	return VRInitError_None;
}
void * HmdDevice::GetComponent(const char *pchComponentNameAndVersion)
{
	if (!_stricmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version))
	{
		return (vr::IVRDisplayComponent*)this;
	}
	return NULL;
}


 void HmdDevice::GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight)
{
	*pnX = m_nWindowX;
	*pnY = m_nWindowY;
	*pnWidth = m_nWindowWidth;
	*pnHeight = m_nWindowHeight;
}

 bool HmdDevice::IsDisplayOnDesktop()
{
	return true;
}

 bool HmdDevice::IsDisplayRealDisplay()
{
	return false;
}

 void HmdDevice::GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight)
{
	*pnWidth = m_nRenderWidth*2;
	*pnHeight = m_nRenderHeight;
}

 void HmdDevice::GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight)
{
	*pnY = 0;
	*pnWidth = m_nWindowWidth ;
	*pnHeight = m_nWindowHeight;

	if (eEye == Eye_Left)
	{
		*pnX = 0;
	}
	else
	{
		*pnX = m_nWindowWidth ;
	}
}
 void HmdDevice::GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom)
{
	*pfLeft = -1.777;
	*pfRight = 1.777;
	*pfTop = -1.0;
	*pfBottom = 1.0;
}

 DistortionCoordinates_t HmdDevice::ComputeDistortion(EVREye eEye, float fU, float fV)
{

	DistortionCoordinates_t coordinates;
	coordinates.rfBlue[0] = fU;
	coordinates.rfBlue[1] = fV;
	coordinates.rfGreen[0] = fU;
	coordinates.rfGreen[1] = fV;
	coordinates.rfRed[0] = fU;
	coordinates.rfRed[1] = fV;
	return coordinates;
}

 void HmdDevice::updatePoseEvent(float deltatime)
 {
	 updatePoseEvent(pose, deltatime);
 }
 void HmdDevice::updatePoseEvent(vr::DriverPose_t base, float deltatime)
 {
	 float f = 0;
	 float r = 0;
	 if (WDown)
		 f -= 1.5;
	 if (SDown)
		 f += 1.5;
	 if (ADown)
		 r -= 1.5;
	 if (DDown)
		 r += 1.5;
	 pose.vecPosition[0] += deltatime*( -f*sin(-mouseX / 180.0*3.1415)+r*sin((-mouseX + 90) / 180.0*3.1415) );
	 pose.vecPosition[2] += deltatime*( f*cos(mouseX / 180.0*3.1415)-r*cos((mouseX - 90) / 180.0*3.1415));
	 updatePoseToSteam(pose);
 }
 void HmdDevice::mouseEvent(int x, int y)
 {
	 mouseX -= x / 10.0;
	 mouseY -= y / 10.0;
	 if (mouseY > 90)
		 mouseY = 90;
	 else if (mouseY < -90)
		 mouseY = -90;
	 HmdQuaternion_t A = { 0 }, rkQ = { 0 }, C = { 0 };
	 A.w = cos(mouseX / 360.0*3.1415);
	 A.x = 0.0f;
	 A.y = sin(mouseX / 360.0*3.1415);
	 A.z = 0.0f;
	 rkQ.w = cos(mouseY / 360.0*3.1415);
	 rkQ.x = sin(mouseY / 360.0*3.1415);
	 rkQ.y = 0.0f;
	 rkQ.z = 0.0f;
	 C.w = A.w * rkQ.w - A.x * rkQ.x - A.y * rkQ.y - A.z * rkQ.z;
	 C.x = A.w * rkQ.x + A.x * rkQ.w + A.y * rkQ.z - A.z * rkQ.y;
	 C.y = A.w * rkQ.y + A.y * rkQ.w + A.z * rkQ.x - A.x * rkQ.z;
	 C.z = A.w * rkQ.z + A.z * rkQ.w + A.x * rkQ.y - A.y * rkQ.x;
	 pose.qRotation.w = C.w;
	 pose.qRotation.x = C.x;
	 pose.qRotation.y = C.y;
	 pose.qRotation.z = C.z;
 }
 void  HmdDevice::keyEvent(int key, bool isdown)
 {
	 switch (key)
	 {
	 case 1:
		 WDown = isdown;
		 break;
	 case 2:
		 SDown = isdown;
		 break;
	 case 3:
		 ADown = isdown;
		 break;
	 case 4:
		 DDown = isdown;
		 break;
	 }
 }
