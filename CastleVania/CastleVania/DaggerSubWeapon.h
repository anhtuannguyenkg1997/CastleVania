#pragma once
#include "SubWeapon.h"

class DaggerSubWeapon : public SubWeapon
{
public:
	DaggerSubWeapon(Simon* simon);
	~DaggerSubWeapon() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);

	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

