#include "stdafx.h"
#include "mainGame.h"

//IsInRect(_ptMouse, D2D1::RectF(24, 24, 888, 693)) : activzone

mainGame::mainGame()
{
}


mainGame::~mainGame()
{
}

HRESULT mainGame::init(void)
{
	gameNode::init(true);
	SOUNDMANAGER->addSound("bgm", "sounds/Retribution.wav", true, true);
	SOUNDMANAGER->addSound("click", "sounds/clickSound.mp3", false, false);

	SOUNDMANAGER->bgmPlay("bgm", 0.5f);

	_center = { WINSIZEX_NUM / 2, WINSIZEY_NUM / 2 };
	CAMERAMANAGER->setPositionMemotyRick(&_center);
	pageNum = EPAGENUMBER_NONE;
	menu = EACTIVEMENU_NONE;
	initList();
	imageLoad();

	bg.kind = L"NONE DATA";
	inClickInfo.type = EITEMTYPE_NONE;

	typingMode = ETEXTINPUTMODE_NONE;

	for (int i = 0; i < OBJECTMAX; ++i) {
		Object[i].kind = EOBJECTKIND_NONE;
	}
	for (int i = 0; i < ENEMYMAX; ++i) {
		Enemy[i].kind = EENEMYKIND_NONE;
	}
	for (int i = 0; i < LINEMAX; ++i) {
		CollisionLine[i].state = EMAKELINE_NONE;
	}
	for (int i = 0; i < RECTMAX; ++i) {
		CollisionRect[i].state = EMAKERECT_NONE;
	}
	for (int i = 0; i < DOORMAX; ++i) {
		Door[i].kind = EDOORKIND_NONE;
	}
	for (int i = 0; i < ELEVATORMAX; ++i) {
		Elevator[i].state = EMAKEELEVATOR_NONE;
	}
	return S_OK;
}

void mainGame::release(void)
{
	gameNode::release();
}

void mainGame::update(void)
{
	gameNode::update();
	SOUNDMANAGER->update();
	if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
		_center.x -= 10.0f;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
		_center.x += 10.0f;
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP)) {
		_center.y -= 10.0f;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN)) {
		_center.y += 10.0f;
	}
	if (KEYMANAGER->isStayKeyDown('J')) {
		_center.x -= 1.0f;
	}
	if (KEYMANAGER->isStayKeyDown('L')) {
		_center.x += 1.0f;
	}
	if (KEYMANAGER->isStayKeyDown('I')) {
		_center.y -= 1.0f;
	}
	if (KEYMANAGER->isStayKeyDown('K')) {
		_center.y += 1.0f;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) {
		SOUNDMANAGER->play("click", 1.0f);
		clickMouse();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN)) {
		if (gameNode::gettypingReady()) {
			if (typingMode != ETEXTINPUTMODE_NONE) {
				switch (typingMode)
				{
					tchar* stop;
				case ETEXTINPUTMODE_NONE:
					break;
				case ETEXTINPUTMODE_DESTINATION:
					wsprintf(inClickInfo.inDoor.destination, StrName);
					break;
				case ETEXTINPUTMODE_DSX:
					inClickInfo.inDoor.dsX = wcstof(StrName, &stop);
					break;
				case ETEXTINPUTMODE_DSY:
					inClickInfo.inDoor.dsY = wcstof(StrName, &stop);
					break;
				}
				typingMode = ETEXTINPUTMODE_NONE;
				settypingMode(false);
			}
		}
	}
}

