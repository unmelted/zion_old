#pragma once

#include "SystemAPI.h"
#include "TraceAPI.h"
#include "SocketAPI.h"
#include "StreamReaderAPI.h"
#include "StreamWriterAPI.h"
#include "DecoderAPI.h"
#include "EncoderAPI.h"
#include "ConverterAPI.h"

#include <thread>
#include <string>


#define		MAX_TIME_DIFF				2000

typedef struct {
	std::string url;
	std::string codec;		// h264
	std::string	codecType;

	DWORD32 dwWidth;
	DWORD32 dwHeight;
	DWORD32 dwInterval;
	DWORD32 dwFramerate;
	DWORD32 dwBitrate;
	
	SHORT	wSvrPort;
	SHORT	wPushPort;
	
} HelperParam;


class FrMgHelper {
private:
	BOOL	m_bEnd;
	FrReaderHandle	m_hReader = nullptr;
	FrWriterHandle	m_hWriter = nullptr;
	FrVideoDecHandle m_hDecVideo = nullptr;
	FrVideoEncHandle m_hEncVideo = nullptr;
	FrVideoResizeHandle	m_hScaler = nullptr;

	FrURLInfo	m_UrlInfo = {};
	FrMediaInfo m_SrcInfo = {};
	FrMediaInfo m_EncInfo = {};

	FrMediaStream m_VideoStream = {};
	FrRawVideo m_RawVideo = {};
	
	FrTimeHandle		m_hTime = nullptr;
	std::thread* m_hHelperThread = nullptr;

	int HelperInit(FrURLInfo& Info, short listenPort);
	int HelperStart();
	int HelperStop();
	
	void HelperServerThread();

public:
	FrMgHelper();
	virtual ~FrMgHelper();
	int FrMgHelperOpen(HelperParam& helperParam);
	void FrMgHelperClose();
};
