//============================================================================
// Name        : rg_engine.cpp
// Author      : 
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <rg_engine.h>

#include <rg_vecmath.h>

//#include <codec_api.h>

int main(int argc, rg_string* argv) {
//	int width;
//	int height;

//	ISVCDecoder* decoder;
//	WelsCreateDecoder(&decoder);
//	SDecodingParam param = {0};
//	SBufferInfo bufferInfo;
//	param.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
//	decoder->Initialize(&param);
//	bufferInfo = {0};
//	bufferInfo.iBufferStatus = 1;
//
//	uint8_t** yuvData = new uint8_t*[3];
//	yuvData[0] = new uint8_t[width * height];
//	yuvData[1] = new uint8_t[width * height / 4];
//	yuvData[2] = new uint8_t[width * height / 4];
//
//	const unsigned char* rawEncodedData;
//
//	int err = decoder->DecodeFrameNoDelay(rawEncodedData, rawEncodedDataLength, yuvData, &bufferInfo);
//	if (err != 0) {
//		printf("Error: %d\n", err);
//	}

//	WelsDestroyDecoder(decoder);




//	FILE* file  = fopen("video.mp4", "rb");
//	if(!file) {
//		printf("File error!\n");
//	}
//	fseek(file, 0, SEEK_END);
//	int iSize = ftell(file);
//	fseek(file, 0, SEEK_SET);
//	unsigned char* pBuf = (unsigned char*)malloc(iSize);
//	fread(pBuf, 1, iSize, file);
//	fclose(file);
//
//	ISVCDecoder* pSvcDecoder;
//	SBufferInfo sDstBufInfo;
//	memset(&sDstBufInfo, 0, sizeof(SBufferInfo));
//
//	if(WelsCreateDecoder(&pSvcDecoder)) {
//		printf("Decoder error!\n");
//	}
//
//	SDecodingParam sDecParam = {0};
//	sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
//	sDecParam.bParseOnly = false;
//	if(pSvcDecoder->Initialize(&sDecParam)) {
//		printf("Init error!\n");
//	}
//
//	int width = 480;
//	int height = 360;
//	uint8_t** pData = new uint8_t*[3];
//	pData[0] = new uint8_t[width * height];
//	pData[1] = new uint8_t[width * height / 4];
//	pData[2] = new uint8_t[width * height / 4];
//
//	int iRet;
//	while((iRet = pSvcDecoder->DecodeFrameNoDelay(pBuf, iSize/32, pData, &sDstBufInfo)) != 0) {
//		printf("Status: %d %d\n", iRet, sDstBufInfo.iBufferStatus);
//		width = sDstBufInfo.UsrData.sSystemBuffer.iWidth;
//		height = sDstBufInfo.UsrData.sSystemBuffer.iHeight;
//		printf("Init frame: %dx%d\n", width, height);
//		break;
//	}
//
//	printf("Error: %x\n", iRet);
//
//
//	pSvcDecoder->Uninitialize();
//	WelsDestroyDecoder(pSvcDecoder);


//	quat q = {0, 0, 1, 0};
//
//	mat4 matrix;
//	mat4_identity(&matrix);
//	mat4_quat(&matrix, &q);
//
//	printf("~~~~~~~~~~~~\n");
//	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
//			matrix.m00, matrix.m01, matrix.m02, matrix.m03,
//			matrix.m10, matrix.m11, matrix.m12, matrix.m13,
//			matrix.m20, matrix.m21, matrix.m22, matrix.m23,
//			matrix.m30, matrix.m31, matrix.m32, matrix.m33);

	rg_init(argc, argv);
	rg_start();
	return 0;
}
