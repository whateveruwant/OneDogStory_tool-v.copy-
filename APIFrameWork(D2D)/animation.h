#pragma once
class animation
{
private:
	typedef vector<POINT> m_vFrameList;
	typedef vector<int> m_vPlayList;	//������ �÷��� ���

	int m_frameNum;	//������ ����
	m_vFrameList m_frameList;	//������ ��ġ���
	m_vPlayList m_playList;		//�÷��� ���

	int m_frameWidth;	//������ ����ũ��
	int m_frameHeight;	//������ ����ũ��
	bool m_loop;

	float m_frameUpdateSec;	//������ ������Ʈ ��
	float m_elapsedSec;		//������ ��� ��
	DWORD m_nowPlayIndex;	//���� �÷��� �ε���(�÷��� ��ϰ� ����)
	bool m_play;	//���� �÷��� ����
public:
	animation();
	~animation();
	//�ʱ�ȭ(�̹��� ��ü����ũ��, ����ũ��, �������� ����ũ��, ����ũ��) 
	HRESULT init(int totalW, int totalH, int frameW, int frameH);

	//ó������ ������ �ִϸ��̼� ����
	void setDefPlayFrame(bool reverse = false, bool loop = false);
	//�迭�� ����ִ°͸� 
	void setPlayFrame(int* playArr, int arrLen, bool loop = false);
	//���� , ������ ���ؼ� 
	void setPlayFrame(int start, int end, bool reverse = false, bool loop = false);

	//�ʴ� ���� Ƚ��
	void setFPS(int framePerSec);
	//������ ������Ʈ Ÿ��
	void frameUpdate(float elapsedTime);

	//�÷��� ����
	void start();
	//����
	void stop();
	//�Ͻ� ����
	void pause();
	//�ٽ� ���
	void resume();

	inline bool isPlay() { return m_play; }
	inline POINT getFramePos() { return m_frameList[m_playList[m_nowPlayIndex]]; }
	inline int getFrameWidth() { return m_frameWidth; }
	inline int getFrameHeight() { return m_frameHeight; }
};