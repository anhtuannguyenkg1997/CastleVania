#pragma once
#include "Item.h"


class MoneyBagPurpleItem : public Item
{
public:
	MoneyBagPurpleItem(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

