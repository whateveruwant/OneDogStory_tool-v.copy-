#include "stdafx.h"
#include "animation.h"

animation::animation()
	:m_frameNum(0), m_frameWidth(0)
	, m_frameHeight(0), m_loop(false),
	m_elapsedSec(0), m_nowPlayIndex(0),
	m_play(false)
{
}


animation::~animation()
{
}

HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	//���� ������ ����
	m_frameWidth = frameW;
	int frameWidthNum = totalW / m_frameWidth;

	//���� ������ ����
	m_frameHeight = frameH;
	int frameHeightNum = totalH / m_frameHeight;

	//�� ������ ��
	m_frameNum = frameWidthNum * frameHeightNum;

	//������ ��ġ ��� ����
	m_frameList.clear();
	for (int i = 0; i < frameHeightNum; i++)
	{
		for (int j = 0; j < frameWidthNum; j++)
		{
			POINT framePos;
			framePos.x = j * m_frameWidth;
			framePos.y = i * m_frameHeight;

			//������ ��ġ ��� �߱⤿
			m_frameList.push_back(framePos);
		}
	}

	//�⺻���������� ����
	setDefPlayFrame();

	return S_OK;
}

void animation::setDefPlayFrame(bool reverse, bool loop)
{
	//���� ����
	m_loop = loop;
	//�÷��� ��� �ʱ�ȭ
	m_playList.clear();

	if (reverse)
	{
		//������ �� 1�� �ε�������
		if (m_loop)
		{
			//������
			for (int i = 0; i < m_frameNum; i++)
			{
				m_playList.push_back(i);
			}
			//������
			for (int i = m_frameNum - 1; i > 0; i--)
			{
				m_playList.push_back(i);
			}
		}

		//������ �� 0�� �ε�������
		else
		{
			//������
			for (int i = 0; i < m_frameNum; i++)
			{
				m_playList.push_back(i);
			}
			//������
			for (int i = m_frameNum - 1; i >= 0; i--)
			{
				m_playList.push_back(i);
			}
		}
	}

	else
	{
		//������
		for (int i = 0; i < m_frameNum; i++)
		{
			m_playList.push_back(i);
		}
	}

}

void animation::setPlayFrame(int * playArr, int arrLen, bool loop)
{
	m_loop = loop;
	//�÷��� ��� �ʱ�ȭ
	m_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		m_playList.push_back(playArr[i]);
	}
}

void animation::setPlayFrame(int start, int end, bool reverse, bool loop)
{
	//���� ����
	m_loop = loop;
	//�÷��� ��� �ʱ�ȭ
	m_playList.clear();

	//���۰� �� �����ӱ����� ������� ����� �ʿ� ����.
	if (start == end)
	{
		m_playList.clear();
		stop();
		return;
	}

	//���� �������� ���� �����Ӻ��� ū ���
	else if (start > end)
	{
		if (reverse)
		{
			//������ �� 1�� �ε��� ������
			if (m_loop)
			{
				//������
				for (int i = start; i >= end; i--)
				{
					m_playList.push_back(i);
				}

				//������
				for (int i = end + 1; i < start; i++)
				{
					m_playList.push_back(i);
				}
			}

			//������ �� 0���ε��� ����
			else
			{
				for (int i = start; i >= end; i--)
				{
					m_playList.push_back(i);
				}
				//������
				for (int i = end + 1; i <= start; i++)
				{
					m_playList.push_back(i);
				}
			}
		}

		else
		{
			for (int i = start; i >= end; i--)
			{
				m_playList.push_back(i);
			}
		}
	}

	//���������� ������������ ���������Ӻ��� ���� ���
	else
	{
		if (reverse)
		{
			//������ �� 1�� �ε��� ������
			if (m_loop)
			{
				for (int i = start; i <= end; i++)
				{
					m_playList.push_back(i);
				}
				//������
				for (int i = end - 1; i > start; i--)
				{
					m_playList.push_back(i);
				}
			}
			else//������ �� 0�� �ε��� ����
			{
				//������
				for (int i = start; i <= end; i++)
				{
					m_playList.push_back(i);
				}
				for (int i = end - 1; i >= start; i--)
				{
					m_playList.push_back(i);
				}
			}
		}

		else
		{
			//������
			for (int i = start; i <= end; i++)
			{
				m_playList.push_back(i);
			}
		}
	}
}

void animation::setFPS(int framePerSec)
{
	m_frameUpdateSec = 1.0f / framePerSec;
}

void animation::frameUpdate(float elapsedTime)
{
	//�÷��� ���̳�?
	if (m_play)
	{
		m_elapsedSec += elapsedTime;
		//������ ������Ʈ �ð��� �Ǿ��ٸ�
		if (m_elapsedSec >= m_frameUpdateSec)
		{
			m_elapsedSec -= m_frameUpdateSec;
			m_nowPlayIndex++;

			if (m_nowPlayIndex == m_playList.size())
			{
				if (m_loop)
				{
					m_nowPlayIndex = 0;
				}
				else
				{
					m_nowPlayIndex--;
					m_play = false;
				}
			}

		}
	}
}

void animation::start()
{
	m_play = true;
	m_nowPlayIndex = 0;
}

void animation::stop()
{
	m_play = false;
	m_nowPlayIndex = 0;
}

void animation::pause()
{
	m_play = false;
}

void animation::resume()
{
	m_play = true;
}