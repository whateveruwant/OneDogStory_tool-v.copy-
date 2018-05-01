#pragma once
#include "gameNode.h"

enum LOAD_KIND {
	LOAD_KIND_IMAGE_0,
	LOAD_KIND_IMAGE_1,
	LOAD_KIND_FRAMEIMAGE_0,
	LOAD_KIND_SOUND,
	LOAD_KIND_END
};

struct tagImageResource {
	tstring keyName;			// �̹��� Ű��
	const TCHAR* fileName;	// ���� �̸�
	int x, y;				// ��ġ ��ǥ
	int width, height;		// ���μ��� ũ��
	int frameX, frameY;		// ������ xy
	bool isCenter;
};

class loadItem {
private:
	LOAD_KIND kind;
	tagImageResource imageResource;

public:
	loadItem() {}
	~loadItem() {}

	// Ű������ �̹��� ���� �ʱ�ȭ
	HRESULT initForImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// Ű������ ������ �̹��� ���� �ʱ�ȭ
	HRESULT initForFrameImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);

	// �ε� ������ ���� ��������
	LOAD_KIND getLoadingKind() { return kind; }
	tagImageResource getImageResource() { return imageResource; }

};

class loading : public gameNode
{
private:
	typedef vector<loadItem*> arrLoadItem;

private:
	arrLoadItem vLoadItem;

	// �ε�ȭ�鿡�� ����� �̹��� �� �ε���
	image* backGround;
	int currentGauge;

public:
	loading();
	~loading();

	HRESULT init();
	void release();
	void update();
	void render();

	// Ű������ �̹��� ���� �ʱ�ȭ
	void loadImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, bool isCenter);
	// Ű������ ������ �̹��� ���Ϸ� �ʱ�ȭ
	void loadFrameImage(tstring keyName, const TCHAR* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isCenter);

	// �ε� �Ϸ�
	bool loadingDone();

	// �ε� ������ ���� ��������
	arrLoadItem getLoadItem() { return vLoadItem; }
};