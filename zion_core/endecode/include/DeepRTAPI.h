#pragma once
#include "SystemAPI.h"
#include <string>

class FrDeepRT {
public:
	class DeepRT;
public:
	typedef struct {
		std::string srcPath;
		std::string dstPath;
		std::string detectRTEnginePath;
		std::string poseRTEnginePath;
	} Param;

	FrDeepRT();
	~FrDeepRT();
	int Run(Param& param);
	int Close();
	
private:
	FrDeepRT::DeepRT* m_hDeep;
};
