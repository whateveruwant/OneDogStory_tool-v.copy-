#include "stdafx.h"
#include "loading.h"


loading::loading() {}
loading::~loading() {}

HRESULT loading::init()
{
	backGround = IMAGEMANAGER->addImage(Rt, L"loadingImage", L"images/UI/back.png", 0, 0, WINSIZEX_NUM, WINSIZEY_NUM, false);
	currentGauge = 0;

	return S_OK;
}

void loading::release()
{
}

void loading::update()
{
}

void loading::render()
{
	IMAGEMANAGER->render(L"loadingImage", Rt);
}

void loading::loadImage(tstring keyName, const TCHAR * fileName, float x, float y, int width, int height, bool isCenter)
{
	loadItem* item = new loadItem;
	item->initForImage(keyName, fileName, x, y, width, height, isCenter);

	vLoadItem.push_back(item);
}

void loading::loadFrameImage(tstring keyName, const TCHAR * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter)
{
	loadItem* item = new loadItem;
	item->initForFrameImage(keyName, fileName, x, y, width, height, frameX, frameY, isCenter);

	vLoadItem.push_back(item);
}

bool loading::loadingDone()
{
	if (currentGauge >= vLoadItem.size()) return true;

	loadItem* item = vLoadItem[currentGauge];
	tagImageResource img = item->getImageResource();

	switch (item->getLoadingKind())
	{
	case LOAD_KIND_IMAGE_0:
		IMAGEMANAGER->addImage(Rt, img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.isCenter);
		break;
	case LOAD_KIND_IMAGE_1:
		break;
	case LOAD_KIND_FRAMEIMAGE_0:
		IMAGEMANAGER->addFrameImage(Rt, img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.frameX, img.frameY, img.isCenter);
		break;
	case LOAD_KIND_SOUND:

		break;
	case LOAD_KIND_END:
		break;
	}

	// Ä«¿îÆ®
	currentGauge++;

	return false;
}



HRESULT loadItem::initForImage(tstring keyName, const TCHAR * fileName, float x, float y, int width, int height, bool isCenter)
{
	kind = LOAD_KIND_IMAGE_0;
	memset(&imageResource, 0, sizeof(tagImageResource));

	imageResource.keyName = keyName;
	imageResource.fileName = fileName;
	imageResource.x = x;
	imageResource.y = y;
	imageResource.width = width;
	imageResource.height = height;
	imageResource.isCenter = isCenter;

	return S_OK;
}

HRESULT loadItem::initForFrameImage(tstring keyName, const TCHAR * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter)
{
	kind = LOAD_KIND_FRAMEIMAGE_0;
	memset(&imageResource, 0, sizeof(tagImageResource));

	imageResource.keyName = keyName;
	imageResource.fileName = fileName;
	imageResource.x = x;
	imageResource.y = y;
	imageResource.width = width;
	imageResource.height = height;
	imageResource.frameX = frameX;
	imageResource.frameY = frameY;
	imageResource.isCenter = isCenter;

	return S_OK;
}