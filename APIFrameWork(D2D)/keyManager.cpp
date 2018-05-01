#include "stdafx.h"
#include "keyManager.h"
keyManager::keyManager()
{
}
keyManager::~keyManager()
{
}
//==================================================================
//		## ���̻� ������, �Ҹ��ڴ� ������� �ʴ´� ##
//==================================================================

//Ű�Ŵ��� �ʱ�ȭ
HRESULT keyManager::init(void)
{
	//Ű���� ���� �������� ���� ���·� �ʱ�ȭ �Ѵ�
	for (int i = 0; i < KEYMAX; i++)
	{
		_keyUp.set(i, false);
		_keyDown.set(i, false);
	}

	return S_OK;
}

//Ű�� �ѹ��� ���ȳ�?
bool keyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!_keyDown[key])
		{
			_keyDown.set(key, true);
			return true;
		}
	}
	else
	{
		_keyDown.set(key, false);
	}

	return false;
}

//Ű�� �ѹ� ������ �����?
bool keyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		_keyUp.set(key, true);
	}
	else
	{
		if (_keyUp[key])
		{
			_keyUp.set(key, false);
			return true;
		}
	}

	return false;
}

//Ű�� ��� ���� �ֳ�?
bool keyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//Ű�� ����̳�?
bool keyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}