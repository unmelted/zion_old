

#ifndef	_DEPACKETAPI_H_
#define	_DEPACKETAPI_H_

#include "SystemAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef	void*	FrDepacketHandle;

FrDepacketHandle FrDepacketOpen(FrMediaHandle hMedia);
BOOL FrDepacketInit(FrDepacketHandle hHandle);
void FrDepacketClose(FrDepacketHandle hHandle);
void FrDepacketPutData(FrDepacketHandle hHandle, BYTE* pPacket, DWORD32 dwPacket, DWORD32 dwCTS);
BYTE* FrDepacketGetData(FrDepacketHandle hHandle, DWORD32* dwData, DWORD32* dwCTS);
void FrDepacketGetPacketInfo(FrDepacketHandle hHandle, DWORD32* pdwPacketNum, DWORD32* pdwPackeLen);

#ifdef __cplusplus
}
#endif

#endif	// _DEPACKETAPI_H_
