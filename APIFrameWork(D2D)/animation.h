#pragma once
class animation
{
private:
	typedef vector<POINT> m_vFrameList;
	typedef vector<int> m_vPlayList;	//프레임 플레이 목록

	int m_frameNum;	//프레임 갯수
	m_vFrameList m_frameList;	//프레임 위치목록
	m_vPlayList m_playList;		//플레이 목록

	int m_frameWidth;	//프레임 가로크기
	int m_frameHeight;	//프레임 세로크기
	bool m_loop;

	float m_frameUpdateSec;	//프레임 업데이트 초
	float m_elapsedSec;		//프레임 경과 초
	DWORD m_nowPlayIndex;	//현재 플레이 인덱스(플레이 목록값 참조)
	bool m_play;	//현재 플레이 여부
public:
	animation();
	~animation();
	//초기화(이미지 전체가로크기, 세로크기, 한프레임 가로크기, 세로크기) 
	HRESULT init(int totalW, int totalH, int frameW, int frameH);

	//처음부터 끝까지 애니메이션 셋팅
	void setDefPlayFrame(bool reverse = false, bool loop = false);
	//배열에 담겨있는것만 
	void setPlayFrame(int* playArr, int arrLen, bool loop = false);
	//시작 , 끝구간 정해서 
	void setPlayFrame(int start, int end, bool reverse = false, bool loop = false);

	//초당 갱신 횟수
	void setFPS(int framePerSec);
	//프레임 업데이트 타임
	void frameUpdate(float elapsedTime);

	//플레이 시작
	void start();
	//정지
	void stop();
	//일시 정지
	void pause();
	//다시 재생
	void resume();

	inline bool isPlay() { return m_play; }
	inline POINT getFramePos() { return m_frameList[m_playList[m_nowPlayIndex]]; }
	inline int getFrameWidth() { return m_frameWidth; }
	inline int getFrameHeight() { return m_frameHeight; }
};