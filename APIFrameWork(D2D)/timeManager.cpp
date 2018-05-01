#include "stdafx.h"
#include "timeManager.h"
timeManager::timeManager()
{
}
timeManager::~timeManager()
{
}
//==================================================================
//		## �����ڿ� �Ҹ��ڴ� ���̻� ������� �ʴ´� ##
//==================================================================

HRESULT timeManager::init(void)
{
	//Ÿ�̸� Ŭ���� �ʱ�ȭ
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release(void)
{
	//Ÿ�̸� Ŭ������ ������ �����Ѵ�
	if (_timer != NULL)
	{
		SAFE_DELETE(_timer);
		_timer = NULL;
	}
}

void timeManager::update(float lock)
{
	//Ÿ�̸� Ŭ������ ������ ������Ʈ�Ѵ�
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

void timeManager::render(ID2D1RenderTarget* RenderTarget)
{
	wchar_t str[256];
	string strFrame;
	//DWInit(L"�������", 15, &WriteFactory, &TextFormat);
	//TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

	//���ڹ����

	//���ڻ���

	//���� ������ #endif ������ ������ ������
	//#ifdef ���ǽ��� if(���ǽ�)�� ������

#ifdef _DEBUG
	if (_timer != nullptr)
	{
		swprintf_s(str, TEXT("FPS: %ld"), _timer->getFPS());
		D2TextOut(RenderTarget, str, TextFormat, D2D1::RectF(0, 0,200, 0), WhiteBrush);
		swprintf_s(str, TEXT("worldTime: %f"), _timer->getWorldTime());
		D2TextOut(RenderTarget, str, TextFormat, D2D1::RectF(0, 15,200, 15), WhiteBrush);
		swprintf_s(str, TEXT("ElapsedTime: %f"), _timer->getElapsedTime());
		D2TextOut(RenderTarget, str, TextFormat, D2D1::RectF(0, 30, 200, 30), WhiteBrush);
	}
#else
	if (_timer != nullptr)
	{
		swprintf_s(str, TEXT("FPS: %ld"), _timer->getFPS());
		D2TextOut(RenderTarget, str, TextFormat, D2D1::RectF(0, 0, 200, 0), WhiteBrush);
		swprintf_s(str, TEXT("worldTime: %f"), _timer->getWorldTime());
		D2TextOut(RenderTarget, str, TextFormat, D2D1::RectF(0, 15, 200, 15), WhiteBrush);
		swprintf_s(str, TEXT("ElapsedTime: %f"), _timer->getElapsedTime());
		D2TextOut(RenderTarget, str, TextFormat, D2D1::RectF(0, 30, 200, 30), WhiteBrush);
	}
#endif
}