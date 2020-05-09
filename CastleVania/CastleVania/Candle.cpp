#include "Candle.h"



Candle::Candle() : CGameObject()
{
	SetState(TORCH_ACTIVE);
}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == TORCH_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true)
		this->active = false; 

}

void Candle::Render()
{
	animation_set->at(state)->Render(DEPEND_ON_CAMERA, orientation, x, y);
}

void Candle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CANDLE_ACTIVE:
		break;
	case CANDLE_DESTROYED:
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}

void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case CANDLE_ACTIVE:
		right = left + CANDLE_ACTIVE_WIDTH;
		bottom = top + CANDLE_ACTIVE_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}