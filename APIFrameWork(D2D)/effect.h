#pragma once
#include "gameNode.h"

class effect : public gameNode
{
private:
	image* m_effectImage;	//����Ʈ �̹���
	animation* m_effectAni;	//����Ʈ �ִϸ��̼�

	int m_x, m_y;	//����Ʈ ��Ʈ�� ��ǥ
	float m_elapsedTime;	//����Ʈ ����ð�(�ӵ�)
	bool m_isRunnig;		//����Ʈ ������̳�?

public:
	effect();
	~effect();

	//����Ʈ �ʱ�ȭ(����Ʈ �̹���, ����Ʈ �������� ���α���, ���α���, FPS(1), ����Ʈ �ӵ�)
	HRESULT init(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime);
	void release(void);
	void update(void);
	void render(void);

	//���� ���ϴ� ��ǥ�� ����Ʈ��Ʈ����
	void startEffect(int x, int y);
	//����Ʈ ���߱�
	void stopEffect();
	//����Ʈ ��������� ���°� ��������
	bool getIsRunning() { return m_isRunnig; }
};