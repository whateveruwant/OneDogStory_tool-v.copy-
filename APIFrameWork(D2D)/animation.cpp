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
	//가로 프레임 갯수
	m_frameWidth = frameW;
	int frameWidthNum = totalW / m_frameWidth;

	//세로 프레임 갯수
	m_frameHeight = frameH;
	int frameHeightNum = totalH / m_frameHeight;

	//총 프레임 수
	m_frameNum = frameWidthNum * frameHeightNum;

	//프레임 위치 목록 셋팅
	m_frameList.clear();
	for (int i = 0; i < frameHeightNum; i++)
	{
		for (int j = 0; j < frameWidthNum; j++)
		{
			POINT framePos;
			framePos.x = j * m_frameWidth;
			framePos.y = i * m_frameHeight;

			//프레임 위치 목록 추기ㅏ
			m_frameList.push_back(framePos);
		}
	}

	//기본프레임으로 셋팅
	setDefPlayFrame();

	return S_OK;
}

void animation::setDefPlayFrame(bool reverse, bool loop)
{
	//루프 여부
	m_loop = loop;
	//플레이 목록 초기화
	m_playList.clear();

	if (reverse)
	{
		//리버스 후 1번 인덱스까지
		if (m_loop)
		{
			//정방향
			for (int i = 0; i < m_frameNum; i++)
			{
				m_playList.push_back(i);
			}
			//역방향
			for (int i = m_frameNum - 1; i > 0; i--)
			{
				m_playList.push_back(i);
			}
		}

		//리버스 후 0번 인덱스까지
		else
		{
			//정방향
			for (int i = 0; i < m_frameNum; i++)
			{
				m_playList.push_back(i);
			}
			//역방향
			for (int i = m_frameNum - 1; i >= 0; i--)
			{
				m_playList.push_back(i);
			}
		}
	}

	else
	{
		//정방향
		for (int i = 0; i < m_frameNum; i++)
		{
			m_playList.push_back(i);
		}
	}

}

void animation::setPlayFrame(int * playArr, int arrLen, bool loop)
{
	m_loop = loop;
	//플레이 목록 초기화
	m_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		m_playList.push_back(playArr[i]);
	}
}

void animation::setPlayFrame(int start, int end, bool reverse, bool loop)
{
	//루프 여부
	m_loop = loop;
	//플레이 목록 초기화
	m_playList.clear();

	//시작과 끝 프레임구간이 같은경우 재생할 필요 없다.
	if (start == end)
	{
		m_playList.clear();
		stop();
		return;
	}

	//시작 프레임이 종료 프레임보다 큰 경우
	else if (start > end)
	{
		if (reverse)
		{
			//리버스 후 1번 인덱스 까지만
			if (m_loop)
			{
				//정방향
				for (int i = start; i >= end; i--)
				{
					m_playList.push_back(i);
				}

				//역방향
				for (int i = end + 1; i < start; i++)
				{
					m_playList.push_back(i);
				}
			}

			//리버스 후 0번인덱스 가지
			else
			{
				for (int i = start; i >= end; i--)
				{
					m_playList.push_back(i);
				}
				//역방향
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

	//정상적으로 시작프레임이 종료프레임보다 작은 경우
	else
	{
		if (reverse)
		{
			//리버스 후 1번 인덱스 까지만
			if (m_loop)
			{
				for (int i = start; i <= end; i++)
				{
					m_playList.push_back(i);
				}
				//역방향
				for (int i = end - 1; i > start; i--)
				{
					m_playList.push_back(i);
				}
			}
			else//리버스 후 0번 인덱스 까지
			{
				//정방향
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
			//정방향
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
	//플레이 중이냐?
	if (m_play)
	{
		m_elapsedSec += elapsedTime;
		//프레임 업데이트 시간이 되었다면
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