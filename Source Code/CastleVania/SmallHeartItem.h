#pragma once
#include "Item.h"

class SmallHeartItem : public Item
{
	float deltaVX;	//Biến đổi vận tốc của quả tim theo trục X
public:
	SmallHeartItem(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

