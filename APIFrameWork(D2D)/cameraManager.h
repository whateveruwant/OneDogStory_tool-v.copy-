#pragma once
#include "gameNode.h"
#include "singletonBase.h"
class cameraManager :public singletonBase<cameraManager>
{
private:
	D2D1_POINT_2F* _standard;

public:
	cameraManager();
	~cameraManager();

	HRESULT init();
	void release();
	void update();
	void render();

	D2D1_POINT_2F getAbsPosition(const D2D1_POINT_2F _pt);

	void setPositionMemotyRick(D2D1_POINT_2F* _pt) { _standard = _pt; }
	D2D1_POINT_2F getStandardPoint() { return *_standard; }
};