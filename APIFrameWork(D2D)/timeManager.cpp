#include "stdafx.h"
#include "timeManager.h"
timeManager::timeManager()
{
}
timeManager::~timeManager()
{
}
//==================================================================
//		## 생성자와 소멸자는 더이상 사용하지 않는다 ##
//==================================================================

HRESULT timeManager::init(void)
{
	//타이머 클래스 초기화
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release(void)
{
	//타이머 클래스가 있으면 해제한다
	if (_timer != NULL)
	{
		SAFE_DELETE(_timer);
		_timer = NULL;
	}
}

void timeManager::update(float lock)
{
	//타이머 클래스가 있으면 업데이트한다
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

void timeManager::render(ID2D1RenderTarget* RenderTarget)
{
	wchar_t str[256];
	string strFrame;
	//DWInit(L"나눔고딕", 15, &WriteFactory, &TextFormat);
	//TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

	//글자배경모드

	//글자색상

	//조건 컴파일 #endif 나오기 전까지 실행함
	//#ifdef 조건식은 if(조건식)과 동일함

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