void mainGame::render(void)
{
	Rt->BeginDraw();
	Rt->SetTransform(D2D1::Matrix3x2F::Identity());
	Rt->Clear();
	D2D1_RECT_F rect = D2D1::RectF(0, 0, WINSIZEX_NUM, WINSIZEY_NUM);
	//==================================================================
	if (bg.kind != L"NONE DATA") {
		IMAGEMANAGER->render(bg.kind, Rt, _zoom);
		//object
		for (int i = 0; i < vObject.size(); ++i) {
			IMAGEMANAGER->render(vObject.at(i).name, Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vObject.at(i).x, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vObject.at(i).y, _zoom);
		}
		//enemy
		for (int i = 0; i < vEnemy.size(); ++i) {
			IMAGEMANAGER->render(vEnemy.at(i).name, Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vEnemy.at(i).x, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vEnemy.at(i).y, _zoom);
		}

		//collision line
		for (int i = 0; i < vCollisionLine.size(); ++i) {
			if (vCollisionLine.at(i).state == EMAKELINE_DONE) {
				LineMake(Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vCollisionLine.at(i).postPoint.x,
					CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vCollisionLine.at(i).postPoint.y,
					CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vCollisionLine.at(i).endPoint.x,
					CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vCollisionLine.at(i).endPoint.y, Brush);
			}
		}

		//collision Rect
		for (int i = 0; i < vCollisionRect.size(); ++i) {
			if (vCollisionRect.at(i).state == EMAKERECT_DONE) {
				D2Rectangle(Rt, { CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vCollisionRect.at(i).rect.left,
					CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vCollisionRect.at(i).rect.top,
					CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vCollisionRect.at(i).rect.right,
					CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vCollisionRect.at(i).rect.bottom}, BlueBrush);
			}
			else if (vCollisionRect.at(i).state == EMAKERECT_ONEPOINT) {
				D2Rectangle(Rt, { CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vCollisionRect.at(i).rect.left,
					CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vCollisionRect.at(i).rect.top
					, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
					CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom) }, BlueBrush);
			}
		}

		//door
		for (int i = 0; i < vDoor.size(); ++i) {
			switch (vDoor.at(i).kind)
			{
			case EDOORKIND_BREAK:
			case EDOORKIND_FREE:
			case EDOORKIND_NEED_RED:
			case EDOORKIND_NEED_BLUE:
			case EDOORKIND_NEED_GREEN:
			case EDOORKIND_NEED_WHITE:
				IMAGEMANAGER->render(L"door_break", Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vDoor.at(i).x, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vDoor.at(i).y, _zoom);
				break;
			case EDOORKIND_NEED_OBJECT:
				IMAGEMANAGER->render(L"door_object", Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vDoor.at(i).x, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vDoor.at(i).y, _zoom);
				break;
			}
		}

		//elevator
		for (int i = 0; i < vElevator.size(); ++i) {
			switch (vElevator.at(i).state)
			{
			case EMAKEELEVATOR_STARTSET:
				IMAGEMANAGER->render(vElevator.at(i).name, Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vElevator.at(i).startX, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).startY, _zoom);
				break;
			case EMAKEELEVATOR_ENDSET:
				IMAGEMANAGER->render(vElevator.at(i).name, Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vElevator.at(i).startX, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).startY, _zoom);
				IMAGEMANAGER->alphaRender(vElevator.at(i).name, Rt, 0.7f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vElevator.at(i).endX, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).endY, _zoom);
				break;
			case EMAKEELEVATOR_SWITCHSET1:
				IMAGEMANAGER->render(vElevator.at(i).name, Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vElevator.at(i).startX, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).startY, _zoom);
				IMAGEMANAGER->alphaRender(vElevator.at(i).name, Rt, 0.7f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vElevator.at(i).endX, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).endY, _zoom);
				IMAGEMANAGER->render(L"panel", Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom* vElevator.at(i).switchPosition[0].x, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).switchPosition[0].y, _zoom);
				break;
			case EMAKEELEVATOR_SWITCHSET2:
				IMAGEMANAGER->render(vElevator.at(i).name, Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vElevator.at(i).startX, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).startY, _zoom);
				IMAGEMANAGER->alphaRender(vElevator.at(i).name, Rt, 0.7f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*vElevator.at(i).endX, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).endY, _zoom);
				IMAGEMANAGER->render(L"panel", Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom* vElevator.at(i).switchPosition[0].x, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).switchPosition[0].y, _zoom);
				IMAGEMANAGER->render(L"panel", Rt, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom* vElevator.at(i).switchPosition[1].x, CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*vElevator.at(i).switchPosition[1].y, _zoom);
				break;
			}
		}

		//마우스 위에 drop대상
		if (IsInRect(_ptMouse, D2D1::RectF(24, 24, 888, 693)) && menu != EACTIVEMENU_COLLISION && menu != EACTIVEMENU_COLLISIONRECT) {
			switch (inClickInfo.type)
			{
			case EITEMTYPE_NONE:
				break;
			case EITEMTYPE_OBJECT:
				IMAGEMANAGER->alphaRender(inClickInfo.inObject.name, Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
				break;
			case EITEMTYPE_ENEMY:
				IMAGEMANAGER->alphaRender(inClickInfo.inEnemy.name, Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
				break;
			case EITEMTYPE_DOOR:
				if (inClickInfo.inDoor.destination != L"PUT THE MAPNAME" && inClickInfo.inDoor.dsX != 0.0f && inClickInfo.inDoor.dsY != 0.0f) {
					switch (inClickInfo.inDoor.kind)
					{
					case EDOORKIND_BREAK:
					case EDOORKIND_FREE:
					case EDOORKIND_NEED_RED:
					case EDOORKIND_NEED_BLUE:
					case EDOORKIND_NEED_GREEN:
					case EDOORKIND_NEED_WHITE:
						IMAGEMANAGER->alphaRender(L"door_break", Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
						break;
					case EDOORKIND_NEED_OBJECT:
						IMAGEMANAGER->alphaRender(L"door_object", Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
						break;
					}
				}
				break;
			case EITEMTYPE_ELEVATOR:
				switch (inClickInfo.inElevator.state)
				{
				case EMAKEELEVATOR_NONE:
					IMAGEMANAGER->alphaRender(inClickInfo.inElevator.name, Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
					break;
				case EMAKEELEVATOR_STARTSET:
					IMAGEMANAGER->alphaRender(inClickInfo.inElevator.name, Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
					break;
				case EMAKEELEVATOR_ENDSET:
					IMAGEMANAGER->alphaRender(L"panel", Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
					break;
				case EMAKEELEVATOR_SWITCHSET1:
					IMAGEMANAGER->alphaRender(L"panel", Rt, 0.5f, CAMERAMANAGER->getAbsPosition({ 0,0 }).x + IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom*(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), CAMERAMANAGER->getAbsPosition({ 0,0 }).y + IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom*(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), _zoom);
					break;
				}
			}
		}
	}

	//UI, button render
	IMAGEMANAGER->render(L"mainBG", Rt);
	IMAGEMANAGER->render(L"mapbutton", Rt);
	IMAGEMANAGER->render(L"enemybutton", Rt);
	IMAGEMANAGER->render(L"objectbutton", Rt);
	IMAGEMANAGER->render(L"bossbutton", Rt);
	IMAGEMANAGER->render(L"doorbutton", Rt);
	IMAGEMANAGER->render(L"collisionbutton", Rt);
	IMAGEMANAGER->render(L"rectbutton", Rt);

	switch (menu)
	{
	case EACTIVEMENU_MAP:
		IMAGEMANAGER->renderByMask(L"mapbutton", Rt);
		if (pageNum != EPAGENUMBER_NONE) {
			for (int k = 0; k < 9; ++k) {
				D2TextOut(Rt, list[0][pageNum-1][k].name, TextFormat, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20), WhiteBrush);
				if (bg.kind == list[0][pageNum - 1][k].name && bg.kind != L"NONE DATA") {
					Rt->FillRectangle(D2D1::RectF(922, 121 + k * 20, 1262, 141 + k * 20), BlackBrush);
					//미리보기 이미지 출력
					image tmpimage = *IMAGEMANAGER->findImage(bg.kind);
					IMAGEMANAGER->render(bg.kind, Rt, 925, 300, 330, 245, tmpimage.getX(), tmpimage.getY(), tmpimage.getWidth(), tmpimage.getHeight());
				}
			}
		}
		break;
	case EACTIVEMENU_ENEMY:
		IMAGEMANAGER->renderByMask(L"enemybutton", Rt);
		if (pageNum != EPAGENUMBER_NONE) {
			for (int k = 0; k < 9; ++k) {
				D2TextOut(Rt, list[1][pageNum - 1][k].name, TextFormat, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20), WhiteBrush);
				tstring tmpStr = inClickInfo.inEnemy.name;
				if (tmpStr == list[1][pageNum - 1][k].name && inClickInfo.type == EITEMTYPE_ENEMY && tmpStr != L"NONE DATA") {
					Rt->FillRectangle(D2D1::RectF(922, 121 + k * 20, 1262, 141 + k * 20), BlackBrush);
					//미리보기 이미지 출력
					IMAGEMANAGER->render(inClickInfo.inEnemy.name, Rt, 1090, 420);
				}
			}
		}
		break;
	case EACTIVEMENU_OBJECT:
		IMAGEMANAGER->renderByMask(L"objectbutton", Rt);
		if (pageNum != EPAGENUMBER_NONE) {
			for (int k = 0; k < 9; ++k) {
				if (pageNum == EPAGENUMBER_NINE) {
					//elevator case
					D2TextOut(Rt, list[2][pageNum - 1][k].name, TextFormat, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20), WhiteBrush);
					tstring tmpStr = inClickInfo.inElevator.name;
					if (tmpStr == list[2][pageNum - 1][k].name && inClickInfo.type == EITEMTYPE_ELEVATOR && tmpStr != L"NONE DATA") {
						Rt->FillRectangle(D2D1::RectF(922, 121 + k * 20, 1262, 141 + k * 20), BlackBrush);
						//미리보기 이미지 출력
						IMAGEMANAGER->render(inClickInfo.inElevator.name, Rt, 1090, 420);
					}
				}
				else {
					D2TextOut(Rt, list[2][pageNum - 1][k].name, TextFormat, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20), WhiteBrush);
					tstring tmpStr = inClickInfo.inObject.name;
					if (tmpStr == list[2][pageNum - 1][k].name && inClickInfo.type == EITEMTYPE_OBJECT && tmpStr != L"NONE DATA") {
						Rt->FillRectangle(D2D1::RectF(922, 121 + k * 20, 1262, 141 + k * 20), BlackBrush);
						//미리보기 이미지 출력
						IMAGEMANAGER->render(inClickInfo.inObject.name, Rt, 1090, 420);
					}
				}
			}
		}
		break;
	case EACTIVEMENU_BOSS:
		IMAGEMANAGER->renderByMask(L"bossbutton", Rt);
		if (pageNum != EPAGENUMBER_NONE) {
			for (int k = 0; k < 9; ++k) {
				D2TextOut(Rt, list[3][pageNum - 1][k].name, TextFormat, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20), WhiteBrush);
				//추가할것
				tstring tmpStr = inClickInfo.inEnemy.name;
				if (tmpStr == list[3][pageNum - 1][k].name && inClickInfo.type == EITEMTYPE_ENEMY && tmpStr != L"NONE DATA") {
					Rt->FillRectangle(D2D1::RectF(922, 121 + k * 20, 1262, 141 + k * 20), BlackBrush);
					//미리보기 이미지 출력
					image tmpimage = *IMAGEMANAGER->findImage(inClickInfo.inEnemy.name);
					IMAGEMANAGER->render(inClickInfo.inEnemy.name, Rt, 925, 350, 330, 145, tmpimage.getX(), tmpimage.getY(), tmpimage.getWidth()*2, tmpimage.getHeight()*2);
				}
			}
		}
		break;
	case EACTIVEMENU_COLLISION:
		IMAGEMANAGER->renderByMask(L"collisionbutton", Rt);
		break;
	case EACTIVEMENU_COLLISIONRECT:
		IMAGEMANAGER->renderByMask(L"rectbutton", Rt);
		break;
	case EACTIVEMENU_DOOR:
		IMAGEMANAGER->renderByMask(L"doorbutton", Rt);
		if (pageNum != EPAGENUMBER_NONE) {
			tchar tmpStr[50];
			for (int k = 0; k < 9; ++k) {
				D2TextOut(Rt, list[4][pageNum - 1][k].name, TextFormat, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20), WhiteBrush);
				if (inClickInfo.type == EITEMTYPE_DOOR && inClickInfo.inDoor.kind == k + 1 && list[4][pageNum - 1][k].name != L"NONE DATA") {
					Rt->FillRectangle(D2D1::RectF(922, 121 + k * 20, 1262, 141 + k * 20), BlackBrush);
					//미리보기 이미지 출력
					IMAGEMANAGER->render(list[4][pageNum - 1][k].name, Rt, 1090, 420);
					//정보출력
					switch (typingMode)
					{
					case ETEXTINPUTMODE_NONE:
						wsprintf(tmpStr, inClickInfo.inDoor.destination);
						D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 310, WINSIZEX_NUM, 330), WhiteBrush);
						if (inClickInfo.inDoor.dsX == 0) {
							wsprintf(tmpStr, L"PUT THE X(FLOAT)");
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350), WhiteBrush);
						}
						else {
							swprintf_s(tmpStr, TEXT("%f"), inClickInfo.inDoor.dsX);
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350), WhiteBrush);
						}
						if (inClickInfo.inDoor.dsY == 0) {
							wsprintf(tmpStr, L"PUT THE Y(FLOAT)");
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370), WhiteBrush);
						}
						else {
							swprintf_s(tmpStr, TEXT("%f"), inClickInfo.inDoor.dsY);
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370), WhiteBrush);
						}
						break;
					case ETEXTINPUTMODE_DESTINATION:
						Rt->FillRectangle(D2D1::RectF(1035, 308, WINSIZEX_NUM - 50, 328), BlackBrush);
						DWInit(L"함초롬돋움", 12.0f, &WriteFactory, &TextFormat);
						D2TextOut(Rt, StrName, TextFormat, D2D1::RectF(1040, 310, WINSIZEX_NUM, 330), AquaBrush);
						if (inClickInfo.inDoor.dsX == 0) {
							wsprintf(tmpStr, L"PUT THE X(FLOAT)");
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350), WhiteBrush);
						}
						else {
							swprintf_s(tmpStr, TEXT("%f"), inClickInfo.inDoor.dsX);
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350), WhiteBrush);
						}
						if (inClickInfo.inDoor.dsY == 0) {
							wsprintf(tmpStr, L"PUT THE Y(FLOAT)");
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370), WhiteBrush);
						}
						else {
							swprintf_s(tmpStr, TEXT("%f"), inClickInfo.inDoor.dsY);
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370), WhiteBrush);
						}
						break;
					case ETEXTINPUTMODE_DSX:
						Rt->FillRectangle(D2D1::RectF(1035, 328, WINSIZEX_NUM - 50, 348), BlackBrush);
						DWInit(L"함초롬돋움", 12.0f, &WriteFactory, &TextFormat);
						D2TextOut(Rt, StrName, TextFormat, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350), AquaBrush);
						wsprintf(tmpStr, inClickInfo.inDoor.destination);
						D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 310, WINSIZEX_NUM, 330), WhiteBrush);
						if (inClickInfo.inDoor.dsY == 0) {
							wsprintf(tmpStr, L"PUT THE Y(FLOAT)");
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370), WhiteBrush);
						}
						else {
							swprintf_s(tmpStr, TEXT("%f"), inClickInfo.inDoor.dsY);
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370), WhiteBrush);
						}
						break;
					case ETEXTINPUTMODE_DSY:
						Rt->FillRectangle(D2D1::RectF(1035, 348, WINSIZEX_NUM - 50, 368), BlackBrush);
						DWInit(L"함초롬돋움", 12.0f, &WriteFactory, &TextFormat);
						D2TextOut(Rt, StrName, TextFormat, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370), AquaBrush);
						wsprintf(tmpStr, inClickInfo.inDoor.destination);
						D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 310, WINSIZEX_NUM, 330), WhiteBrush);
						if (inClickInfo.inDoor.dsX == 0) {
							wsprintf(tmpStr, L"PUT THE X(FLOAT)");
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350), WhiteBrush);
						}
						else {
							swprintf_s(tmpStr, TEXT("%f"), inClickInfo.inDoor.dsX);
							D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350), WhiteBrush);
						}
						break;
					}
				}
			}
			wsprintf(tmpStr, L"Destination       :");
			D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(930, 310, WINSIZEX_NUM, 330), WhiteBrush);
			wsprintf(tmpStr, L"Destination(X)   :");
			D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(930, 330, WINSIZEX_NUM, 350), WhiteBrush);
			wsprintf(tmpStr, L"Destination(Y)   :");
			D2TextOut(Rt, tmpStr, TextFormat, D2D1::RectF(930, 350, WINSIZEX_NUM, 370), WhiteBrush);
		}
		break;
	}
	IMAGEMANAGER->render(L"deletebutton", Rt);
	IMAGEMANAGER->render(L"savebutton", Rt);
	IMAGEMANAGER->render(L"loadbutton", Rt);
	IMAGEMANAGER->render(L"newpointbutton", Rt);

	for (int i = 1; i < 10; ++i) {
		tchar tmp[10];
		wsprintf(tmp, L"%dbutton", i);
		IMAGEMANAGER->render(tmp, Rt);
		if (i == (int)pageNum) {
			IMAGEMANAGER->renderByMask(tmp, Rt);
		}
	}

	//collision helper textout
	wchar_t tmpColli[256];
	swprintf_s(tmpColli, L"CurrentPoint :");
	D2TextOut(Rt, tmpColli, TextFormat, D2D1::RectF(932, 600, 1262, 620), WhiteBrush);
	swprintf_s(tmpColli, L"StartPoint :");
	D2TextOut(Rt, tmpColli, TextFormat, D2D1::RectF(932, 620, 1262, 640), WhiteBrush);
	swprintf_s(tmpColli, L"EndPoint :");
	D2TextOut(Rt, tmpColli, TextFormat, D2D1::RectF(932, 640, 1262, 660), WhiteBrush);
	if (bg.kind != L"NONE DATA") {
		swprintf_s(tmpColli, L"( %.3f, %.3f )", (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom));
		D2TextOut(Rt, tmpColli, TextFormat, D2D1::RectF(1020, 600, 1262, 620), WhiteBrush);
		if (vCollisionLine.size() == 0) {}
		else if (vCollisionLine.at(vCollisionLine.size() - 1).state == EMAKELINE_HALF) {
			swprintf_s(tmpColli, L"( %.3f, %.3f )", vCollisionLine.at(vCollisionLine.size() - 1).postPoint.x, vCollisionLine.at(vCollisionLine.size() - 1).postPoint.y);
			D2TextOut(Rt, tmpColli, TextFormat, D2D1::RectF(1020, 620, 1262, 640), WhiteBrush);
			swprintf_s(tmpColli, L"( %.3f, %.3f )", (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom));
			D2TextOut(Rt, tmpColli, TextFormat, D2D1::RectF(1020, 640, 1262, 660), WhiteBrush);
		}
		else {
			swprintf_s(tmpColli, L"( %.3f, %.3f )", (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom), (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom));
			D2TextOut(Rt, tmpColli, TextFormat, D2D1::RectF(1020, 620, 1262, 640), WhiteBrush);
		}
	}
	gameNode::render();
	//==================================================================
	TIMEMANAGER->render(Rt);
	Rt->EndDraw();
}

