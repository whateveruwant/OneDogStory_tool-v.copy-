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
	//����Ʈ �̹����� ������ �ٷ� ����
	if (!effectImage) return E_FAIL;

	m_effectImage = effectImage;
	m_elapsedTime = elapsedTime;
	m_isRunnig = false;

	//����Ʈ �ִϸ��̼� ������ ���� ����!
	if (!m_effectAni) m_effectAni = new animation;
	//�ִϸ��̼� ���� �ʱ�ȭ
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

	//����Ʈ �ִϸ��̼��� ���ᰡ �Ǹ� ���߱�
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
	//��ġ ��ǥ�� ���� ��ǥ�� �����
	m_x = x - (m_effectAni->getFrameWidth() / 2);
	m_y = y - (m_effectAni->getFrameHeight() / 2);

	m_isRunnig = true;
	m_effectAni->start();

}

void effect::stopEffect()
{
	m_isRunnig = false;
}