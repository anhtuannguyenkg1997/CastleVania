#include "Candle.h"

Candle::Candle(float x, float y, int Type, bool isActive, int State) : CGameObject(x, y, Type, isActive, State)
{

}



void Candle::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects)
{
	//Nếu Candle bị phá và thời gian render hiệu ứng biến mất kết thúc
	if (state == STATIC_OBJECT_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true)
		setActive(false);

}




void Candle::Render()
{
	animation_set->at(state)->Render(INSIDE_CAMERA, false, orientation, x, y);
}



void Candle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	//Active
	case STATIC_OBJECT_ACTIVE:
		break;

	//Destroy
	case STATIC_OBJECT_DESTROYED:
		//Set thời gian để bắt đầu render hiệu ứng break
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
	case STATIC_OBJECT_ACTIVE:
		right = left + CANDLE_WIDTH;
		bottom = top + CANDLE_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}