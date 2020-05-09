#include "Brick.h"


Brick::Brick() : CGameObject()
{
}

void Brick::Render()
{
	animation_set->at(state)->Render(1, -1, x, y);
	RenderBoundingBox();
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + GROUND_BBOX_WIDTH;
	b = t + GROUND_BBOX_HEIGHT;
}