#include "Torch.h"

Torch::Torch(float x, float y, int Type, bool isActive, int State) : CGameObject(x, y, Type, isActive, State)
{

}


void Torch::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects)
{
	if (state == STATIC_OBJECT_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) 
		this->active = false;

}

void Torch::Render()
{
	animation_set->at(state)->Render(INSIDE_CAMERA, false, orientation, x, y);
}

void Torch::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STATIC_OBJECT_ACTIVE:
		break;
	case STATIC_OBJECT_DESTROYED:
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}

void Torch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case STATIC_OBJECT_ACTIVE:
		right = left + TORCH_WIDTH;
		bottom = top + TORCH_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}