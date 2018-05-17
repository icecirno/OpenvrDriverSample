#include "ServerDriver.h"
HmdDevice* ServerDriver::m_Hmd = NULL;
std::vector<EventReceiver*> ServerDriver::receivers;
bool ServerDriver::isWorking=1;
bool ServerDriver::enableMouse=0;
EVRInitError ServerDriver::Init(vr::IVRDriverContext *pDriverContext)
{
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());
	isWorking = 1;
	m_Hmd = new HmdDevice("1234", "SampleHMD","{sample}/input/vive_profile.json");
	receivers.push_back(m_Hmd);
	receivers.push_back(new ControllerDevice("12341", "SampleLeftController", "{sample}/input/vive_controller_profile.json", TrackedControllerRole_RightHand));
	for (EventReceiver*r : receivers)
	{
		r->InitPoseEvent();
	}
	m_Thread = new std::thread(updatePostThread);
	return VRInitError_None;
}

void ServerDriver::Cleanup()
{
	if (threadId)
		PostThreadMessage(threadId, WM_CLOSELOOP, 0, 0);
	isWorking = 0;
	Sleep(1);
	DriverLog("Cleanup");
	CleanupDriverLog();
	
}
void ServerDriver::setKeyDown(char key, bool isdown)
{
	for (EventReceiver*r : receivers)
	{
		r->keyEvent(key, isdown);
	}
}
void ServerDriver::setMouseMove(int x, int y)
{
	for (EventReceiver*r : receivers)
	{
		r->mouseEvent(x, y);
	}
}

void ServerDriver::updatePostThread()
{
	VREvent_t event;
	while (isWorking)
	{
		for(EventReceiver*r : receivers)
		{
			r->updatePoseEvent(m_Hmd->GetPose());
		}
		Sleep(10);
	}
	m_Hmd = 0;
	for (EventReceiver*r : receivers)
		delete r;
	receivers.clear();
}

void ServerDriver::RunFrame()
{
}