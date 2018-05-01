#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//timeGetTime 함수를 사용하기 위한 라이브러리 추가
//윈도우가 시작되고 난뒤의 시간을 리턴

timer::timer()
{
}
timer::~timer()
{
}

//타이머 초기화
HRESULT timer::init(void)
{
	//고성능 타이머 지원여부 체크한다
	//고성능 타이머를 지원하면 초당 마이크로세컨드까지 값이 들어간다(1/1000000)
	//시간 측정 함수
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		//LARGE_INTEGER __int64 자료형을 지원하기 위해
		//64비트 정수를 사용할 수있도록 정의한 이유는 마이크로세컨드를 (1/1000000)초
		//까지 측정하여 좀 더 정확한 시간값을 저장하기 위함
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

		//초당 시간 계산 범위
		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		_isHardware = false;
		//밀리세컨드 단위의 시간으로 초당 1000번 카운트 한다
		_lastTime = timeGetTime();
		_timeScale = 0.001f;
	}

	//타이머 관련 변수들 초기화
	_frameRate = 0;			//FPS
	_FPSFrameCount = 0;		//FPS카운트
	_FPSTimeElapsed = 0.0f;	//한프레임당 경과시간(1 / 60)
	_worldTime = 0.0f;		//전체 경과시간

	return S_OK;
}

//현재시간 계산
void timer::tick(float lockFPS)
{
	//고성능 타이머를 지원한다면
	if (_isHardware)
	{
		//초정밀 마이크로초 단위로 계산한다(1/1000000)
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		//지원하지 않으면 time함수를 이용한다(밀리세컨드 1/1000)
		_curTime = timeGetTime();
	}

	//마지막 시간과 현재시간의 경과량
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lockFPS > 0.0f)
	{
		//고정 프레임의 시간을 만족할때까지 루프시켜라
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			//마지막 시간과 현재시간의 경과량
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;				//마지막 시간 기록한다
	_FPSFrameCount++;					//초당 프레임 카운트증가
	_FPSTimeElapsed += _timeElapsed;	//초당 프레임 시간 경과량 증가
	_worldTime += _timeElapsed;			//전체시간 경과량 증가

										//프레임 초기화를 1초마다 진행한다
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}

unsigned long timer::getFrameRate(wchar_t * str) const
{
	if (str != nullptr) {
		wsprintf(str, L"FPS: %d", _frameRate);
	}
	return _frameRate;
}