void mainGame::save()
{
	//충돌라인저장
	for (int i = 0; i < vCollisionLine.size(); ++i) {
		CollisionLine[i] = vCollisionLine.at(i);
	}
	HANDLE file;
	DWORD write;
	tstring nameString = L"mapfile/" + bg.kind + L"_line.map";
	tchar* name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	WriteFile(file, CollisionLine, sizeof(SCOLLILINE) * LINEMAX, &write, NULL);
	CloseHandle(file);
	for (int i = 0; i < LINEMAX; ++i) {
		CollisionLine[i].state = EMAKELINE_NONE;
	}

	//충돌렉트저장
	for (int i = 0; i < vCollisionRect.size(); ++i) {
		CollisionRect[i] = vCollisionRect.at(i);
	}
	nameString = L"mapfile/" + bg.kind + L"_rect.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	WriteFile(file, CollisionRect, sizeof(SCOLLIRECT) * RECTMAX, &write, NULL);
	CloseHandle(file);
	for (int i = 0; i < RECTMAX; ++i) {
		CollisionRect[i].state = EMAKERECT_NONE;
	}

	//오브젝트저장
	for (int i = 0; i < vObject.size(); ++i) {
		Object[i] = vObject.at(i);
	}
	nameString = L"mapfile/" + bg.kind + L"_object.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	WriteFile(file, Object, sizeof(SOBJECT) * OBJECTMAX, &write, NULL);
	CloseHandle(file);
	for (int i = 0; i < OBJECTMAX; ++i) {
		Object[i].kind = EOBJECTKIND_NONE;
	}

	//enemy저장
	for (int i = 0; i < vEnemy.size(); ++i) {
		Enemy[i] = vEnemy.at(i);
	}
	nameString = L"mapfile/" + bg.kind + L"_enemy.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	WriteFile(file, Enemy, sizeof(SENEMY) * ENEMYMAX, &write, NULL);
	CloseHandle(file);
	for (int i = 0; i < ENEMYMAX; ++i) {
		Enemy[i].kind = EENEMYKIND_NONE;
	}

	//Door저장
	for (int i = 0; i < vDoor.size(); ++i) {
		Door[i] = vDoor.at(i);
	}
	nameString = L"mapfile/" + bg.kind + L"_door.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	WriteFile(file, Door, sizeof(SDOOR) * DOORMAX, &write, NULL);
	CloseHandle(file);
	for (int i = 0; i < DOORMAX; ++i) {
		Door[i].kind = EDOORKIND_NONE;
	}

	//Elevator저장
	for (int i = 0; i < vElevator.size(); ++i) {
		if (vElevator.at(i).state >= EMAKEELEVATOR_SWITCHSET1) {
			Elevator[i] = vElevator.at(i);
		}
	}
	nameString = L"mapfile/" + bg.kind + L"_elevator.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	WriteFile(file, Elevator, sizeof(SELEVATOR) * ELEVATORMAX, &write, NULL);
	CloseHandle(file);
	for (int i = 0; i < ELEVATORMAX; ++i) {
		Elevator[i].state = EMAKEELEVATOR_NONE;
	}
}

