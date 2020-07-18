#pragma once
#include "Item.h"

class MorningStarItem : public Item
{
public:
	MorningStarItem(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

