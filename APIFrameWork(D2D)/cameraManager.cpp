#include "stdafx.h"
#include "cameraManager.h"


cameraManager::cameraManager()
{
}


cameraManager::~cameraManager()
{
}

HRESULT cameraManager::init()
{
	return S_OK;
}

void cameraManager::release()
{
}

void cameraManager::update()
{
}

void cameraManager::render()
{
}

D2D1_POINT_2F cameraManager::getAbsPosition(const D2D1_POINT_2F _pt)
{
	D2D1_POINT_2F tmp;
	tmp.x = _pt.x - _standard->x + WINSIZEX_NUM / 2;
	tmp.y = _pt.y - _standard->y + WINSIZEY_NUM / 2;
	return tmp;
}