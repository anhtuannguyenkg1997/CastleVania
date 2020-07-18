#pragma once
#include "SubWeapon.h"

class AxeSubWeapon : public SubWeapon
{
public:
	AxeSubWeapon(Simon* simon);
	~AxeSubWeapon() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);

	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

