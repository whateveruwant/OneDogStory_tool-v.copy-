#pragma once
//==================================================================
//		## timer ## (타이머 클래스)
//==================================================================

class timer
{
private:
	bool _isHardware;				//고성능 타이머를 지원하냐?
	float _timeScale;				//시간 경과량
	float _timeElapsed;				//한프레임당 경과시간(1 / 60)
	__int64	_curTime;				//현재시간
	__int64 _lastTime;				//마지막시간
	__int64 _periodFrequency;		//시간주기

	unsigned long _frameRate;		//FPS
	unsigned long _FPSFrameCount;	//FPS카운드
	float _FPSTimeElapsed;			//FPS 마지막시간과 현재시간의 경과량
	float _worldTime;				//전체 경과시간

public:
	timer();
	~timer();

	//타이머 초기화
	HRESULT init(void);

	//현재시간 계산
	void tick(float lockFPS = 0.0f);

	//현재 FPS 가져오기
	unsigned long getFrameRate(wchar_t* str = nullptr) const;
	
	//한프레임당 경과시간(1 / 60) 가져오기
	inline float getElapsedTime(void) const { return _timeElapsed; }
	
	//전체 경과시간 가져오기
	inline float getWorldTime(void) const { return _worldTime; }
	
	inline unsigned long getFPS() { return _frameRate; }
};