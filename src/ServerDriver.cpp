#include "ServerDriver.h"
HmdDevice* ServerDriver::m_pNullHmdLatest = NULL;
ControllerDevice* ServerDriver::m_rightHand = NULL;

bool ServerDriver::isWorking=1;
bool ServerDriver::enableMouse=0;
EVRInitError ServerDriver::Init(vr::IVRDriverContext *pDriverContext)
{
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());
	isWorking = 1;
	if (m_pNullHmdLatest == NULL)
		m_pNullHmdLatest = new HmdDevice();
	vr::VRServerDriverHost()->TrackedDeviceAdded("sampleHMD", vr::TrackedDeviceClass_HMD, m_pNullHmdLatest);
	if(m_rightHand == NULL)
		m_rightHand = new ControllerDevice(TrackedControllerRole_RightHand);
	vr::VRServerDriverHost()->TrackedDeviceAdded("sampleController", vr::TrackedDeviceClass_Controller, m_rightHand);

	m_Thread = new std::thread(updatePostThread);
	return VRInitError_None;
}

void ServerDriver::Cleanup()
{
	if (threadId)
		PostThreadMessage(threadId, WM_CLOSELOOP, 0, 0);
	isWorking = 0;
	DriverLog("Cleanup");
	CleanupDriverLog();
	if(m_pNullHmdLatest!=NULL)
		delete m_pNullHmdLatest;
	m_pNullHmdLatest = NULL;
	if (m_rightHand != NULL)
		delete m_rightHand;
	m_rightHand = NULL;
}
void ServerDriver::debug(const char* str, int i)
{
	DriverLog(str, i);
}
void ServerDriver::debug(const char* str)
{
	DriverLog(str);
}
void ServerDriver::setHookThread(DWORD threadId)
{
	this->threadId = threadId;
}
void ServerDriver::setKeyDown(char key, bool isdown)
{
	if (m_pNullHmdLatest)
	{
		m_pNullHmdLatest->setKeyDown(key, isdown);
	}
	if (m_rightHand)
	{
		m_rightHand->setKeyDown(key, isdown);
	}
}
void ServerDriver::setMouseMove(int x, int y)
{
	if (m_pNullHmdLatest)
	{
		m_pNullHmdLatest->setMouseMove(x, y);
	}
}

void ServerDriver::updatePostThread()
{
	VREvent_t event;
	while (isWorking)
	{
		if (m_pNullHmdLatest)
		{
			m_pNullHmdLatest->updatePose();
			vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_pNullHmdLatest->m_unObjectId, m_pNullHmdLatest->GetPose(), sizeof(DriverPose_t));
			if (m_rightHand)
			{
				m_rightHand->updatePose(m_pNullHmdLatest->GetPose());
				vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_rightHand->m_unObjectId, m_rightHand->GetPose(), sizeof(DriverPose_t));
			}
		}
		
		Sleep(10);
	}
}

void ServerDriver::RunFrame()
{
}