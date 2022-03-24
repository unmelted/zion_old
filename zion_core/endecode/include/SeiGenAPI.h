#pragma once
#include "SystemAPI.h"

class FrPoseRecon {
public:
	class SeiGen;
	class PoseRecon;
public:
	typedef struct {
		HANDLE hWnd;
	} Param;

	FrPoseRecon();
	~FrPoseRecon();
	int Open(Param& param);
	int Close();
	int Start();
	int Stop();

private:
	FrPoseRecon::SeiGen* m_hSeiGen;
	FrPoseRecon::PoseRecon* m_hPose;
};
