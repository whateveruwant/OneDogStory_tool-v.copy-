#pragma once
#include "gameNode.h"
#define LINEMAX 300
#define OBJECTMAX 500
#define DOORMAX 15
#define ELEVATORMAX 20
#define RECTMAX 50
#define ENEMYMAX 100
enum EACTIVEMENU { EACTIVEMENU_NONE, EACTIVEMENU_MAP, EACTIVEMENU_ENEMY, EACTIVEMENU_OBJECT, EACTIVEMENU_BOSS, EACTIVEMENU_COLLISION, EACTIVEMENU_COLLISIONRECT, EACTIVEMENU_DOOR};
enum EPAGENUMBER { EPAGENUMBER_NONE , EPAGENUMBER_ONE, EPAGENUMBER_TWO, EPAGENUMBER_THREE, EPAGENUMBER_FOUR, EPAGENUMBER_FIVE, EPAGENUMBER_SIX, EPAGENUMBER_SEVEN, EPAGENUMBER_EIGHT, EPAGENUMBER_NINE};
enum EMAKELINE { EMAKELINE_NONE, EMAKELINE_HALF, EMAKELINE_DONE};
enum ETEXTINPUTMODE { ETEXTINPUTMODE_NONE, ETEXTINPUTMODE_DESTINATION, ETEXTINPUTMODE_DSX, ETEXTINPUTMODE_DSY};

enum EMAKERECT { EMAKERECT_NONE, EMAKERECT_ONEPOINT, EMAKERECT_DONE};
enum EMAKEELEVATOR { EMAKEELEVATOR_NONE, EMAKEELEVATOR_STARTSET, EMAKEELEVATOR_ENDSET, EMAKEELEVATOR_SWITCHSET1, EMAKEELEVATOR_SWITCHSET2};
enum EELEVATORKIND { EELEVATORKIND_SMALL, EELEVATORKIND_LASER};
enum EDOORKIND { EDOORKIND_NONE, EDOORKIND_BREAK, EDOORKIND_FREE, EDOORKIND_NEED_RED, EDOORKIND_NEED_BLUE, EDOORKIND_NEED_GREEN, EDOORKIND_NEED_WHITE, EDOORKIND_NEED_OBJECT};
enum EOBJECTKIND { EOBJECTKIND_NONE, EOBJECTKIND_MADE};
enum EENEMYKIND { EENEMYKIND_NONE, EENEMYKIND_MADE};

enum EITEMTYPE { EITEMTYPE_NONE, EITEMTYPE_OBJECT, EITEMTYPE_ENEMY, EITEMTYPE_DOOR, EITEMTYPE_ELEVATOR};

struct SENEMY {
	float x, y; //¸Ê¿¡ ´ëÇÑ ÁÂÇ¥
	EENEMYKIND kind;
	tchar name[50];
};
struct SOBJECT {
	float x, y; //¸Ê¿¡ ´ëÇÑ ÁÂÇ¥
	EOBJECTKIND kind;
	tchar name[50];
};
struct SDOOR {
	float x, y; //¸Ê¿¡ ´ëÇÑ ÁÂÇ¥
	EDOORKIND kind;
	tchar destination[50];		//¸ñÀû¸Ê
	float dsX, dsY;				//¸ñÀû¸ÊÀÇ ÁÂÇ¥
};
struct SELEVATOR {
	float startX, startY;	//½ÃÀÛÁ¡
	float endX, endY;		//³¡Á¡
	EELEVATORKIND kind;
	EMAKEELEVATOR state;
	tchar name[50];
	D2D_POINT_2F switchPosition[2]; //½ºÀ§Ä¡ÁÂÇ¥
};
struct SCOLLILINE {
	D2D_POINT_2F postPoint;	//¸Ê¿¡ ´ëÇÑ ÁÂÇ¥
	D2D_POINT_2F endPoint;  //¸Ê¿¡ ´ëÇÑ ÁÂÇ¥
	EMAKELINE state;
};
struct SCOLLIRECT {
	D2D_RECT_F rect;
	EMAKERECT state;
};
struct SBACKGROUND {
	tstring kind;
};
struct SLISTNAME {
	tchar name[20]; //text Ãâ·Â¶§¹®¿¡ tcharÇü
};
struct SINMOUSEINFO {
	SENEMY inEnemy;
	SDOOR inDoor;
	SOBJECT inObject;
	SELEVATOR inElevator;

	EITEMTYPE type;
};
class mainGame : public gameNode
{
private:
	D2D1_POINT_2F _center;

	EACTIVEMENU menu;
	EPAGENUMBER pageNum;
	SLISTNAME list[5][9][9]; // 0:map, 1:enemy, 2:object, 3:boss, 4:door

	SINMOUSEINFO inClickInfo;
	D2D_RECT_F makinRect;
	ETEXTINPUTMODE typingMode;

	SBACKGROUND bg;
	vector<SOBJECT> vWater;
	vector<SOBJECT> vObject;
	vector<SENEMY> vEnemy;
	vector<SCOLLILINE> vCollisionLine;
	vector<SDOOR> vDoor;
	vector<SELEVATOR> vElevator;
	vector<SCOLLIRECT> vCollisionRect;

	//ÀúÀå¿ë
	SOBJECT Object[OBJECTMAX];
	SENEMY Enemy[ENEMYMAX];
	SCOLLILINE CollisionLine[LINEMAX];
	SDOOR Door[DOORMAX];
	SELEVATOR Elevator[ELEVATORMAX];
	SCOLLIRECT CollisionRect[RECTMAX];

public:
	mainGame();
	~mainGame();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void initList();

	void save();
	void load(tstring bgName);

	void clickMouse();
	void imageLoad();
	void dropResource();
};