#include "stdafx.h"
#include "iniData.h"

iniData::iniData()
{
}

iniData::~iniData()
{
}
HRESULT iniData::init()
{
	return S_OK;
}

//������ �߰��ϱ�(����,Ű,����)
void iniData::addData(tchar* section, tchar* key, tchar* value)
{
	//ini������ ����ü ����, �ʱ�ȭ
	tagIniData iniData;
	iniData.section = section;
	iniData.key = key;
	iniData.value = value;

	//ini������ ����ü ���Ϳ� ���
	arrIniData vIniData;
	vIniData.push_back(iniData);

	//��� �����ʹ� _vIniData�� ����ü�� ���� ���͸� ���
	_vIniData.push_back(vIniData);
}

//���̺�-> �����̸��� �־ (�����̸�.ini)���Ϸ� �ڵ� ����
void iniData::iniSave(tchar* fileName)
{
	tchar str[256];
	tchar dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	swprintf_s(dir, TEXT("\\%s.ini"), fileName);

	GetCurrentDirectory(256, str);
	wcscat_s(str, 256, dir);

	for (int i = 0; i < _vIniData.size(); i++)
	{
		arrIniData vData = _vIniData[i];

		WritePrivateProfileString(
			vData[0].section,		//������
			vData[0].key,			//Ű��(�������)
			vData[0].value,			//Ű��(�������)�� �ۼ��� ����
			str);					//ini���� ���
		vData.clear();
	}
	_vIniData.clear();
}

//���ڰ� �ε��ϱ�(�����̸�,����,Ű)
tchar* iniData::loadDataString(tchar* fileName, tchar* section, tchar* key)
{
	tchar str[256];
	tchar dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	swprintf_s(dir, TEXT("\\%s.ini"), fileName);

	GetCurrentDirectory(256, str);
	wcscat_s(str, 256, dir);

	tchar data[64] = { NULL };

	GetPrivateProfileString(
		section,			//������
		key,				//Ű��
		L"",					//�����̸��̳� Ű�̸��� ã�� ���ϸ� ����Ʈ�� ����� ��Ʈ��
		data,				//Ű������ ����Ű�� ���� ������ ����
		64,					//Ű���� ������ ���� ������
		str);				//ini���� ���

	return data;
}

//������ �ε��ϱ�(�����̸�,����,Ű)
int iniData::loadDataInteger(tchar* fileName, tchar* section, tchar* key)
{
	tchar str[256];
	tchar dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	swprintf_s(dir, TEXT("\\%s.ini"), fileName);

	GetCurrentDirectory(256, str);
	wcscat_s(str, 256, dir);

	return GetPrivateProfileInt(section, key, 0, str);
}