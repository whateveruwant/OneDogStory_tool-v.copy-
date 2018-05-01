#pragma once
#include "gameNode.h"

enum LOAD_KIND {
	LOAD_KIND_IMAGE_0,
	LOAD_KIND_IMAGE_1,
	LOAD_KIND_FRAMEIMAGE_0,
	LOAD_KIND_SOUND,
	LOAD_KIND_END
};

struct tagImageResource {
	tstring keyName;			// 이미지 키값
	const TCHAR* fileName;	// 파일 이름
	int x, y;				// 위치 좌표
	int width, height;		// 가로세로 크기
	int frameX, frameY;		// 프레임 xy
	bool isCenter;
};

class loadItem {
private:
	LOAD_KIND kind;
	tagImageResource imageResource;

public:
	loadItem() {}
	~loadItem() {}

	// 키값으로 이미지 파일 초기화
	HRESULT initForImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// 키값으로 프레임 이미지 파일 초기화
	HRESULT initForFrameImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);

	// 로딩 아이템 종류 가져오기
	LOAD_KIND getLoadingKind() { return kind; }
	tagImageResource getImageResource() { return imageResource; }

};

class loading : public gameNode
{
private:
	typedef vector<loadItem*> arrLoadItem;

private:
	arrLoadItem vLoadItem;

	// 로딩화면에서 사용할 이미지 및 로딩바
	image* backGround;
	int currentGauge;

public:
	loading();
	~loading();

	HRESULT init();
	void release();
	void update();
	void render();

	// 키값으로 이미지 파일 초기화
	void loadImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// 키값으로 프레임 이미지 파일로 초기화
	void loadFrameImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);

	// 로딩 완료
	bool loadingDone();

	// 로드 아이템 벡터 가져오기
	arrLoadItem getLoadItem() { return vLoadItem; }
};