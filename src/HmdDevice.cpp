#include "HmdDevice.h"

HmdDevice::HmdDevice()
{
	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
	m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;
	DriverLog("Using settings values\n");
	m_flIPD = vr::VRSettings()->GetFloat(k_pch_SteamVR_Section, k_pch_SteamVR_IPD_Float);

	char buf[1024];
	vr::VRSettings()->GetString(k_pch_Sample_Section, k_pch_Sample_SerialNumber_String, buf, sizeof(buf));
	m_sSerialNumber = buf;

	vr::VRSettings()->GetString(k_pch_Sample_Section, k_pch_Sample_ModelNumber_String, buf, sizeof(buf));
	m_sModelNumber = buf;

	m_nWindowX = vr::VRSettings()->GetInt32(k_pch_Sample_Section, k_pch_Sample_WindowX_Int32);
	m_nWindowY = vr::VRSettings()->GetInt32(k_pch_Sample_Section, k_pch_Sample_WindowY_Int32);
	m_nWindowWidth = vr::VRSettings()->GetInt32(k_pch_Sample_Section, k_pch_Sample_WindowWidth_Int32);
	m_nWindowHeight = vr::VRSettings()->GetInt32(k_pch_Sample_Section, k_pch_Sample_WindowHeight_Int32);
	m_nRenderWidth = vr::VRSettings()->GetInt32(k_pch_Sample_Section, k_pch_Sample_RenderWidth_Int32);
	m_nRenderHeight = vr::VRSettings()->GetInt32(k_pch_Sample_Section, k_pch_Sample_RenderHeight_Int32);
	m_flSecondsFromVsyncToPhotons = vr::VRSettings()->GetFloat(k_pch_Sample_Section, k_pch_Sample_SecondsFromVsyncToPhotons_Float);
	m_flDisplayFrequency = vr::VRSettings()->GetFloat(k_pch_Sample_Section, k_pch_Sample_DisplayFrequency_Float);

	DriverLog("driver_null: Serial Number: %s\n", m_sSerialNumber.c_str());
	DriverLog("driver_null: Model Number: %s\n", m_sModelNumber.c_str());
	DriverLog("driver_null: Window: %d %d %d %d\n", m_nWindowX, m_nWindowY, m_nWindowWidth, m_nWindowHeight);
	DriverLog("driver_null: Render Target: %d %d\n", m_nRenderWidth, m_nRenderHeight);
	DriverLog("driver_null: Seconds from Vsync to Photons: %f\n", m_flSecondsFromVsyncToPhotons);
	DriverLog("driver_null: Display Frequency: %f\n", m_flDisplayFrequency);
	DriverLog("driver_null: IPD: %f\n", m_flIPD);
	initPose();
}

 HmdDevice::~HmdDevice()
{
	DriverLog("HmdDevice%f\n", 123.0f);
}
 void HmdDevice::initPose()
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

 EVRInitError HmdDevice::Activate(vr::TrackedDeviceIndex_t unObjectId)
{
	DriverLog("{Activate%f\n", 123.0f);

	m_unObjectId = unObjectId;
	m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);

	DriverLog(m_sModelNumber.c_str(), 123.0f);
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_ModelNumber_String, m_sModelNumber.c_str());
	DriverLog(m_sModelNumber.c_str(), 123.0f);
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_RenderModelName_String, m_sModelNumber.c_str());
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_InputProfilePath_String, "{sample}/input/vive_profile.json");
	DriverLog("Prop_UserIpdMeters_Float%f\n", m_flIPD);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_UserIpdMeters_Float, m_flIPD);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_UserHeadToEyeDepthMeters_Float, 0.f);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_DisplayFrequency_Float, m_flDisplayFrequency);
	DriverLog("Prop_DisplayFrequency_Float%f\n", m_flDisplayFrequency);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_SecondsFromVsyncToPhotons_Float, m_flSecondsFromVsyncToPhotons);
	DriverLog("Prop_SecondsFromVsyncToPhotons_Float%f\n", m_flSecondsFromVsyncToPhotons);
	vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, Prop_CurrentUniverseId_Uint64, 2);
	DriverLog("Prop_CurrentUniverseId_Uint64%d\n", 2);
	vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, Prop_IsOnDesktop_Bool, false);
	DriverLog("Prop_IsOnDesktop_Bool%d\n", 0);
	bool bSetupIconUsingExternalResourceFile = true;
	if (!bSetupIconUsingExternalResourceFile)
	{

		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceOff_String, "{sample}/icons/headset_sample_status_off.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{sample}/icons/headset_sample_status_searching.gif");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{sample}/icons/headset_sample_status_searching_alert.gif");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReady_String, "{sample}/icons/headset_sample_status_ready.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{sample}/icons/headset_sample_status_ready_alert.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{sample}/icons/headset_sample_status_error.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{sample}/icons/headset_sample_status_standby.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{sample}/icons/headset_sample_status_ready_low.png");
	}
	DriverLog("}Activate%f\n", 123.0f);
	return VRInitError_None;
}

 void HmdDevice::Deactivate()
{
	DriverLog("Deactivate%f\n", 123.0f);
	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
}

 void HmdDevice::EnterStandby()
{
	DriverLog("EnterStandby%f\n", 123.0f);
}