void mainGame::load(tstring bgName)
{
	//충돌라인불러오기
	HANDLE file;
	DWORD read;
	tstring nameString = L"mapfile/" + bgName + L"_line.map";
	tchar* name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	ReadFile(file, CollisionLine, sizeof(SCOLLILINE) * LINEMAX, &read, NULL);
	CloseHandle(file);
	vCollisionLine.clear();
	for (int i = 0; i < LINEMAX; ++i) {
		if (CollisionLine[i].state != EMAKELINE_NONE) {
			vCollisionLine.push_back(CollisionLine[i]);
		}
		else break;
	}
	for (int i = 0; i < LINEMAX; ++i) {
		CollisionLine[i].state = EMAKELINE_NONE;
	}

	//충돌렉트불러오기
	nameString = L"mapfile/" + bgName + L"_rect.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	ReadFile(file, CollisionRect, sizeof(SCOLLIRECT) * RECTMAX, &read, NULL);
	CloseHandle(file);
	vCollisionRect.clear();
	for (int i = 0; i < RECTMAX; ++i) {
		if (CollisionRect[i].state != EMAKERECT_NONE) {
			vCollisionRect.push_back(CollisionRect[i]);
		}
		else break;
	}
	for (int i = 0; i < RECTMAX; ++i) {
		CollisionRect[i].state = EMAKERECT_NONE;
	}

	//오브젝트불러오기
	nameString = L"mapfile/" + bgName + L"_object.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	ReadFile(file, Object, sizeof(SOBJECT) * OBJECTMAX, &read, NULL);
	CloseHandle(file);
	vObject.clear();
	for (int i = 0; i < OBJECTMAX; ++i) {
		if (Object[i].kind != EOBJECTKIND_NONE) {
			vObject.push_back(Object[i]);
		}
		else break;
	}
	for (int i = 0; i < OBJECTMAX; ++i) {
		Object[i].kind = EOBJECTKIND_NONE;
	}

	//에너미불러오기
	nameString = L"mapfile/" + bgName + L"_enemy.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	ReadFile(file, Enemy, sizeof(SENEMY) * ENEMYMAX, &read, NULL);
	CloseHandle(file);
	vEnemy.clear();
	for (int i = 0; i < ENEMYMAX; ++i) {
		if (Enemy[i].kind != EENEMYKIND_NONE) {
			vEnemy.push_back(Enemy[i]);
		}
		else break;
	}
	for (int i = 0; i < ENEMYMAX; ++i) {
		Enemy[i].kind = EENEMYKIND_NONE;
	}

	//Door불러오기
	nameString = L"mapfile/" + bgName + L"_door.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	ReadFile(file, Door, sizeof(SDOOR) * DOORMAX, &read, NULL);
	CloseHandle(file);
	vDoor.clear();
	for (int i = 0; i < DOORMAX; ++i) {
		if (Door[i].kind != EDOORKIND_NONE) {
			vDoor.push_back(Door[i]);
		}
		else break;
	}
	for (int i = 0; i < DOORMAX; ++i) {
		Door[i].kind = EDOORKIND_NONE;
	}

	//elevator 불러오기
	nameString = L"mapfile/" + bgName + L"_elevator.map";
	name = (wchar_t *)nameString.c_str();
	file = CreateFile(name,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	ReadFile(file, Elevator, sizeof(SELEVATOR) * ELEVATORMAX, &read, NULL);
	CloseHandle(file);
	vElevator.clear();
	for (int i = 0; i < ELEVATORMAX; ++i) {
		if (Elevator[i].state >= EMAKEELEVATOR_SWITCHSET1) {
			vElevator.push_back(Elevator[i]);
		}
		else break;
	}
	for (int i = 0; i < ELEVATORMAX; ++i) {
		Elevator[i].state = EMAKEELEVATOR_NONE;
	}
}

void mainGame::clickMouse()
{
	//item drop
	if (IsInRect(_ptMouse, D2D1::RectF(24, 24, 888, 693)) && bg.kind != L"NONE DATA" && menu != EACTIVEMENU_COLLISION && menu != EACTIVEMENU_COLLISIONRECT) {
		dropResource();
	}

	//collision line create
	else if (IsInRect(_ptMouse, D2D1::RectF(24, 24, 888, 693)) && bg.kind != L"NONE DATA" && menu == EACTIVEMENU_COLLISION) {
		if (vCollisionLine.size() == 0) { //하나도 생성이 안되었을때
			SCOLLILINE tmp = { {(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
				(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom)},{ 0, 0}, EMAKELINE_HALF };
			vCollisionLine.push_back(tmp);
		}
		else if (vCollisionLine.at(vCollisionLine.size() - 1).state == EMAKELINE_DONE) {
			SCOLLILINE tmp = { {(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
				(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom)},{0, 0},EMAKELINE_HALF };
			vCollisionLine.push_back(tmp);
		}
		else {
			vCollisionLine.at(vCollisionLine.size()-1).state = EMAKELINE_DONE;
			vCollisionLine.at(vCollisionLine.size() - 1).endPoint = { (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom) , (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom) };
			SCOLLILINE tmp = { {(_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
				(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom)},{0, 0},EMAKELINE_HALF };
			vCollisionLine.push_back(tmp);
		}
	}

	//collision rect create
	else if (IsInRect(_ptMouse, D2D1::RectF(24, 24, 888, 693)) && bg.kind != L"NONE DATA" && menu == EACTIVEMENU_COLLISIONRECT) {
		if (vCollisionRect.size() == 0) { //하나도 생성이 안되었을때
			SCOLLIRECT tmp = { { (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
				(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), 0, 0 }, EMAKERECT_ONEPOINT };
			vCollisionRect.push_back(tmp);
		}
		else if (vCollisionRect.at(vCollisionRect.size() - 1).state == EMAKERECT_DONE) {
			SCOLLIRECT tmp = { { (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
				(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom), 0, 0 }, EMAKERECT_ONEPOINT };
			vCollisionRect.push_back(tmp);
		}
		else if(vCollisionRect.at(vCollisionRect.size() - 1).state == EMAKERECT_ONEPOINT){
			vCollisionRect.at(vCollisionRect.size() - 1).state = EMAKERECT_DONE;
			vCollisionRect.at(vCollisionRect.size() - 1).rect.right = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
			vCollisionRect.at(vCollisionRect.size() - 1).rect.bottom = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
		}
	}

	//new line button
	else if (IsInRect(_ptMouse, D2D1::RectF(1172, 592, 1172 + 85, 592 + 19)) && menu == EACTIVEMENU_COLLISION) {
		if (vCollisionLine.size() != 0 && vCollisionLine.at(vCollisionLine.size() - 1).state == EMAKELINE_HALF) {
			vCollisionLine.pop_back();
		}
	}

	//menu click
	if (IsInRect(_ptMouse, D2D1::RectF(918, 21, 918 + 85, 21 + 19))) {//MAP
		if (menu == EACTIVEMENU_MAP) {
			menu = EACTIVEMENU_NONE;
		}
		else menu = EACTIVEMENU_MAP;
		pageNum = EPAGENUMBER_ONE;
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1039, 21, 1039 + 85, 21 + 19))) {//ENEMY
		if (menu == EACTIVEMENU_ENEMY) {
			menu = EACTIVEMENU_NONE;
		}
		else menu = EACTIVEMENU_ENEMY;
		pageNum = EPAGENUMBER_ONE;
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1160, 21, 1160 + 85, 21 + 19))) {//OBJECT
		if (menu == EACTIVEMENU_OBJECT) {
			menu = EACTIVEMENU_NONE;
		}
		else menu = EACTIVEMENU_OBJECT;
		pageNum = EPAGENUMBER_ONE;
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(918, 56, 918 + 85, 56 + 19))) {//BOSS
		if (menu == EACTIVEMENU_BOSS) {
			menu = EACTIVEMENU_NONE;
		}
		else menu = EACTIVEMENU_BOSS;
		pageNum = EPAGENUMBER_ONE;
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1039, 56, 1039 + 85, 56 + 19))) { //DOOR
		if (menu == EACTIVEMENU_DOOR) {
			menu = EACTIVEMENU_NONE;
		}
		else menu = EACTIVEMENU_DOOR;
		pageNum = EPAGENUMBER_ONE;
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(920, 564, 920 + 85, 564 + 19))) {//COLLISION
		if (menu == EACTIVEMENU_COLLISION) {
			menu = EACTIVEMENU_NONE;
		}
		else menu = EACTIVEMENU_COLLISION;
		pageNum = EPAGENUMBER_NONE;
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1020, 564, 1020 + 85, 564 + 19))) {//COLLISION
		if (menu == EACTIVEMENU_COLLISIONRECT) {
			menu = EACTIVEMENU_NONE;
		}
		else menu = EACTIVEMENU_COLLISIONRECT;
		pageNum = EPAGENUMBER_NONE;
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1160, 56, 1160 + 85, 56 + 19)) && menu == EACTIVEMENU_OBJECT) { //DELETE OBJECT
		if (pageNum == EPAGENUMBER_NINE) {
			if (vElevator.size() != 0) {
				vElevator.pop_back();
			}
		}
		else {
			if (vObject.size() != 0) {
				vObject.pop_back();
			}
		}
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1160, 56, 1160 + 85, 56 + 19)) && menu == EACTIVEMENU_ENEMY) { //DELETE ENEMY
		if (vEnemy.size() != 0) {
			vEnemy.pop_back();
		}
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1160, 56, 1160 + 85, 56 + 19)) && menu == EACTIVEMENU_COLLISION) { //DELETE LINE
		if (vCollisionLine.size() != 0) {
			vCollisionLine.pop_back();
		}
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1160, 56, 1160 + 85, 56 + 19)) && menu == EACTIVEMENU_COLLISIONRECT) { //DELETE RECT
		if (vCollisionRect.size() != 0) {
			vCollisionRect.pop_back();
		}
	}
	else if (IsInRect(_ptMouse, D2D1::RectF(1160, 56, 1160 + 85, 56 + 19)) && menu == EACTIVEMENU_DOOR) { //DELETE DOOR
		if (vDoor.size() != 0) {
			vDoor.pop_back();
		}
	}

	//page click
	for (int i = 0; i < 9; ++i) {
		if (menu == EACTIVEMENU_NONE || menu == EACTIVEMENU_COLLISION) break;
		if (IsInRect(_ptMouse, D2D1::RectF(924 + i * 18, 82, 924 + i * 18 + 17, 82 + 27))) {
			pageNum = (EPAGENUMBER)(i + 1);
		}
	}

	//item click
	switch (menu)
	{
	case EACTIVEMENU_MAP:
		if (pageNum != EPAGENUMBER_NONE) {
			for (int k = 0; k < 9; ++k) {
				if (IsInRect(_ptMouse, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20))) {
					if (list[0][pageNum - 1][k].name != L"NONE DATA" && bg.kind != list[0][pageNum - 1][k].name) {
						bg.kind = list[0][pageNum - 1][k].name;
						_center = { WINSIZEX_NUM / 2, WINSIZEY_NUM / 2 };
						vCollisionRect.clear();
						vObject.clear();
						vEnemy.clear();
						vCollisionLine.clear();
						vDoor.clear();
						vElevator.clear();
						break;
					}
				}
			}
		}
		break;
	case EACTIVEMENU_ENEMY:
		for (int k = 0; k < 9; ++k) {
			if (IsInRect(_ptMouse, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20)) && pageNum != EPAGENUMBER_NONE) {
				if (list[1][pageNum - 1][k].name != L"NONE DATA") {
					inClickInfo.inEnemy.kind = EENEMYKIND_MADE;
					wsprintf(inClickInfo.inEnemy.name, list[1][pageNum - 1][k].name);
					inClickInfo.type = EITEMTYPE_ENEMY;
					break;
				}
			}
		}
		break;
	case EACTIVEMENU_OBJECT:
		for (int k = 0; k < 9; ++k) {
			if (pageNum == EPAGENUMBER_NINE) {
				//elevator
				if (IsInRect(_ptMouse, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20)) && pageNum != EPAGENUMBER_NONE) {
					if (list[2][pageNum - 1][k].name != L"NONE DATA") {
						wsprintf(inClickInfo.inElevator.name, list[2][pageNum - 1][k].name);
						inClickInfo.inElevator.state = EMAKEELEVATOR_NONE;
						inClickInfo.type = EITEMTYPE_ELEVATOR;
						switch (k)
						{
						case 2:
							inClickInfo.inElevator.kind = EELEVATORKIND_LASER;
							break;
						default:
							inClickInfo.inElevator.kind = EELEVATORKIND_SMALL;
							break;
						}
						break;
					}
				}
			}
			else {
				if (IsInRect(_ptMouse, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20)) && pageNum != EPAGENUMBER_NONE) {
					if (list[2][pageNum - 1][k].name != L"NONE DATA") {
						inClickInfo.inObject.kind = EOBJECTKIND_MADE;
						wsprintf(inClickInfo.inObject.name, list[2][pageNum - 1][k].name);
						inClickInfo.type = EITEMTYPE_OBJECT;
						break;
					}
				}
			}
		}
		break;
	case EACTIVEMENU_BOSS:
		for (int k = 0; k < 9; ++k) {
			if (IsInRect(_ptMouse, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20)) && pageNum != EPAGENUMBER_NONE) {
				if (list[3][pageNum - 1][k].name != L"NONE DATA") {
					inClickInfo.inEnemy.kind = EENEMYKIND_MADE;
					wsprintf(inClickInfo.inEnemy.name, list[3][pageNum - 1][k].name);
					inClickInfo.type = EITEMTYPE_ENEMY;
					break;
				}
			}
		}
		break;
	case EACTIVEMENU_DOOR:
			for (int k = 0; k < 9; ++k) {
				if (IsInRect(_ptMouse, D2D1::RectF(932, 121 + k * 20, 1262, 141 + k * 20)) && pageNum != EPAGENUMBER_NONE) {
					if (list[4][pageNum - 1][k].name != L"NONE DATA") {
						switch (k)
						{
						case 0:
							inClickInfo.inDoor.kind = EDOORKIND_BREAK;
							wsprintf(inClickInfo.inDoor.destination, L"PUT THE MAPNAME");
							inClickInfo.inDoor.dsX = inClickInfo.inDoor.dsY = 0;
							inClickInfo.type = EITEMTYPE_DOOR;
							break;
						case 1:
							inClickInfo.inDoor.kind = EDOORKIND_FREE;
							wsprintf(inClickInfo.inDoor.destination, L"PUT THE MAPNAME");
							inClickInfo.inDoor.dsX = inClickInfo.inDoor.dsY = 0;
							inClickInfo.type = EITEMTYPE_DOOR;
							break;
						case 2:
							inClickInfo.inDoor.kind = EDOORKIND_NEED_RED;
							wsprintf(inClickInfo.inDoor.destination, L"PUT THE MAPNAME");
							inClickInfo.inDoor.dsX = inClickInfo.inDoor.dsY = 0;
							inClickInfo.type = EITEMTYPE_DOOR;
							break;
						case 3:
							inClickInfo.inDoor.kind = EDOORKIND_NEED_BLUE;
							wsprintf(inClickInfo.inDoor.destination, L"PUT THE MAPNAME");
							inClickInfo.inDoor.dsX = inClickInfo.inDoor.dsY = 0;
							inClickInfo.type = EITEMTYPE_DOOR;
							break;
						case 4:
							inClickInfo.inDoor.kind = EDOORKIND_NEED_GREEN;
							wsprintf(inClickInfo.inDoor.destination, L"PUT THE MAPNAME");
							inClickInfo.inDoor.dsX = inClickInfo.inDoor.dsY = 0;
							inClickInfo.type = EITEMTYPE_DOOR;
							break;
						case 5:
							inClickInfo.inDoor.kind = EDOORKIND_NEED_WHITE;
							wsprintf(inClickInfo.inDoor.destination, L"PUT THE MAPNAME");
							inClickInfo.inDoor.dsX = inClickInfo.inDoor.dsY = 0;
							inClickInfo.type = EITEMTYPE_DOOR;
							break;
						case 6:
							inClickInfo.inDoor.kind = EDOORKIND_NEED_OBJECT;
							wsprintf(inClickInfo.inDoor.destination, L"PUT THE MAPNAME");
							inClickInfo.inDoor.dsX = inClickInfo.inDoor.dsY = 0;
							inClickInfo.type = EITEMTYPE_DOOR;
							break;
						}
						break;
					}
				}
			}
			if (inClickInfo.type == EITEMTYPE_DOOR) {
				if(IsInRect(_ptMouse, D2D1::RectF(1040, 310, WINSIZEX_NUM, 330))) {
					gameNode::settypingMode(true);
					typingMode = ETEXTINPUTMODE_DESTINATION;
				}
				if (IsInRect(_ptMouse, D2D1::RectF(1040, 330, WINSIZEX_NUM, 350))) {
					gameNode::settypingMode(true);
					typingMode = ETEXTINPUTMODE_DSX;
				}
				if (IsInRect(_ptMouse, D2D1::RectF(1040, 350, WINSIZEX_NUM, 370))) {
					gameNode::settypingMode(true);
					typingMode = ETEXTINPUTMODE_DSY;
				}
			}
			break;
	}

	//save, load
	if (IsInRect(_ptMouse, D2D1::RectF(922, 683, 922 + 85, 683 + 19))) {
		save();
	}
	if (IsInRect(_ptMouse, D2D1::RectF(1018, 683, 1018 + 85, 683 + 19))) {
		load(bg.kind);
	}
}

