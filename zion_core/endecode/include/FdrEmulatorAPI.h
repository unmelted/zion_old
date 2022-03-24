#pragma once

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


typedef struct {
	// PreSd
	char* pUrl;
	char szIp[1024];
	short port;
	
	int sessionNum;

	// PostSd
	char* pSave;
	short wListenPort;
	
} FdrEmulParam;

typedef struct {
	// PreSd
	char* pUrl;
	char VpdIp[1024];
	char PostIp[1024];
	short port;
} FdrEmulPreParam;

typedef struct {
	char VpdIp[1024];
	short vpdPort;

	char PostSdIp[1024];
	short postPort;
} FdrEmulPcdParam;

typedef void* FdrEmulHandle;


FdrEmulHandle FdrEmulOpen(FdrEmulParam* pParam);
void FdrEmulClose(FdrEmulHandle hEmul);
int FdrEmulPreStart(FdrEmulHandle hEmul, FdrEmulPreParam* param);
int FdrEmulPreStop(FdrEmulHandle hEmul);

// Pcd
int FdrEmulConnectPost(FdrEmulHandle hEmul, FdrEmulPcdParam* param);
int FdrEmulSendPost(FdrEmulHandle hEmul, const char* payload, int32_t size);

int FdrEmulConnectVpd(FdrEmulHandle hEmul, FdrEmulPcdParam* param);
int FdrEmulSendVpd(FdrEmulHandle hEmul, const char* payload, int32_t size);

int FdrEmulRestartCamera(FdrEmulHandle hEmul);



#ifdef __cplusplus
}
#endif	/* __cplusplus */