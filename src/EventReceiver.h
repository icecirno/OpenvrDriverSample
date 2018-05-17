#pragma once
#include "openvr\openvr_driver.h"
class EventReceiver
{
public:
	virtual void keyEvent(int key, bool isdown)=0;
	virtual void mouseEvent(int x, int y) = 0;
	virtual void updatePoseEvent(float deltatime = 0.01) = 0;
	virtual void updatePoseEvent(vr::DriverPose_t base, float deltatime =0.01) = 0;
	virtual void InitPoseEvent() = 0;
};