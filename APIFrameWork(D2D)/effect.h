#pragma once
#include "gameNode.h"

class effect : public gameNode
{
private:
	image* m_effectImage;	//이펙트 이미지
	animation* m_effectAni;	//이펙트 애니메이션

	int m_x, m_y;	//이펙트 터트릴 좌표
	float m_elapsedTime;	//이펫트 경과시간(속도)
	bool m_isRunnig;		//이펙트 재생중이냐?

public:
	effect();
	~effect();

	//이펙트 초기화(이펙트 이미지, 이펙트 한프레임 가로길이, 세로길이, FPS(1), 이펙트 속도)
	HRESULT init(image* effectImage, int frameW, int frameH, int FPS, float elapsedTime);
	void release(void);
	void update(void);
	void render(void);

	//내가 원하는 좌표에 이펙트터트리기
	void startEffect(int x, int y);
	//이펙트 멈추기
	void stopEffect();
	//이펙트 재생중인지 상태값 가져오기
	bool getIsRunning() { return m_isRunnig; }
};