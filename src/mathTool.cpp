#include "mathTool.h"



mathTool::mathTool()
{
}


mathTool::~mathTool()
{
}
HmdQuaternion_t mathTool::HmdQuaternion_Init(double w, double x, double y, double z)
{
	HmdQuaternion_t quat;
	quat.w = w;
	quat.x = x;
	quat.y = y;
	quat.z = z;
	return quat;
}
HmdQuaternion_t mathTool::Mul(HmdQuaternion_t& A,const HmdQuaternion_t&rkQ)
{
	HmdQuaternion_t R = { 0 };
	R.w = A.w * rkQ.w - A.x * rkQ.x - A.y * rkQ.y - A.z * rkQ.z;
	R.x = A.w * rkQ.x + A.x * rkQ.w + A.y * rkQ.z - A.z * rkQ.y;
	R.y = A.w * rkQ.y + A.y * rkQ.w + A.z * rkQ.x - A.x * rkQ.z;
	R.z = A.w * rkQ.z + A.z * rkQ.w + A.x * rkQ.y - A.y * rkQ.x;
	return R;
}
void mathTool::HmdMatrix_SetIdentity(HmdMatrix34_t *pMatrix)
{
	pMatrix->m[0][0] = 1.f;
	pMatrix->m[0][1] = 0.f;
	pMatrix->m[0][2] = 0.f;
	pMatrix->m[0][3] = 0.f;
	pMatrix->m[1][0] = 0.f;
	pMatrix->m[1][1] = 1.f;
	pMatrix->m[1][2] = 0.f;
	pMatrix->m[1][3] = 0.f;
	pMatrix->m[2][0] = 0.f;
	pMatrix->m[2][1] = 0.f;
	pMatrix->m[2][2] = 1.f;
	pMatrix->m[2][3] = 0.f;
}