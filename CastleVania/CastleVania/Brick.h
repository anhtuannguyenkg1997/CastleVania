#pragma once

#include "CGameObject.h"


class Brick : public CGameObject
{
public:

	//Constructor
	Brick(float x, float y, int Type, bool isActive, int State);

	//Render
	virtual void Render();

	//Lấy vùng bao của Brick
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	//Update brick theo dt
	void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects);


	//Set trạng thái của Brick
	void SetState(int state);
};

