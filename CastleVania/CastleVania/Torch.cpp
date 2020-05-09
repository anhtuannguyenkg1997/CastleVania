#include "Torch.h"


Torch::Torch() : CGameObject()
{
	SetState(TORCH_ACTIVE);
}

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == TORCH_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) 
		this->active = false;

}

void Torch::Render()
{
	animation_set->at(state)->Render(DEPEND_ON_CAMERA, orientation, x, y);
}

void Torch::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case TORCH_ACTIVE:
		break;
	case TORCH_DESTROYED:
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
	case TORCH_ACTIVE:
		right = left + TORCH_ACTIVE_WIDTH;
		bottom = top + TORCH_ACTIVE_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}