#pragma once
#include "singletonBase.h"
#include "timer.h"
//==================================================================
//		## timeManager ## (타임매니져)
//==================================================================

class timeManager : public singletonBase <timeManager>
{
private:
	timer* _timer;	//타이머 클래스

public:
	HRESULT init(void);
	void release(void);
	void update(float lock = 0.0f);
	void render(ID2D1RenderTarget* RenderTarget);

	//한프레임당 경과시간(1 / 60) 가져오기
	float getElapsedTime(void) const { return _timer->getElapsedTime(); }
	//전체 경과시간 가져오기
	float getWorldTime(void) const { return _timer->getWorldTime(); }
	unsigned long getFPS() { return _timer->getFrameRate(); }

	timeManager();
	~timeManager();
};