#pragma once
//==================================================================
//		## timer ## (Ÿ�̸� Ŭ����)
//==================================================================

class timer
{
private:
	bool _isHardware;				//���� Ÿ�̸Ӹ� �����ϳ�?
	float _timeScale;				//�ð� �����
	float _timeElapsed;				//�������Ӵ� ����ð�(1 / 60)
	__int64	_curTime;				//����ð�
	__int64 _lastTime;				//�������ð�
	__int64 _periodFrequency;		//�ð��ֱ�

	unsigned long _frameRate;		//FPS
	unsigned long _FPSFrameCount;	//FPSī���
	float _FPSTimeElapsed;			//FPS �������ð��� ����ð��� �����
	float _worldTime;				//��ü ����ð�

public:
	timer();
	~timer();

	//Ÿ�̸� �ʱ�ȭ
	HRESULT init(void);

	//����ð� ���
	void tick(float lockFPS = 0.0f);

	//���� FPS ��������
	unsigned long getFrameRate(wchar_t* str = nullptr) const;
	
	//�������Ӵ� ����ð�(1 / 60) ��������
	inline float getElapsedTime(void) const { return _timeElapsed; }
	
	//��ü ����ð� ��������
	inline float getWorldTime(void) const { return _worldTime; }
	
	inline unsigned long getFPS() { return _frameRate; }
};