void mainGame::initList()
{
	//list[4][9][10];
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 9; ++j) {
			for (int k = 0; k < 9; ++k) {
				wsprintf(list[i][j][k].name, L"NONE DATA");
			}
		}
	}

	//map
	wsprintf(list[0][0][0].name, L"start_00");
	wsprintf(list[0][0][1].name, L"start_01");

	wsprintf(list[0][0][2].name, L"thePit_00");
	wsprintf(list[0][0][3].name, L"thePit_01");
	wsprintf(list[0][0][4].name, L"thePit_02");
	wsprintf(list[0][0][5].name, L"thePit_03");
	wsprintf(list[0][0][6].name, L"thePit_04");
	wsprintf(list[0][0][7].name, L"thePit_05");
	wsprintf(list[0][0][8].name, L"thePit_06");
	wsprintf(list[0][1][0].name, L"thePit_07");
	wsprintf(list[0][1][1].name, L"thePit_08");
	wsprintf(list[0][1][2].name, L"thePit_09");
	wsprintf(list[0][1][3].name, L"thePit_10");
	wsprintf(list[0][1][4].name, L"thePit_11");

	//enemy						   
	wsprintf(list[1][0][0].name, L"slime_green");
	wsprintf(list[1][0][1].name, L"slime_big");
	wsprintf(list[1][0][2].name, L"spider");
	wsprintf(list[1][0][3].name, L"egg");
	wsprintf(list[1][0][4].name, L"rat");
	wsprintf(list[1][0][5].name, L"slime_blue");
	wsprintf(list[1][0][6].name, L"slime_orange");
	wsprintf(list[1][0][7].name, L"slime_pink");
	wsprintf(list[1][0][8].name, L"slime_red");

	//object
	wsprintf(list[2][0][0].name, L"healmachine");
	wsprintf(list[2][0][1].name, L"upgrademachine");
	wsprintf(list[2][0][2].name, L"savemachine");
	wsprintf(list[2][0][3].name, L"box");
	wsprintf(list[2][0][4].name, L"depot");
	wsprintf(list[2][0][5].name, L"pc_1");
	wsprintf(list[2][0][6].name, L"electr_1");
	wsprintf(list[2][0][7].name, L"monitorv_1");
	wsprintf(list[2][0][8].name, L"monitorv_2");
	wsprintf(list[2][1][0].name, L"capsule_eo42");
	wsprintf(list[2][1][1].name, L"capsule_eo43");
	wsprintf(list[2][1][2].name, L"boxAbs");
	wsprintf(list[2][1][3].name, L"shelf");
	wsprintf(list[2][1][4].name, L"cctv");
	wsprintf(list[2][1][5].name, L"depot_boots");
	wsprintf(list[2][1][6].name, L"depot_card_red");
	wsprintf(list[2][1][7].name, L"depot_card_white");
	wsprintf(list[2][1][8].name, L"depot_card_green");
	wsprintf(list[2][2][0].name, L"depot_card_blue");
	wsprintf(list[2][2][1].name, L"health");
	wsprintf(list[2][2][2].name, L"upgrade");
	wsprintf(list[2][2][3].name, L"bullet");
	wsprintf(list[2][2][4].name, L"drain");

	wsprintf(list[2][6][0].name, L"waterTop");
	wsprintf(list[2][6][1].name, L"waterBottom");

	wsprintf(list[2][7][0].name, L"escalator");
	wsprintf(list[2][7][1].name, L"firePillar");
	wsprintf(list[2][7][2].name, L"spike");
	wsprintf(list[2][7][3].name, L"spike_up");
	wsprintf(list[2][7][4].name, L"spike_right");
	wsprintf(list[2][7][5].name, L"spike_left");
	wsprintf(list[2][7][6].name, L"press");

	wsprintf(list[2][8][0].name, L"lift_small");
	wsprintf(list[2][8][1].name, L"lift_middle");
	wsprintf(list[2][8][2].name, L"laser");

	//door
	wsprintf(list[4][0][0].name, L"door_break");
	wsprintf(list[4][0][1].name, L"door_free");
	wsprintf(list[4][0][2].name, L"door_red");
	wsprintf(list[4][0][3].name, L"door_blue");
	wsprintf(list[4][0][4].name, L"door_green");
	wsprintf(list[4][0][5].name, L"door_white");
	wsprintf(list[4][0][6].name, L"door_object");

	wsprintf(list[3][0][0].name, L"1st_boss");
}

