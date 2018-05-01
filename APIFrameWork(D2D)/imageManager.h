#pragma once
#include "SingletonBase.h"
#include "image.h"

//==================================================================
//		## imageManager ## (�̹��� �Ŵ���)
//==================================================================
class imageManager : public singletonBase <imageManager>
{
private:
	typedef map<tstring, image*> mapImageList;
	typedef map<tstring, image*>::iterator mapImageIter;

private:
	mapImageList m_imageList;	// ������ ������ �̹��� ���

public:
	imageManager();
	~imageManager();

	// �̹��� �Ŵ��� �ʱ�ȭ
	HRESULT init(void);
	// �̹��� �Ŵ��� ����
	void release(void);

	// Ű������ �̹��� ���� �ʱ�ȭ
	image* addImage(ID2D1RenderTarget* RenderTarget, tstring strKey, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// Ű������ ������ �̹��� ���� �ʱ�ȭ
	image* addFrameImage(ID2D1RenderTarget* RenderTarget, tstring strKey, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);


	// �̹��� Ű���� ã��
	image* findImage(tstring strKey);
	bool searchImage(tstring strKey);
	// �̹��� Ű������ ����
	bool deleteImage(tstring strKey);
	// �̹��� ��ü ����
	bool deleteAll(void);

	//==================================================================
	//		## �Ϲݷ��� ##
	//==================================================================
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget);
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float x, float y);
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float destX, float destY, float destWidth, float destHeight, float sorceX, float sorceY, float sourceWidth, float sourceHeight);
	void renderByMask(tstring strKey, ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## �Ϲݷ��� ##
	//==================================================================
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float magnification);

	//==================================================================
	//		## �Ϲݷ��� ##
	//==================================================================
	void render(tstring strKey, ID2D1RenderTarget* RenderTarget, float x, float y, float magnification);

	//==================================================================
	//		## ���ķ��� ##
	//==================================================================
	void alphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, float alpha);
	void alphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, float alpha, float x, float y, float magnification = 1.0f);

	//==================================================================
	//		## �����ӷ��� ##
	//==================================================================
	void frameRender(tstring strKey, ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);

	//==================================================================
	//		## ���������ӷ��� ##
	//==================================================================
	void frameAlphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY, float alpha);

	//==================================================================
	//		## bottom�Ϲݷ��� ##
	//==================================================================
	void bottomRender(tstring strKey, ID2D1RenderTarget* RenderTarget);

	//==================================================================
	//		## bottom���ķ��� ##
	//==================================================================
	void bottomAlphaRender(tstring strKey, ID2D1RenderTarget* RenderTarget, float alpha);

	//==================================================================
	//		## bottom�����ӷ��� ##
	//==================================================================
	void  bottomFrameRender(tstring strKey, ID2D1RenderTarget* RenderTarget, int currentFrameX, int currentFrameY);

	//==================================================================
	//		## bottom���������ӷ��� ##
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