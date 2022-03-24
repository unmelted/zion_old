#pragma once

#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */

#define	CODEC_NVIDIA	100
#define CODEC_FFMPEG	200


typedef struct {
	int codec_type;
	unsigned char* pData;
	int size;
	int width;
	int height;
	int pitch;

	unsigned char* pY;
	unsigned char* pU;
	unsigned char* pV;
} CbData;

typedef struct {
	char *pUrl;
	int codec_type;

	unsigned int newWidth;
	unsigned int newHeight;

} MgVideoParam;

typedef void* FrMgVideoHandle;


FrMgVideoHandle FrMgVideoOpen(MgVideoParam* pParam);
void FrMgVideoRegCallback(FrMgVideoHandle hVideo, void* fVideoCallback, void* phCallbackHandle);
void FrMgVideoClose(FrMgVideoHandle hVideo);
int FrMgVideoStart(FrMgVideoHandle hVideo);
int FrMgVideoStop(FrMgVideoHandle hVideo);


#ifdef __cplusplus
}
#endif	/* __cplusplus */
