#include "stdafx.h"
#include "animation.h"
#pragma once
class image
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE = 0,		//리소스로 로딩
		LOAD_FILE,				//파일로 로딩
		LOAD_EMPTY,				//빈비트맵 로딩
		LOAD_END
	};

	typedef struct tagImage
	{
		DWORD		resID;			//리소스 ID	
		float		x;				//이미지 x좌표
		float		y;				//이미지 y좌표
		float		width;			//이미지 가로크기
		float		height;			//이미지 세로크기
		int			currentFrameX;	//현재 프레임 X
		int			currentFrameY;	//현재 프레임 Y
		int			maxFrameX;		//최대 X프레임 갯수
		int			maxFrameY;		//최대 Y프레임 갯수
		float		frameWidth;		//1프레임 가로길이
		float		frameHeight;	//1프레임 세로길이
		BYTE		loadType;		//이미지 로드타입

		tagImage()
		{
			resID = 0;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_RESOURCE;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	ID2D1Bitmap *_bitmap;
	TCHAR* _fileName;
	LPIMAGE_INFO	_imageInfo;		//이미지 정보
	float _alpha;
	bool _isCenter;

public:
	image();
	~image();

	// 이미지 파일 초기화
	HRESULT init(ID2D1RenderTarget* RenderTarget, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// 프레임 이미지 파일 초기화
	HRESULT init(ID2D1RenderTarget* RenderTarget, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);

	//==================================================================
	//		## 일반렌더 ##
	//==================================================================
	void render(ID2D1RenderTarget* RenderTarget);
	void render(ID2D1RenderTarget* RenderTarget, float x, float y);
	void render(ID2D1RenderTarget* RenderTarget, float destX, float destY, float destWidth, float destHeight, float sorceX, float sorceY, float sourceWidth, float sourceHeight);
	void renderByMask(ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## 일반렌더(배율) ##
	//==================================================================
	void magnificateRender(ID2D1RenderTarget* RenderTarget, float magnification);
	void magnificateRender(ID2D1RenderTarget* RenderTarget, float magnification, float x, float y);

	//==================================================================
	//		## 알파렌더 ##
	//==================================================================
	void alphaRender(ID2D1RenderTarget* RenderTarget, float alpha);
	void alphaRender(ID2D1RenderTarget* RenderTarget, float alpha, float magnification, float x, float y);
	
	//==================================================================
	//		## 프레임렌더 ##
	//==================================================================
	void frameRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);
	
	//==================================================================
	//		## 알파프레임렌더 ##
	//==================================================================
	void frameAlphaRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY, float alpha);

	//==================================================================
	//		## bottom일반렌더 ##
	//==================================================================
	void bottomRender(ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## bottom알파렌더 ##
	//==================================================================
	void bottomAlphaRender(ID2D1RenderTarget* RenderTarget, float alpha);

	//==================================================================
	//		## bottom프레임렌더 ##
	//==================================================================
	void bottomFrameRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);

	//==================================================================
	//		## bottom알파프레임렌더 ##
	//==================================================================
	void bottomFrameAlphaRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY, float alpha);
	
	//==================================================================
	//		## animation렌더 ## 
	//==================================================================
	void aniRender(ID2D1RenderTarget* RenderTarget, int destX, int destY, animation* ani);

	//해제
	void release(void);
	TCHAR* getStr() { return _fileName; }
	ID2D1Bitmap* getBitmap() { return _bitmap; }
	
	//==================================================================
	//		## inline ## (인라인 함수들 - 겟터, 셋터)
	//==================================================================
	//이미지 x좌표
	inline float getX(void) { return _imageInfo->x; }
	inline void setX(float x) { _imageInfo->x = x; }

	//이미지 y좌표
	inline float getY(void) { return _imageInfo->y; }
	inline void setY(float y) { _imageInfo->y = y; }

	//이미지 x좌표
	inline void setWidth(float width) { _imageInfo->width = width; }

	//이미지 y좌표
	inline void setHeight(float height) { _imageInfo->height = height; }

	//이미지 센터좌표
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//이미지 가로, 세로크기
	inline float getWidth(void) { return _imageInfo->width; }
	inline float getHeight(void) { return _imageInfo->height; }

	//바운딩박스 (충돌용)
	inline RECT boudingBox(void)
	{
		RECT rc = { (int)_imageInfo->x, (int)_imageInfo->y,
			(int)_imageInfo->x + _imageInfo->width,
			(int)_imageInfo->y + _imageInfo->height };

		return rc;
	}
	inline RECT boundingBoxWithFrame(void)
	{
		RECT rc = { (int)_imageInfo->x, (int)_imageInfo->y,
			(int)_imageInfo->x + _imageInfo->frameWidth,
			(int)_imageInfo->y + _imageInfo->frameHeight };

		return rc;
	}

	//프레임 X
	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}

	//프레임 Y
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }
	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}

	//1프레임 가로, 세로 크기
	inline float getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline float getFrameHeight(void) { return _imageInfo->frameHeight; }

	//맥스프레임 X, Y
	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	float getAlpha() { return _alpha; }
	void setAlpha(float alpha) { _alpha = alpha; }
};