void * HmdDevice::GetComponent(const char *pchComponentNameAndVersion)
{
	DriverLog("GetComponent\n");
	DriverLog(pchComponentNameAndVersion);
	if (!_stricmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version))
	{
		DriverLog("\nGetComponent this\n");
		return (vr::IVRDisplayComponent*)this;
	}
	DriverLog("\nGetComponent null\n");
	// override this to add a component to a driver
	return NULL;
}

 void HmdDevice::PowerOff()
{
	DriverLog("PowerOff%f\n", 123.0f);
}


 void HmdDevice::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
{
	DriverLog("DebugRequest%f\n", 123.0f);
	if (unResponseBufferSize >= 1)
		pchResponseBuffer[0] = 0;
}

 void HmdDevice::GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight)
{
	DriverLog("GetWindowBounds%f\n", 123.0f);
	*pnX = m_nWindowX;
	*pnY = m_nWindowY;
	*pnWidth = m_nWindowWidth;
	*pnHeight = m_nWindowHeight;
}

 bool HmdDevice::IsDisplayOnDesktop()
{
	DriverLog("IsDisplayOnDesktop%f\n", 123.0f);
	return true;
}

 bool HmdDevice::IsDisplayRealDisplay()
{
	DriverLog("IsDisplayRealDisplay%f\n", 123.0f);
	return false;
}

 void HmdDevice::GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight)
{
	DriverLog("GetRecommendedRenderTargetSize%f\n", 123.0f);
	*pnWidth = m_nRenderWidth*2;
	*pnHeight = m_nRenderHeight;
}

 void HmdDevice::GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight)
{
	DriverLog("GetEyeOutputViewport%f\n", 123.0f);
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
	DriverLog("GetProjectionRaw%f\n", 123.0f);
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
 DriverPose_t HmdDevice::GetPose()
 {
	 return m_pose;
}

 void HmdDevice::updatePose(float ms)
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
	 m_pose.vecPosition[0] += ms*( -f*sin(-mouseX / 180.0*3.1415)+r*sin((-mouseX + 90) / 180.0*3.1415) );
	 m_pose.vecPosition[2] += ms*( f*cos(mouseX / 180.0*3.1415)-r*cos((mouseX - 90) / 180.0*3.1415));

 }
 void HmdDevice::setMouseMove(int x, int y)
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
	 m_pose.qRotation.w = C.w;
	 m_pose.qRotation.x = C.x;
	 m_pose.qRotation.y = C.y;
	 m_pose.qRotation.z = C.z;
 }
 void  HmdDevice::setKeyDown(char key, bool isdown)
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

std::string HmdDevice::GetSerialNumber() const { DriverLog("GetSerialNumber%f\n", 123.0f); return m_sSerialNumber; }