#pragma once
#include "SubWeapon.h"

class BoomerangSubWeapon : public SubWeapon
{
public:
	BoomerangSubWeapon(Simon* simon);
	~BoomerangSubWeapon() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);

	virtual void Render();

	bool CheckCollision(float otherLeft, float otherTop, float otherRight, float otherBottom);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

