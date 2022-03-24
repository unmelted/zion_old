#pragma once

#include "mediainfo.h"

// All of these variable's value is gotton from Config File except m_dwServiceMode.
typedef struct {
	SHORT wSvrPort;
	SHORT wPushPort;
} FrSvrConfigInfo;


typedef struct {
	DWORD32 dwWidth;
	DWORD32 dwHeight;
	DWORD32 dwInterval;
	DWORD32 dwFramerate;
	DWORD32 dwBitrate;
} FrTransInfo;

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */




typedef void* FrArServerHandle;


LRSLT FrArServerOpen(FrArServerHandle* phServer, FrSvrConfigInfo* pCfgInfo, FrTransInfo* pTransInfo);
void FrArServerClose(FrArServerHandle hHandle);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
