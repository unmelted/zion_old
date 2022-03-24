#pragma once

#include <vector>
#include "cuda_runtime.h"
#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include "opencv2/cudawarping.hpp"
#include "opencv2/cudaarithm.hpp"

class FrCvClrConv {
public:
	static void YUV2BGR(cv::cuda::GpuMat& img, unsigned char* pVideo, int width, int height, int pitch);
	static void BGR2YUV(uint8_t* pYUV, int pitch, cv::cuda::GpuMat& img);
};
