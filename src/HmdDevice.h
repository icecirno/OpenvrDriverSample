#pragma once
#include <openvr_driver.h>
#include "driverlog.h"

#include <vector>
#include <thread>
#include <chrono>
#include <math.h>
#include "mathTool.h"
#if defined( _WINDOWS )
#include <windows.h>
#endif

using namespace vr;
static const char * const k_pch_Sample_Section = "driver_sample";
static const char * const k_pch_Sample_SerialNumber_String = "serialNumber";
static const char * const k_pch_Sample_ModelNumber_String = "modelNumber";
static const char * const k_pch_Sample_WindowX_Int32 = "windowX";
static const char * const k_pch_Sample_WindowY_Int32 = "windowY";
static const char * const k_pch_Sample_WindowWidth_Int32 = "windowWidth";
static const char * const k_pch_Sample_WindowHeight_Int32 = "windowHeight";
static const char * const k_pch_Sample_RenderWidth_Int32 = "renderWidth";
static const char * const k_pch_Sample_RenderHeight_Int32 = "renderHeight";
static const char * const k_pch_Sample_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char * const k_pch_Sample_DisplayFrequency_Float = "displayFrequency";


class HmdDevice : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent
{
public:
	HmdDevice();

	virtual ~HmdDevice();


	virtual EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId);

	void initPose();

	virtual void Deactivate();

	virtual void EnterStandby();

	virtual void *GetComponent(const char *pchComponentNameAndVersion);

	virtual void PowerOff();

	/** debug request from a client */
	virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize);

	virtual void GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight);

	virtual bool IsDisplayOnDesktop();

	virtual bool IsDisplayRealDisplay();

	virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight);

	virtual void GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight);
	
	virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom);

	virtual DistortionCoordinates_t ComputeDistortion(EVREye eEye, float fU, float fV);
	
	virtual DriverPose_t GetPose();


	void updatePose(float ms = 0.01);
	void setMouseMove(int x, int y);
	void setKeyDown(char key, bool isdown);
	std::string GetSerialNumber() const;
	vr::TrackedDeviceIndex_t m_unObjectId;
private:
	
	
	vr::PropertyContainerHandle_t m_ulPropertyContainer;
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
	
	DriverPose_t m_pose;
	float mouseY=0;
	float mouseX=0;

	bool WDown=0;
	bool SDown=0;
	bool ADown=0;
	bool DDown=0;
};