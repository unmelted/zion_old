#ifndef _ESM_BASE_H_
#define _ESM_BASE_H_

//#include <winsock2.h>
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <climits>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>

#if defined(WIN32) && defined(_USRDLL)
#if defined(EXPORT_ESM_BASE)
#define ESM_BASE_CLASS __declspec(dllexport)
#else
#define ESM_BASE_CLASS __declspec(dllimport)
#endif
#else
#define ESM_BASE_CLASS 
#endif

// need to clearify
#ifndef MAX_PATH
#define MAX_PATH	260
#endif

//class ESM_BASE_CLASS ESMBase
class ESMBase
{
public:
	static const int32_t COMMAND_MAKE_T_NAME_SIZE = MAX_PATH;

	typedef struct _ERR_CODE_T
	{
		static const int UNKNOWN = -1;
		static const int SUCCESS = 0;
		static const int GENERIC_FAIL = 1;
		static const int INVALID_PARAMETER = 2;

		static const int32_t UNINITIALIZED_ERROR = 3;
		static const int32_t OPEN_INPUT_ERROR = 4;
		static const int32_t FIND_STREAM_INFO_ERROR = 5;
		static const int32_t STREAM_INDEX_ERROR = 6;
		static const int32_t IMAGE_ADJUST_ERROR = 7;
		static const int32_t VMCC_ERROR = 8;
		static const int32_t ERR_AGAIN = 9;
		static const int32_t FINISH_DECODE = 10;
		static const int32_t GOPSIZE_IS_NOT_EQUAL = 11;
		static const int32_t FRAMECOUNT_DIFFER_FROM_FPS = 12;
		static const int32_t FILEWRITE_ERROR = 13;
		static const int32_t DECODING_FAIL = 14;

		static const int32_t JSON_PARSE_DOC_ERROR = 20;
		static const int32_t JSON_PARSE_INT_ERROR = 21;
		static const int32_t JSON_PARSE_STR_ERROR = 22;
		static const int32_t JSON_PARSE_CODE_NOT_FOUND = 23;
		static const int32_t JSON_PARSE_KEY_NOT_FOUND = 24;
		static const int32_t JSON_PARSE_VALUE_NOT_FOUND = 25;
		static const int32_t JSON_PARSE_TYPE_ERROR = 26;
		static const int32_t JSON_FORMAT_ERROR = 27;
		static const int32_t API_ID_NOT_FOUND = 28;

		static const int32_t FILE_NAME_FORMAT_ERROR = 50;
		static const int32_t FILE_PATH_NOT_FOUND = 51;
		static const int32_t MESSAGE_CODE_ERROR = 52;

		static const int32_t SOCKET_WINSOCK_INIT_ERROR = 100;
		static const int32_t SOCKET_BIND_ERROR = 101;
		static const int32_t SOCKET_LISTEN_ERROR = 102;
		static const int32_t SOCKET_TIMEOUT_ERROR = 103;
		static const int32_t SOCKET_SELECT_ERROR = 104;

		static const int32_t TEMPLATE_NOT_SUPPORT = 150;
		static const int32_t LOGO_SIZE_MISMATCH = 200;
		static const int32_t VIDEO_MAKE_FAIL = 300;
		static const int32_t SDI_FAIL = 400;

		static const int32_t INFER_INIT_ERROR = 500;
	} ERR_CODE_T;

	typedef struct _MEDIA_TYPE_T
	{
		static const int UNKNOWN = 0x00;
		static const int VIDEO = 0x01;
		static const int AUDIO = 0x02;
	} MEDIA_TYPE_T;

	typedef struct _VIDEO_CODEC_T
	{
		static const int UNKNOWN = -1;
		static const int AVC = 0;
		static const int HEVC = 1;
	} VIDEO_CODEC_T;

	typedef struct _COLORSPACE_T
	{
		static const int NV12 = 0;
		static const int BGRA = 1;
		static const int YV12 = 2;
		static const int I420 = 3;
		static const int UYVY = 4;
	} COLORSPACE_T;

