#pragma once
#include "Item.h"

class DoubleShotItem : public Item
{
public:
	DoubleShotItem(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

