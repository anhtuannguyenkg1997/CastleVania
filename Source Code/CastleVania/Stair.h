#pragma once
#include "CGameObject.h"

class Stair : public CGameObject
{
	//break point của bậc thang. Xác định đầu thang (1), chân thang (-1), bậc thang thông thường (0)
	int breakPoint = 0;		
public:
	Stair(float x, float y, int Type, bool isActive, int State);

	void SetBreakPoint(int breakPoint) { this->breakPoint = breakPoint; }
	int GetBreakPoint() { return breakPoint; }

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
