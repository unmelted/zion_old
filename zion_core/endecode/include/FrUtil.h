#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <chrono>

namespace fr {
	void cvYUV2BGR24(int w, int h, int pitch, uchar* y, uchar* u, uchar* v, uchar* bgr);

	class conv {
	public:
		static void YUVToBGR24(int w, int h, int pitch, uchar* y, uchar* u, uchar* v, uchar* bgr);
	};

	// tick
	std::chrono::system_clock::time_point GetTickCount();
	//auto GetTickDiff(std::chrono::system_clock::time_point end, std::chrono::system_clock::time_point start);
	
	class Tick {
	public:
		void Start();
		long Stop();
	private:
		std::chrono::system_clock::time_point t0;
	};
}
