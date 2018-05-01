#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//timeGetTime �Լ��� ����ϱ� ���� ���̺귯�� �߰�
//�����찡 ���۵ǰ� ������ �ð��� ����

timer::timer()
{
}
timer::~timer()
{
}

//Ÿ�̸� �ʱ�ȭ
HRESULT timer::init(void)
{
	//���� Ÿ�̸� �������� üũ�Ѵ�
	//���� Ÿ�̸Ӹ� �����ϸ� �ʴ� ����ũ�μ�������� ���� ����(1/1000000)
	//�ð� ���� �Լ�
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		//LARGE_INTEGER __int64 �ڷ����� �����ϱ� ����
		//64��Ʈ ������ ����� ���ֵ��� ������ ������ ����ũ�μ����带 (1/1000000)��
		//���� �����Ͽ� �� �� ��Ȯ�� �ð����� �����ϱ� ����
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

		//�ʴ� �ð� ��� ����
		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		_isHardware = false;
		//�и������� ������ �ð����� �ʴ� 1000�� ī��Ʈ �Ѵ�
		_lastTime = timeGetTime();
		_timeScale = 0.001f;
	}

	//Ÿ�̸� ���� ������ �ʱ�ȭ
	_frameRate = 0;			//FPS
	_FPSFrameCount = 0;		//FPSī��Ʈ
	_FPSTimeElapsed = 0.0f;	//�������Ӵ� ����ð�(1 / 60)
	_worldTime = 0.0f;		//��ü ����ð�

	return S_OK;
}

//����ð� ���
void timer::tick(float lockFPS)
{
	//���� Ÿ�̸Ӹ� �����Ѵٸ�
	if (_isHardware)
	{
		//������ ����ũ���� ������ ����Ѵ�(1/1000000)
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		//�������� ������ time�Լ��� �̿��Ѵ�(�и������� 1/1000)
		_curTime = timeGetTime();
	}

	//������ �ð��� ����ð��� �����
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lockFPS > 0.0f)
	{
		//���� �������� �ð��� �����Ҷ����� �������Ѷ�
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			//������ �ð��� ����ð��� �����
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;				//������ �ð� ����Ѵ�
	_FPSFrameCount++;					//�ʴ� ������ ī��Ʈ����
	_FPSTimeElapsed += _timeElapsed;	//�ʴ� ������ �ð� ����� ����
	_worldTime += _timeElapsed;			//��ü�ð� ����� ����

										//������ �ʱ�ȭ�� 1�ʸ��� �����Ѵ�
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