	typedef struct _AVC_PROFILE_T
	{
		static const int BP = 0;
		static const int HP = 1;
		static const int MP = 2;
	} AVC_PROFILE_T;

	typedef struct _HEVC_PROFILE_T
	{
		static const int DP = 0;
		static const int MP = 1;
	} HEVC_PROFILE_T;

	typedef struct _VIDEO_MEMORY_TYPE_T
	{
		static const int HOST = 0;
		static const int CUDA = 1;
	} VIDEO_MEMORY_TYPE_T;

	typedef struct _AUDIO_CODEC_T
	{
		static const int UNKNOWN = -1;
		static const int MP3 = 0;
		static const int AAC = 1;
		static const int AC3 = 2;
	} AUDIO_CODEC_T;

	typedef struct _AUDIO_SAMPLE_FORMAT_T
	{
		static const int UNKNOWN = -1;
		static const int FMT_U8 = 0;
		static const int FMT_S16 = 1;
		static const int FMT_S32 = 2;
		static const int FMT_FLOAT = 3;
		static const int FMT_DOUBLE = 4;
		static const int FMT_S64 = 5;
		static const int FMT_U8P = 6;
		static const int FMT_S16P = 7;
		static const int FMT_S32P = 8;
		static const int FMT_FLOATP = 9;
		static const int FMT_DOUBLEP = 10;
		static const int FMT_S64P = 11;
	} AUDIO_SAMPLE_FORMAT_T;

	typedef struct _INTER_COMM_MODE_T
	{
		static const int32_t CONFIG = 0;
		static const int32_t MAKE = 1;
		static const int32_t SET_ADJUST = 2;
		static const int32_t HEATH_CHECK = 3;
		static const int32_t VISUAL_SERVOING = 4;
		static const int32_t STOP_PRESET = 5;
		static const int32_t MMC_ERROR = 6;
	} INTER_COMM_MODE_T;


	typedef struct _INTER_MMSC_PACKET_TYPE_T
	{
		static const int32_t TEXT = 0;
		static const int32_t BINARY = 1;
	} INTER_MMSC_PACKET_TYPE_T;

	typedef struct _INTER_MMSC_PACKET_T
	{
		int32_t		type;
		int32_t		length;
		char* payload;
		_INTER_MMSC_PACKET_T(void)
			: type(_INTER_MMSC_PACKET_TYPE_T::TEXT)
			, length{ 0 }
			, payload(nullptr)
		{}

		~_INTER_MMSC_PACKET_T(void)
		{
			if (payload && length > 0)
				free(payload);
			payload = nullptr;
			length = 0;
		}
	} INTER_MMSC_PACKET_T;

	typedef struct _COMMAND_T
	{
		int16_t command;
		int16_t status;
	} COMMAND_T;

	typedef struct _COMMAND_MAKE_T : public _COMMAND_T
	{
		int32_t frameNumber;
		int32_t length;
		char name[COMMAND_MAKE_T_NAME_SIZE];
	} COMMAND_MAKE_T;

	static const int32_t EXTRA_DATA = 0;


	typedef struct _CUDA_IMAGE_CONVERTING_T
	{
		static const int32_t ERROR_INVALID_GPU_MAT = -1;
		static const int32_t ERROR_INPUT_OR_OUPUT_IS_NOT_3CHANNEL = -2;
		static const int32_t ERROR_INPUT_IS_NOT_YUV420 = -3;
		static const int32_t ERROR_ORIGINAL_IMAGE_IS_EMPTY = -4;
		static const int32_t ERROR_HOMOGRAPHY_IS_EMPTY = -5;
		static const int32_t ERROR_HOMOGRAPHY_IS_NOT_INVALID = -6;

	} CUDA_IMAGE_CONVERTING_T;

	enum class FrameType // Type of Frame. Note that it is related to the period.
	{
		Normal, // Normal play
		Still, // Play of the stopped image
		Rotation //  Play of the rotating images
	};
};

#endif
