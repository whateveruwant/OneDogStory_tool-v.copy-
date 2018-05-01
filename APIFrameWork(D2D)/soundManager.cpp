#include "stdafx.h"
#include "soundManager.h"

soundManager::soundManager() : _system(NULL), _channel(NULL), _sound(NULL)
{
}

soundManager::~soundManager()
{
}

HRESULT soundManager::init(void)
{
	//���� �ý��� ����
	System_Create(&_system);

	//maxchannels : fomd ���� ����ϴ� �ִ� ä���� ���� ����
	//���� ä�μ� ����
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//���� , ä�� �����Ҵ�
	_sound = new Sound *[TOTALSOUNDBUFFER];
	_channel = new Channel *[TOTALSOUNDBUFFER];

	//�޸� �ʱ�ȭ
	memset(_sound, 0, sizeof(Sound*)* (TOTALSOUNDBUFFER));
	memset(_channel, 0, sizeof(Channel*)* (TOTALSOUNDBUFFER));

	return S_OK;
}
void soundManager::release(void)
{
	//���� �� ä�� ����
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//�޸� �����
	SAFE_DELETE(_channel);
	SAFE_DELETE(_sound);

	//�ý��� �ݱ�
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void soundManager::update(void)
{
	/*
	���� �ý����� ��� ������Ʈ ����� ������ �ٲ�ų�
	�缺�� ���� ���带 ä�ο��� ������ �� �پ��� �۾��� �ڵ����� ���ش�
	*/
	//���ΰ��ӿ��� ����Ŵ����� ������Ʈ ���ش�
	_system->update();
}

void soundManager::render(void)
{
}

//���� �߰�(Ű��, �����̸�, BGM?, ������?)
void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	if (loop)//��� or ����Ʈ
	{
		if (bgm)
		{
			//1: ���� URL�� �̸� 
			//2: ���带 ���� ���� �ɼ�
			//3: ���带 ����ϴ� ���� �������� ������ �����ϴ� FMOD_CREATESOUNDEXINFO�� ������ , �̿ɼ��� �����Ϸ��� 0 �Ǵ� NULL�� �����ϸ� ��
			//4: FMOD::SOUND ������Ʈ�� �޴� ������ �ּ�
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}

	else  //����Ʈ
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� ����ش�
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

//���� �÷���(Ű��, ����) 0.0f~ 1.0f
void soundManager::play(string keyName, float volume)
{
	bool playing;

	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			for (int i = 0; i < TOTALSOUNDBUFFER; i++)
			{
				_channel[i]->isPlaying(&playing);
				if (playing) continue;

				//���� �÷���
				_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[i]);
				//��������
				_channel[i]->setVolume(volume);
				break;
			}

		}
	}
}
void soundManager::bgmPlay(string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//��������
			_channel[count]->setVolume(volume);
		}
	}
}

//���� ����
void soundManager::stop(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� ����
			_channel[count]->stop();
			break;
		}
	}
}

//���� �Ͻ�����
void soundManager::pause(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� ����
			_channel[count]->setPaused(true);
			break;
		}
	}
}

//���� �ٽ� ���
void soundManager::resume(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �Ͻ�����(�ٽ����)
			_channel[count]->setPaused(false);
			break;
		}
	}
}

//�÷��� ���̳�?
bool soundManager::isPlaySound(string keyName)
{
	bool isPlay = false;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//�÷��� ���̳�?
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

//�Ͻ����� ���̳�?
bool soundManager::isPauseSound(string keyName)
{
	bool isPause = false;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//�÷��� ���̳�? (�Ͻ�����)
			_channel[count]->isPlaying(&isPause);
			break;
		}
	}
	return isPause;
}