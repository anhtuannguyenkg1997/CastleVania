#include "Stair.h"

Stair::Stair(float x, float y, int Type, bool isActive, int State):CGameObject(x, y, Type, isActive, State)
{
}


void Stair::Render()
{
	//Không render những stair break point = 0 tức là những bậc thang thông thường
	if (breakPoint == 0) {
		animation_set->at(state)->Render(INSIDE_CAMERA, false, NOT_FLIPPING, x, y);
	}
	
	//RenderBoundingBox();
}


void Stair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + STAIR_WIDTH;
	b = t + STAIR_HEIGHT;
}