#pragma once
#include "openvr\openvr_driver.h"
using namespace vr;

class mathTool
{
public:
	mathTool();
	~mathTool();
	static inline HmdQuaternion_t HmdQuaternion_Init(double w, double x, double y, double z);
	static HmdQuaternion_t mathTool::Mul(HmdQuaternion_t& A, const HmdQuaternion_t&rkQ);
	static inline void HmdMatrix_SetIdentity(HmdMatrix34_t *pMatrix);

};

