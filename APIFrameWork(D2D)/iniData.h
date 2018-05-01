#pragma once
#include "singletonBase.h"

//ini������ ����ü
struct tagIniData
{
	tchar* section;
	tchar* key;
	tchar* value;
};

class iniData : public singletonBase <iniData>
{
private:
	typedef vector<tagIniData> arrIniData;
	typedef vector<tagIniData>::iterator arrIniDataIter;
	typedef vector<arrIniData> arrIniDatas;
	typedef vector<arrIniData>::iterator arrIniDatasIter;

private:
	arrIniDatas _vIniData;

public:
	iniData();
	~iniData();

	HRESULT init();

	//������ �߰��ϱ�(����,Ű,����)
	void addData(tchar* section, tchar* key, tchar* value);
	//���̺�-> �����̸��� �־ (�����̸�.ini)���Ϸ� �ڵ� ����
	void iniSave(tchar* fileName);

	//���ڰ� �ε��ϱ�(�����̸�,����,Ű)
	tchar* loadDataString(tchar* fileName, tchar* section, tchar* key);
	//������ �ε��ϱ�(�����̸�,����,Ű)
	int loadDataInteger(tchar* fileName, tchar* section, tchar* key);
};