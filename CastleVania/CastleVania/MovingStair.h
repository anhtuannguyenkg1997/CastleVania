#pragma once
#include "CGameObject.h"
#include "Simon.h"

class MovingStair : public CGameObject
{
	float movingRegionLeft, movingRegionRight;
public:
	MovingStair(float x, float y, int Type, bool isActive, int State);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject){}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon);


	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon);

	void SetMovingRegionRight(float movingRegionRight) { this->movingRegionRight = movingRegionRight; }
	void SetMovingRegionLeft(float movingRegionLeft) { this->movingRegionLeft = movingRegionLeft; }
};
