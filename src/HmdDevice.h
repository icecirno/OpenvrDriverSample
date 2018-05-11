#pragma once
#include "openvr\openvr_driver.h"
#include "driverlog.h"
#include "TrackedDevice.h"
#include "EventReceiver.h"

#include <vector>
#include <thread>
#include <chrono>
#include <math.h>
#include "mathTool.h"
#if defined( _WINDOWS )
#include <windows.h>
#endif

using namespace vr;

class HmdDevice : public TrackedDevice, public vr::IVRDisplayComponent,public EventReceiver
{
public:
	HmdDevice(std::string serialNumber, std::string modelNumber, std::string inputProfilePath);

	~HmdDevice();

	virtual EVRInitError Activate();
	virtual void EnterStandby() {};
	virtual void *GetComponent(const char *pchComponentNameAndVersion);

	virtual void GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight);

	virtual bool IsDisplayOnDesktop();

	virtual bool IsDisplayRealDisplay();

	virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight);

	virtual void GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight);
	
	virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom);

	virtual DistortionCoordinates_t ComputeDistortion(EVREye eEye, float fU, float fV);
	
	virtual void keyEvent(int key, bool isdown);
	virtual void mouseEvent(int x, int y);
	virtual void updatePoseEvent(float deltatime = 0.01);
	virtual void updatePoseEvent(vr::DriverPose_t base, float deltatime = 0.01);

private:

	float a = 0;
	float color = -1.0;
	std::string m_sSerialNumber;
	std::string m_sModelNumber;

	int32_t m_nWindowX;
	int32_t m_nWindowY;
	int32_t m_nWindowWidth;
	int32_t m_nWindowHeight;
	int32_t m_nRenderWidth;
	int32_t m_nRenderHeight;
	float m_flSecondsFromVsyncToPhotons;
	float m_flDisplayFrequency;
	float m_flIPD;
	
	float mouseY=0;
	float mouseX=0;

	bool WDown=0;
	bool SDown=0;
	bool ADown=0;
	bool DDown=0;
};