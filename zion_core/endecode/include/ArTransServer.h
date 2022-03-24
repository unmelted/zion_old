#pragma once

#include "SystemAPI.h"
#include "TraceAPI.h"
#include "StreamReaderAPI.h"
#include "StreamWriterAPI.h"
#include "DecoderAPI.h"
#include "EncoderAPI.h"
#include "ConverterAPI.h"

#include "StreamRTPSender.h"

#include <thread>
#include <string>
#include <mutex>

#define		SEND_TIME_DIFF				1000
#define		MAX_TIME_DIFF				2000

typedef struct {
	std::string url;
	std::string dstUrl;
	std::string codec;		// h264
	std::string	codecType;
	std::string	codecEncType;

	DWORD32 dwWidth;
	DWORD32 dwHeight;
	DWORD32 dwInterval;
	DWORD32 dwFramerate;
	DWORD32 dwBitrate;
	DWORD32 dwGopLength;
	DWORD32	dwThreadNum;
	
	SHORT	wSvrPort;
	SHORT	wPushPort;

	BOOL	bEnableStreaming;
	BOOL	bBenchTest;
	
} ArSvrParam;

typedef struct {
	FrVideoDecHandle m_hDecVideo;
	FrVideoEncHandle m_hEncVideo;
	FrVideoResizeHandle	m_hScaler;
	
	FrMediaInfo m_SrcInfo;
	FrMediaInfo m_EncInfo;

	FrMediaStream m_VideoStream;
	FrRawVideo m_RawVideo;
} FrArTransStruct;


class FrArServer {
private:
	BOOL	m_bEnd;
	BOOL	m_bServer = false;
	FrReaderHandle	m_hReader = nullptr;
	FrWriterHandle	m_hWriter = nullptr;
	FrVideoDecHandle m_hDecVideo = nullptr;
	FrVideoEncHandle m_hEncVideo = nullptr;
	FrVideoResizeHandle	m_hScaler = nullptr;
	FrVideoConvHandle m_hConv = nullptr;

	FrURLInfo	m_UrlInfo = {};
	FrURLInfo	m_DstUrlInfo = {};
	FrMediaInfo m_SrcInfo = {};
	FrMediaInfo m_EncInfo = {};

	FrMediaStream m_VideoStream = {};
	FrRawVideo m_RawVideo = {};
	TASK_HANDLE m_hArSvrTask = INVALID_TASK; // or

	BOOL	m_bEncoding = false;
	
	FrStreamRTPSender* m_hSender = nullptr;
	std::thread* m_hArSvrThread = nullptr;

	FrTimeHandle		m_hTime = nullptr;
	BOOL		m_bBenchTest = false;
	char		m_szDstName[1024] = "";

	// throughput test..
	/*int m_sizeOfCh{};
	std::vector<std::vector<FrMediaStream>> m_vStreams;
	std::vector<FrReaderHandle>	m_vhReader;
	std::vector<FrWriterHandle>	m_vhWriter;
	std::vector<FrVideoDecHandle> m_vhDecVideo;
	std::vector<FrVideoEncHandle> m_vhEncVideo;
	std::vector<FrVideoResizeHandle> m_vhScaler;
	std::vector<FrVideoConvHandle> m_vhConv;

	std::vector<std::thread*> m_vhReadThread;
	std::vector<std::thread*> m_vhDecThread;
	std::thread* m_hEncThread = nullptr;

	std::mutex	m_xFrame;
	std::mutex	m_xRaw;*/

	int SvrInit(FrURLInfo& Info, short listenPort, FrURLInfo& DstInfo);
	int SvrStart();
	int SvrStop();
	
	void ArServerThread();
	void ReadThread(int idx);
	void DecThread(int idx);
	void EncThread();

public:
	FrArServer();
	virtual ~FrArServer();
	int FrSvrOpen(ArSvrParam& svrParam);
	void FrSvrClose();
	void FrBenchTest(ArSvrParam& svrParam);
};
