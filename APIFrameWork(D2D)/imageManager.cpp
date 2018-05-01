#include "stdafx.h"
#include "imageManager.h"

imageManager::imageManager() {}
imageManager::~imageManager() {}

//이미지매니져 초기화
HRESULT imageManager::init(void)
{
	return S_OK;
}

//이미지매니져 해제
void imageManager::release(void)
{
	deleteAll();
}

//키값으로 이미지 파일로 초기화
image * imageManager::addImage(ID2D1RenderTarget * RenderTarget, tstring strKey, const TCHAR * fileName, float x, float y, int width, int height, bool isCenter)
{
	//추가 하려는 키 값으로 이미지가 존재하는지 확인
	image* img = findImage(strKey);

	//추가 하려는 이미지가 이미 존재한다면 새로 만들지 않고 이미지를 리턴시킨다
	if (img) return img;

	// 이미지 새로 생성
	img = new image;
	// 이미지가 제대로 초기화가 되지 않았다면 NULL 리턴
	if (FAILED(img->init(RenderTarget, fileName, x, y, width, height, isCenter))) {
		SAFE_DELETE(img);
		return NULL;
	}

	// 생성된 이미지를 맵으로 만든 이미지 목록에 추가한다.
	m_imageList.insert(make_pair(strKey, img));

	return img;
}

//키값으로 프레임 이미지 파일로 초기화
image * imageManager::addFrameImage(ID2D1RenderTarget * RenderTarget, tstring strKey, const TCHAR * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter)
{
	//추가 하려는 키 값으로 이미지가 존재하는지 확인
	image* img = findImage(strKey);

	//추가 하려는 이미지가 이미 존재한다면 새로 만들지 않고 이미지를 리턴시킨다
	if (img) return img;

	// 이미지 새로 생성
	img = new image;
	// 이미지가 제대로 초기화가 되지 않았다면 NULL 리턴
	if (FAILED(img->init(RenderTarget, fileName, x, y, width, height, frameX, frameY, isCenter))) {
		SAFE_DELETE(img);
		return NULL;
	}

	// 생성된 이미지를 맵으로 만든 이미지 목록에 추가한다.
	m_imageList.insert(make_pair(strKey, img));

	return img;
}

//이미지 키값으로 찾기
image * imageManager::findImage(tstring strKey)
{
	// 해당키 검색
	mapImageIter key = m_imageList.find(strKey);

	// 검색한 키를 찾았다면
	if (key != m_imageList.end()) {
		return key->second;
	}
	// 검색한 키로 이미지를 못 찾았다면
	return NULL;
}

bool imageManager::searchImage(tstring strKey)
{
	// 해당키 검색
	mapImageIter key = m_imageList.find(strKey);

	// 검색한 키를 찾았다면
	if (key != m_imageList.end()) {
		return true;
	}
	// 검색한 키로 이미지를 못 찾았다면
	return false;
}

//이미지 키값으로 삭제
bool imageManager::deleteImage(tstring strKey)
{
	// 해당키 검색
	mapImageIter key = m_imageList.find(strKey);

	//검색한 키를 찾았다면
	if (key != m_imageList.end()) {
		// 이미지 해제
		SAFE_RELEASE(key->second);
		// 메모리 해제
		SAFE_DELETE(key->second);
		// 맵에서 반복자 삭제
		m_imageList.erase(key);

		return true;
	}

	return false;
}

bool imageManager::deleteAll(void)
{
	// 맵 전체를 돌면서 지운다
	mapImageIter iter = m_imageList.begin();
	for (; iter != m_imageList.end();) {
		if (iter->second != NULL) {
			SAFE_RELEASE(iter->second);
			SAFE_DELETE(iter->second);
			iter = m_imageList.erase(iter);
		}
		else {
			++iter;
		}
	}

	// 맵 전체 삭제
	m_imageList.clear();

	return true;
}

void imageManager::render(tstring strKey, ID2D1RenderTarget * RenderTarget)
{
	image* img = findImage(strKey);
	if (img) img->render(RenderTarget);
}

void imageManager::renderByMask(tstring strKey, ID2D1RenderTarget * RenderTarget)
{
	image* img = findImage(strKey);
	if (img) img->renderByMask(RenderTarget);
}

void imageManager::render(tstring strKey, ID2D1RenderTarget * RenderTarget, float magnification)
{
	image* img = findImage(strKey);
	if (img) img->magnificateRender(RenderTarget, magnification);
}

void imageManager::render(tstring strKey, ID2D1RenderTarget * RenderTarget, float x, float y)
{
	image* img = findImage(strKey);
	if (img) img->render(RenderTarget, x, y);
}

void imageManager::render(tstring strKey, ID2D1RenderTarget * RenderTarget, float destX, float destY, float destWidth, float destHeight, float sorceX, float sorceY, float sourceWidth, float sourceHeight)
{
	image* img = findImage(strKey);
	if (img) img->render(RenderTarget, destX, destY, destWidth, destHeight, sorceX, sorceY, sourceWidth, sourceHeight);
}

void imageManager::render(tstring strKey, ID2D1RenderTarget * RenderTarget, float x, float y, float magnification)
{
	image* img = findImage(strKey);
	if (img) img->magnificateRender(RenderTarget, magnification, x, y);
}

void imageManager::alphaRender(tstring strKey, ID2D1RenderTarget * RenderTarget, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(RenderTarget, alpha);
}

void imageManager::alphaRender(tstring strKey, ID2D1RenderTarget * RenderTarget, float alpha, float x, float y, float magnification)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(RenderTarget, alpha, magnification, x, y);
}

void imageManager::frameRender(tstring strKey, ID2D1RenderTarget * RenderTarget, int currentFrameX, int currentFrameY)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(RenderTarget, currentFrameX, currentFrameY);
}

void imageManager::frameAlphaRender(tstring strKey, ID2D1RenderTarget * RenderTarget, int currentFrameX, int currentFrameY, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->frameAlphaRender(RenderTarget, currentFrameX, currentFrameY, alpha);
}

void imageManager::bottomRender(tstring strKey, ID2D1RenderTarget * RenderTarget)
{
	image* img = findImage(strKey);
	if (img) img->bottomRender(RenderTarget);
}

void imageManager::bottomAlphaRender(tstring strKey, ID2D1RenderTarget * RenderTarget, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->bottomAlphaRender(RenderTarget, alpha);
}

void imageManager::bottomFrameRender(tstring strKey, ID2D1RenderTarget * RenderTarget, int currentFrameX, int currentFrameY)
{
	image* img = findImage(strKey);
	if (img) img->bottomFrameRender(RenderTarget, currentFrameX, currentFrameY);
}

void imageManager::bottomFrameAlphaRender(tstring strKey, ID2D1RenderTarget * RenderTarget, int currentFrameX, int currentFrameY, float alpha)
{
	image* img = findImage(strKey);
	if (img) img->bottomFrameAlphaRender(RenderTarget, currentFrameX, currentFrameY, alpha);
}
