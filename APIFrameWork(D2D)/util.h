#pragma once
//==================================================================
//		## using namespace MY_UTIL ##
//==================================================================

//angle * DEG_TO_RAD
#define DEG_TO_RAD 0.017435f  //degree 1도 (라디안값)
#define PI 3.141592654f
#define PI2		(PI * 2)

#define PI_2		(PI / 2)
#define PI_4		(PI / 4)
#define PI_8		(PI / 8)
#define PI_16	(PI / 16)
#define PI_32	(PI / 32)

namespace MY_UTIL //이건 너희들이 네임스페이스 이름을 변경해도 된다
{
	//두점사이의 각도
	float getAngle(float startX, float startY, float endX, float endY);
	//두점사이의 거리
	float getDistance(float startX, float startY, float endX, float endY);
};


struct tagCoordinate {
	float x, y;
};

struct tagPos
{
	int x, y;
};

inline bool samePos(tagPos pos1, tagPos pos2)
{
	if (pos1.x == pos2.x && pos1.y == pos2.y)
	{
		return true;
	}
	else
		return false;
}

inline tagPos makePos(int x, int y)
{
	tagPos temp;
	temp.x = x;
	temp.y = y;
	return temp;
}

enum STATUS
{
	BASE = 0,
	BASE_ENDTURN,
	WALK,
	WALK_STAY,
	ATTACK_READY,
	ATTACK_STAY,
	ATTACK_END,
	BATTLE_START_MOVE,
	HIT,
	HIT_ENDTURN,
	SKILL,
	MISS,
	BATTLE,
	DEAD_STAY,
	DEAD,
	EXTINCTION
};

enum TURN
{
	TURN_NON = 0,
	TURN_START,
	TURN_END
};

enum SPECIALITEM
{
	NON = 0,
	REBRITH,
	NONBATTLE,
	QUEST,
	NEWTYPE
};

struct tagStat
{
	//능력치
	int Lv;
	int CurrentEXP;
	int NextEXP;

	int currentHP;
	int maxHP;
	int currentMP;
	int maxMP;

	int Pow;
	int Int;
	int Dex;
	int Luck;

	int Att;
	int Def;
	int HitNum;

	SPECIALITEM acce1;
	SPECIALITEM acce2;
};

struct tagDamage
{
	float x, y;
	RECT rc;
	D2D1_RECT_F rcf;
	int damage;
	int cipher;
	bool on;
	float alpha;
};

enum EVENTSTATUS
{
	EVENTSTATUS_NON = 0,
	EVENTSTATUS_EXTIOCTION,
	EVENTSTATUS_NONACTIVATION,
	EVENTSTATUS_ACTIVATION
};

enum QUESTSTATUS
{
	QUEST_NON = 0,
	QUEST_BOSS,
	QUEST_BOSS_BATTLE,
	QUEST_CLEAR
};