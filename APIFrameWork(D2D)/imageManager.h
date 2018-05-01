#pragma once
#include "SingletonBase.h"
#include "image.h"

//==================================================================
//		## imageManager ## (이미지 매니져)
//==================================================================
class imageManager : public singletonBase <imageManager>
{
private:
	typedef map<tstring, image*> mapImageList;
	typedef map<tstring, image*>::iterator mapImageIter;

private:
	mapImageList m_imageList;	// 맵으로 구현된 이미지 목록

public:
	imageManager();
	~imageManager();

	// 이미지 매니저 초기화
	HRESULT init(void);
	// 이미지 매니저 해제
	void release(void);

	// 키값으로 이미지 파일 초기화
	image* addImage(ID2D1RenderTarget* RenderTarget, tstring strKey, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// 키값으로 프레임 이미지 파일 초기화
	image* addFrameImage(ID2D1RenderTarget* RenderTarget, tstring strKey, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);


	// 이미지 키값을 찾기
	image* findImage(tstring strKey);
	bool searchImage(tstring strKey);
	// 이미지 키값으로 삭제
	bool deleteImage(tstring strKey);
	// 이미지 전체 삭제
	bool deleteAll(void);

	//==================================================================
	//		## 일반렌더 ##
	//==================================================================
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget);
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float x, float y);
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float destX, float destY, float destWidth, float destHeight, float sorceX, float sorceY, float sourceWidth, float sourceHeight);
	void renderByMask(tstring strKey, ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## 일반렌더 ##
	//==================================================================
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float magnification);

	//==================================================================
	//		## 일반렌더 ##
	//==================================================================
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float x, float y, float magnification);

	//==================================================================
	//		## 알파렌더 ##
	//==================================================================
	void alphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, float alpha);
	void alphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, float alpha, float x, float y, float magnification = 1.0f);

	//==================================================================
	//		## 프레임렌더 ##
	//==================================================================
	void frameRender(tstring strKey, ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);

	//==================================================================
	//		## 알파프레임렌더 ##
	//==================================================================
	void frameAlphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY, float alpha);

	//==================================================================
	//		## bottom일반렌더 ##
	//==================================================================
	void bottomRender(tstring strKey, ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## bottom알파렌더 ##
	//==================================================================
	void bottomAlphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, float alpha);

	//==================================================================
	//		## bottom프레임렌더 ##
	//==================================================================
	void  bottomFrameRender(tstring strKey, ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);

	//==================================================================
	//		## bottom알파프레임렌더 ##
	//==================================================================
	void  bottomFrameAlphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY, float alpha);

	void setWidth(tstring strKey, float width) {

		image* img = findImage(strKey);
		if (img) img->setWidth(width);
	}
	void setHeight(tstring strKey, float height) {
		image* img = findImage(strKey);
		if (img) img->setHeight(height);
	}
};