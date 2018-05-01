#include "stdafx.h"
#include "effect.h"

effect::effect()
	:m_effectImage(nullptr)
	, m_effectAni(nullptr)
	, m_isRunnig(NULL)
	, m_x(0)
	, m_y(0)
{
}

effect::~effect()
{
}

HRESULT effect::init(image * effectImage, int frameW, int frameH, int FPS, float elapsedTime)
{
	//이펙트 이미지가 없을때 바로 리턴
	if (!effectImage) return E_FAIL;

	m_effectImage = effectImage;
	m_elapsedTime = elapsedTime;
	m_isRunnig = false;

	//이펙트 애니메이션 없으면 새로 생성!
	if (!m_effectAni) m_effectAni = new animation;
	//애니메이션 정보 초기화
	m_effectAni->init(m_effectImage->getWidth(), m_effectImage->getHeight(), frameW, frameH);
	m_effectAni->setDefPlayFrame();
	m_effectAni->setFPS(FPS);

	return S_OK;
}

void effect::release(void)
{
	SAFE_DELETE(m_effectAni);
}

void effect::update(void)
{
	if (m_isRunnig)
	{
		m_effectAni->frameUpdate(m_elapsedTime);
	}

	//이펙트 애니메이션이 종료가 되면 멈추기
	if (!m_effectAni->isPlay()) stopEffect();
}

void effect::render(void)
{
	if (m_isRunnig)
	{
		m_effectImage->aniRender(Rt, m_x, m_y, m_effectAni);
	}
}

void effect::startEffect(int x, int y)
{
	//위치 좌표를 선터 좌표로 만들기
	m_x = x - (m_effectAni->getFrameWidth() / 2);
	m_y = y - (m_effectAni->getFrameHeight() / 2);

	m_isRunnig = true;
	m_effectAni->start();

}

void effect::stopEffect()
{
	m_isRunnig = false;
}