void mainGame::imageLoad()
{
	//기본UI배치
	IMAGEMANAGER->addImage(Rt, L"mainBG", L"images/UI/toolBGn.png", 0, 0, WINSIZEX_NUM, WINSIZEY_NUM, false);
	for (int i = 0; i < 9; ++i) {
		tchar tmp[10];
		tchar tmp2[30];
		wsprintf(tmp, L"%dbutton", i + 1);
		wsprintf(tmp2, L"images/UI/%dbutton.png", i + 1);
		IMAGEMANAGER->addImage(Rt, tmp, tmp2, 924 + i*18, 82, 17, 27, false);
	}
	IMAGEMANAGER->addImage(Rt, L"mapbutton", L"images/UI/mapbutton.png", 918, 21, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"enemybutton", L"images/UI/enemybutton.png", 1039, 21, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"objectbutton", L"images/UI/objectbutton.png", 1160, 21, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"bossbutton", L"images/UI/bossbutton.png", 918, 56, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"doorbutton", L"images/UI/doorbutton.png", 1039, 56, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"deletebutton", L"images/UI/deletebutton.png", 1160, 56, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"rectbutton", L"images/UI/rectbutton.png", 1020, 564, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"collisionbutton", L"images/UI/collisionbutton.png", 920, 564, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"savebutton", L"images/UI/savebutton.png", 918, 683, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"loadbutton", L"images/UI/loadbutton.png", 1018, 683, 85, 19, false);
	IMAGEMANAGER->addImage(Rt, L"newpointbutton", L"images/UI/newpointbutton.png", 1172, 592, 85, 19, false);

	//맵
	IMAGEMANAGER->addImage(Rt, L"start_00", L"images/map/start/start_00.png", 0, 0, 1024, 512, false);
	IMAGEMANAGER->addImage(Rt, L"start_01", L"images/map/start/start_01.png", 0, 0, 576, 736, false);

	IMAGEMANAGER->addImage(Rt, L"thePit_00", L"images/map/thePit/the_pit_00_complete.png", 0, 0, 6400, 2848, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_01", L"images/map/thePit/the_pit_01_complete.png", 0, 0, 960, 544, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_02", L"images/map/thePit/the_pit_02_complete.png", 0, 0, 768, 544, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_03", L"images/map/thePit/the_pit_03_complete.png", 0, 0, 768, 544, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_04", L"images/map/thePit/the_pit_04_complete.png", 0, 0, 896, 352, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_05", L"images/map/thePit/the_pit_05_complete.png", 0, 0, 736, 448, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_06", L"images/map/thePit/the_pit_06_complete.png", 0, 0, 768, 736, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_07", L"images/map/thePit/the_pit_07_complete.png", 0, 0, 768, 544, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_08", L"images/map/thePit/the_pit_08_complete.png", 0, 0, 448, 256, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_09", L"images/map/thePit/the_pit_09_complete.png", 0, 0, 1728, 704, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_10", L"images/map/thePit/the_pit_10_complete.png", 0, 0, 2240, 3872, false);
	IMAGEMANAGER->addImage(Rt, L"thePit_11", L"images/map/thePit/the_pit_11_complete.png", 0, 0, 1888, 608, false);
	
	//오브젝트
	IMAGEMANAGER->addImage(Rt, L"healmachine"	, L"images/object/heal_3.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 52, 60, true);
	IMAGEMANAGER->addImage(Rt, L"upgrademachine", L"images/object/upgrade_3.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 52, 60, true);
	IMAGEMANAGER->addImage(Rt, L"savemachine"	, L"images/object/save_zone1.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 40, 60, true);
	IMAGEMANAGER->addImage(Rt, L"depot",			L"images/object/box_9.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 36, 22, true);
	IMAGEMANAGER->addImage(Rt, L"depot_boots", L"images/object/box_9.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 36, 22, true);
	IMAGEMANAGER->addImage(Rt, L"depot_card_red", L"images/object/box_9.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 36, 22, true);
	IMAGEMANAGER->addImage(Rt, L"depot_card_white", L"images/object/box_9.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 36, 22, true);
	IMAGEMANAGER->addImage(Rt, L"depot_card_green", L"images/object/box_9.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 36, 22, true);
	IMAGEMANAGER->addImage(Rt, L"depot_card_blue", L"images/object/box_9.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 36, 22, true);

	IMAGEMANAGER->addImage(Rt, L"cctv", L"images/object/vid_cam5.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 20, 20, true);
	IMAGEMANAGER->addImage(Rt, L"shelf", L"images/object/env_2.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 25, 32, true);
	IMAGEMANAGER->addImage(Rt, L"boxAbs", L"images/object/box_7.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 34, 23, true);
	IMAGEMANAGER->addImage(Rt, L"box"			, L"images/object/box_0.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 36, 22, true);
	IMAGEMANAGER->addImage(Rt, L"pc_1"			, L"images/object/pc_1.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 54, 31, true);
	IMAGEMANAGER->addImage(Rt, L"electr_1"		, L"images/object/electr1.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 47, 46, true);
	IMAGEMANAGER->addImage(Rt, L"monitorv_1"	, L"images/object/monitorv1_7_2.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 32, 32, true);
	IMAGEMANAGER->addImage(Rt, L"monitorv_2"	, L"images/object/monitorv8.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 34, 23, true);
	IMAGEMANAGER->addImage(Rt, L"capsule_eo42", L"images/object/capsule_eo42_1.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 112, 68, true);
	IMAGEMANAGER->addImage(Rt, L"capsule_eo43", L"images/object/capsule_eo43_1.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 112, 69, true);
	IMAGEMANAGER->addImage(Rt, L"drain", L"images/object/drain.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 28, 16, true);

	IMAGEMANAGER->addImage(Rt, L"firePillar", L"images/object/fire_pillar_9.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 32, 96, true);
	IMAGEMANAGER->addImage(Rt, L"spike", L"images/object/spikes.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 32, 16, true);
	IMAGEMANAGER->addImage(Rt, L"spike_up", L"images/object/spikes_up.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 32, 16, true);
	IMAGEMANAGER->addImage(Rt, L"spike_right", L"images/object/spikes_right.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 16, 32, true);
	IMAGEMANAGER->addImage(Rt, L"spike_left", L"images/object/spikes_left.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 16, 32, true);
	IMAGEMANAGER->addImage(Rt, L"waterTop", L"images/object/waterTop2_2_x96.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 96, 96, true);
	IMAGEMANAGER->addImage(Rt, L"waterBottom", L"images/object/waterTop2_1_x96.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 96, 96, true);
	IMAGEMANAGER->addImage(Rt, L"press", L"images/object/press_0.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 62, 96, true);
	
	IMAGEMANAGER->addImage(Rt, L"door_break", L"images/object/doorCantOpen.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 56, 53, true);
	IMAGEMANAGER->addImage(Rt, L"door_free", L"images/object/doorCantOpen.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 56, 53, true);
	IMAGEMANAGER->addImage(Rt, L"door_red", L"images/object/doorCantOpen.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 56, 53, true);
	IMAGEMANAGER->addImage(Rt, L"door_green", L"images/object/doorCantOpen.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 56, 53, true);
	IMAGEMANAGER->addImage(Rt, L"door_blue", L"images/object/doorCantOpen.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 56, 53, true);
	IMAGEMANAGER->addImage(Rt, L"door_white", L"images/object/doorCantOpen.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 56, 53, true);
	IMAGEMANAGER->addImage(Rt, L"door_object", L"images/object/vent.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 34, 33, true);

	IMAGEMANAGER->addImage(Rt, L"escalator", L"images/object/as_line1.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 32, 5, true);

	IMAGEMANAGER->addImage(Rt, L"panel", L"images/object/panel.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 19, 29, true);
	IMAGEMANAGER->addImage(Rt, L"lift_small", L"images/object/lift.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 32, 11, true);
	IMAGEMANAGER->addImage(Rt, L"lift_middle", L"images/object/lift_x96.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 96, 11, true);
	IMAGEMANAGER->addImage(Rt, L"laser", L"images/object/laser.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 9, 18, true);

	IMAGEMANAGER->addImage(Rt, L"health", L"images/object/health.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 30, 15, true);
	IMAGEMANAGER->addImage(Rt, L"upgrade", L"images/object/upgrade.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 30, 15, true);
	IMAGEMANAGER->addImage(Rt, L"bullet", L"images/object/bullet.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 30, 15, true);


	//에너미
	IMAGEMANAGER->addImage(Rt, L"slime_big", L"images/enemy/slime_big.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 38, 35, true);
	IMAGEMANAGER->addImage(Rt, L"slime_blue", L"images/enemy/slime_blue.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 31, 26, true);
	IMAGEMANAGER->addImage(Rt, L"slime_green", L"images/enemy/slime_green.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 30, 26, true);
	IMAGEMANAGER->addImage(Rt, L"slime_orange", L"images/enemy/slime_orange.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 30, 27, true);
	IMAGEMANAGER->addImage(Rt, L"slime_pink", L"images/enemy/slime_pink.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 30, 27, true);
	IMAGEMANAGER->addImage(Rt, L"slime_red", L"images/enemy/slime_red.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 34, 30, true);
	IMAGEMANAGER->addImage(Rt, L"spider", L"images/enemy/spider.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 52, 31, true);
	IMAGEMANAGER->addImage(Rt, L"rat", L"images/enemy/rat.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 41, 32, true);
	IMAGEMANAGER->addImage(Rt, L"egg", L"images/enemy/egg.png", WINSIZEX_NUM / 2, WINSIZEY_NUM / 2, 29, 26, true);

	//보스
	IMAGEMANAGER->addImage(Rt, L"1st_boss", L"images/boss/1st_boss.png", 0, 0, 640, 125, true);
}

void mainGame::dropResource()
{
	switch (inClickInfo.type)
	{
	case EITEMTYPE_OBJECT:
		SOBJECT tmpObj;
		tmpObj.x = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
		tmpObj.y = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
		wsprintf(tmpObj.name, inClickInfo.inObject.name);
		tmpObj.kind = inClickInfo.inObject.kind;
		vObject.push_back(tmpObj);
		break;
	case EITEMTYPE_ENEMY:
		SENEMY tmpEnemy;
		tmpEnemy.x = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
		tmpEnemy.y = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
		wsprintf(tmpEnemy.name, inClickInfo.inEnemy.name);
		tmpEnemy.kind = inClickInfo.inEnemy.kind;
		vEnemy.push_back(tmpEnemy);
		break;
	case EITEMTYPE_DOOR:
		if (inClickInfo.inDoor.destination != L"PUT THE MAPNAME" && inClickInfo.inDoor.dsX != 0.0f && inClickInfo.inDoor.dsY != 0.0f) {
			SDOOR tmpDoor;
			wsprintf(tmpDoor.destination, inClickInfo.inDoor.destination);
			tmpDoor.dsX = inClickInfo.inDoor.dsX;
			tmpDoor.dsY = inClickInfo.inDoor.dsY;
			tmpDoor.kind = inClickInfo.inDoor.kind;
			tmpDoor.x = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
			tmpDoor.y = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
			vDoor.push_back(tmpDoor);
		}
		break;
	case EITEMTYPE_ELEVATOR:
		SELEVATOR tmpEle;
		if (inClickInfo.inElevator.kind == EELEVATORKIND_SMALL) {
			switch (inClickInfo.inElevator.state)
			{
			case EMAKEELEVATOR_NONE:
				inClickInfo.inElevator.state = EMAKEELEVATOR_STARTSET;
				tmpEle.state = EMAKEELEVATOR_STARTSET;
				tmpEle.kind = inClickInfo.inElevator.kind;
				wsprintf(tmpEle.name, inClickInfo.inElevator.name);
				tmpEle.startX = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
				tmpEle.startY = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
				vElevator.push_back(tmpEle);
				break;
			case EMAKEELEVATOR_STARTSET:
				vElevator.at(vElevator.size() - 1).endX = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
				vElevator.at(vElevator.size() - 1).endY = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
				vElevator.at(vElevator.size() - 1).state = EMAKEELEVATOR_ENDSET;
				inClickInfo.inElevator.state = EMAKEELEVATOR_ENDSET;
				break;
			case EMAKEELEVATOR_ENDSET:
				vElevator.at(vElevator.size() - 1).switchPosition[0] = { (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
					(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom) };
				vElevator.at(vElevator.size() - 1).state = EMAKEELEVATOR_SWITCHSET1;
				inClickInfo.inElevator.state = EMAKEELEVATOR_SWITCHSET1;
				break;
			case EMAKEELEVATOR_SWITCHSET1:
				vElevator.at(vElevator.size() - 1).switchPosition[1] = { (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
					(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom) };
				vElevator.at(vElevator.size() - 1).state = EMAKEELEVATOR_SWITCHSET2;
				inClickInfo.inElevator.state = EMAKEELEVATOR_NONE;
				break;
			}
		}
		else {
			switch (inClickInfo.inElevator.state)
			{
			case EMAKEELEVATOR_NONE:
				inClickInfo.inElevator.state = EMAKEELEVATOR_STARTSET;
				tmpEle.state = EMAKEELEVATOR_STARTSET;
				tmpEle.kind = inClickInfo.inElevator.kind;
				wsprintf(tmpEle.name, inClickInfo.inElevator.name);
				tmpEle.startX = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
				tmpEle.startY = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
				vElevator.push_back(tmpEle);
				break;
			case EMAKEELEVATOR_STARTSET:
				vElevator.at(vElevator.size() - 1).endX = (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom);
				vElevator.at(vElevator.size() - 1).endY = (_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom);
				vElevator.at(vElevator.size() - 1).state = EMAKEELEVATOR_ENDSET;
				inClickInfo.inElevator.state = EMAKEELEVATOR_ENDSET;
				break;
			case EMAKEELEVATOR_ENDSET:
				vElevator.at(vElevator.size() - 1).switchPosition[0] = { (_ptMouse.x - CAMERAMANAGER->getAbsPosition({ 0,0 }).x) / (IMAGEMANAGER->findImage(bg.kind)->getWidth()*_zoom),
					(_ptMouse.y - CAMERAMANAGER->getAbsPosition({ 0,0 }).y) / (IMAGEMANAGER->findImage(bg.kind)->getHeight()*_zoom) };
				vElevator.at(vElevator.size() - 1).state = EMAKEELEVATOR_SWITCHSET1;
				inClickInfo.inElevator.state = EMAKEELEVATOR_NONE;
				break;
			}
			break;
		}
	}
}