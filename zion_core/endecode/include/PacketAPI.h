
#ifndef	_PACKETAPI_H_
#define	_PACKETAPI_H_

#include "SystemAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

typedef	void*	FrPacketHandle;

FrPacketHandle FrPacketOpen(FrMediaHandle hMedia, BOOL bRTSP);
BOOL FrPacketInit(FrPacketHandle hHandle, BOOL bUDP);
void FrPacketClose(FrPacketHandle hHandle);
void FrPacketPutData(FrPacketHandle hHandle, BYTE* pData, DWORD32 dwData, DWORD32 dwCTS);
BYTE* FrPacketGetData(FrPacketHandle hHandle, DWORD32* pdwPacket, DWORD32* pdwCTS, BOOL* pbMBit);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	// _PACKETAPI_H_
