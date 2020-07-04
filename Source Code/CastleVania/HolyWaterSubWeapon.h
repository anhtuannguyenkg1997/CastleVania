#pragma once
#include "SubWeapon.h"
#include "Simon.h"


class HolyWaterSubWeapon: public SubWeapon
{
	bool isBurning = false;			//cờ xác định trạng thái đang cháy của weapon này
	DWORD startBurningTime = 0;			//Thời gian bắt đầu hiệu ứng cháy

public:
	HolyWaterSubWeapon(Simon* simon);
	~HolyWaterSubWeapon() {};


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);

	virtual void Render();


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);


};

