#include "stdafx.h"
#include "animation.h"
#pragma once
class image
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE = 0,		//���ҽ��� �ε�
		LOAD_FILE,				//���Ϸ� �ε�
		LOAD_EMPTY,				//���Ʈ�� �ε�
		LOAD_END
	};

	typedef struct tagImage
	{
		DWORD		resID;			//���ҽ� ID	
		float		x;				//�̹��� x��ǥ
		float		y;				//�̹��� y��ǥ
		float		width;			//�̹��� ����ũ��
		float		height;			//�̹��� ����ũ��
		int			currentFrameX;	//���� ������ X
		int			currentFrameY;	//���� ������ Y
		int			maxFrameX;		//�ִ� X������ ����
		int			maxFrameY;		//�ִ� Y������ ����
		float		frameWidth;		//1������ ���α���
		float		frameHeight;	//1������ ���α���
		BYTE		loadType;		//�̹��� �ε�Ÿ��

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
	LPIMAGE_INFO	_imageInfo;		//�̹��� ����
	float _alpha;
	bool _isCenter;

public:
	image();
	~image();

	// �̹��� ���� �ʱ�ȭ
	HRESULT init(ID2D1RenderTarget* RenderTarget, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// ������ �̹��� ���� �ʱ�ȭ
	HRESULT init(ID2D1RenderTarget* RenderTarget, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);

	//==================================================================
	//		## �Ϲݷ��� ##
	//==================================================================
	void render(ID2D1RenderTarget* RenderTarget);
	void render(ID2D1RenderTarget* RenderTarget, float x, float y);
	void render(ID2D1RenderTarget* RenderTarget, float destX, float destY, float destWidth, float destHeight, float sorceX, float sorceY, float sourceWidth, float sourceHeight);
	void renderByMask(ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## �Ϲݷ���(����) ##
	//==================================================================
	void magnificateRender(ID2D1RenderTarget* RenderTarget, float magnification);
	void magnificateRender(ID2D1RenderTarget* RenderTarget, float magnification, float x, float y);

	//==================================================================
	//		## ���ķ��� ##
	//==================================================================
	void alphaRender(ID2D1RenderTarget* RenderTarget, float alpha);
	void alphaRender(ID2D1RenderTarget* RenderTarget, float alpha, float magnification, float x, float y);
	
	//==================================================================
	//		## �����ӷ��� ##
	//==================================================================
	void frameRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);
	
	//==================================================================
	//		## ���������ӷ��� ##
	//==================================================================
	void frameAlphaRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY, float alpha);

	//==================================================================
	//		## bottom�Ϲݷ��� ##
	//==================================================================
	void bottomRender(ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## bottom���ķ��� ##
	//==================================================================
	void bottomAlphaRender(ID2D1RenderTarget* RenderTarget, float alpha);

	//==================================================================
	//		## bottom�����ӷ��� ##
	//==================================================================
	void bottomFrameRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);

	//==================================================================
	//		## bottom���������ӷ��� ##
	//==================================================================
	void bottomFrameAlphaRender(ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY, float alpha);
	
	//==================================================================
	//		## animation���� ## 
	//==================================================================
	void aniRender(ID2D1RenderTarget* RenderTarget, int destX, int destY, animation* ani);

	//����
	void release(void);
	TCHAR* getStr() { return _fileName; }
	ID2D1Bitmap* getBitmap() { return _bitmap; }
	
	//==================================================================
	//		## inline ## (�ζ��� �Լ��� - ����, ����)
	//==================================================================
	//�̹��� x��ǥ
	inline float getX(void) { return _imageInfo->x; }
	inline void setX(float x) { _imageInfo->x = x; }

	//�̹��� y��ǥ
	inline float getY(void) { return _imageInfo->y; }
	inline void setY(float y) { _imageInfo->y = y; }

	//�̹��� x��ǥ
	inline void setWidth(float width) { _imageInfo->width = width; }

	//�̹��� y��ǥ
	inline void setHeight(float height) { _imageInfo->height = height; }

	//�̹��� ������ǥ
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//�̹��� ����, ����ũ��
	inline float getWidth(void) { return _imageInfo->width; }
	inline float getHeight(void) { return _imageInfo->height; }

	//�ٿ���ڽ� (�浹��)
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

	//������ X
	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}

	//������ Y
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }
	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}

	//1������ ����, ���� ũ��
	inline float getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline float getFrameHeight(void) { return _imageInfo->frameHeight; }

	//�ƽ������� X, Y
	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	float getAlpha() { return _alpha; }
	void setAlpha(float alpha) { _alpha = alpha; }
};