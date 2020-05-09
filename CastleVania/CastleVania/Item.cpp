#include "Item.h"
#include "Torch.h"
#include "Candle.h"
#include "Solider.h"

Item::Item(LPGAMEOBJECT object) : CGameObject()
{
	timeAppear = -1;

	int idItem=-1;

	float x=0, y=0;

	if (object->IsDroppedItem() == true)
		return;


	if (dynamic_cast<Torch*>(object) && object->GetState() == TORCH_DESTROYED)
	{
		idItem = object->idItem;
		object->GetPosition(x, y);
		object->SetIsDroppedItem(true);
	}

	if (dynamic_cast<Candle*>(object) && object->GetState() == CANDLE_DESTROYED)
	{
		idItem = object->idItem;
		object->GetPosition(x, y);
		object->SetIsDroppedItem(true);
	}

	if (dynamic_cast<Solider*>(object) && object->GetState() == SOLIDER_DESTROYED)
	{
		idItem = object->idItem;
		object->GetPosition(x, y);
		object->SetIsDroppedItem(true);
	}


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_ITEMS);


	SetAnimationSet(ani_set);
	Active(true);
	SetPosition(x, y);
	SetState(idItem);


}

void Item::Render()
{
	int alpha = 255;

	if (GetTickCount() - timeAppear > ITEM_TIME_DISAPEAR / 2)
		alpha = rand() % 255 + 0;

	animation_set->at(state)->Render(DEPEND_ON_CAMERA, -1, x, y, alpha);
}


//Xét va chạm của Item
void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

	//Kiểm tra Item còn thời gian xuất hiện hay không???
	if (timeAppear == -1)
		timeAppear = GetTickCount();
	else
	{
		DWORD now = GetTickCount();

		if (now - timeAppear > ITEM_TIME_DISAPEAR)
		{
			active = false;
			return;
		}
	}


	CGameObject::Update(dt);

	// Check collision between item and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		y += min_ty * dy + ny * 0.1f;
		if (ny != 0)
		{
			vx = 0;
			vy = 0;
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


//Lấy vùng bao của từng item khác nhau
void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item

	switch (state)
	{
	case ITEM_STOPWATCH:
		right = left + 30;
		bottom = top + 32;
		break;
	case ITEM_DAGGER:
		right = left + 32;
		bottom = top + 18;
		break;
	case ITEM_AXE:
		right = left + 30;
		bottom = top + 28;
		break;
	case ITEM_HOLYWATER:
		right = left + 32;
		bottom = top + 32;
		break;
	case ITEM_BOOMERANG:
		right = left + 30;
		bottom = top + 28;
		break;
	case ITEM_SMALLHEART:
		right = left + 16;
		bottom = top + 26;
		break;
	case ITEM_BIGHEART:
		right = left + 24;
		bottom = top + 20;
		break;
	case ITEM_ROSARY:
		right = left + 32;
		bottom = top + 32;
		break;
	case ITEM_INVISIBILITYPOTION:
		right = left + 26;
		bottom = top + 36;
		break;
	case ITEM_MORNINGSTAR:
		right = left + 32;
		bottom = top + 32;
		break;
	case ITEM_MONEYBAG:
	case ITEM_MONEYBAG_RED:
	case ITEM_MONEYBAG_PURPLE:
	case ITEM_MONEYBAG_WHITE:
		right = left + 30;
		bottom = top + 30;
		break;
	case ITEM_DOUBLESHOT:
	case ITEM_TRIPLESHOT:
		right = left + 28;
		bottom = top + 28;
		break;
	case ITEM_PORKCHOP:
		right = left + 32;
		bottom = top + 26;
		break;
	case ITEM_CHEST:
		right = left + 32;
		bottom = top + 24;
		break;
	case ITEM_DIADEM:
		right = left + 30;
		bottom = top + 32;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void Item::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	default:
		vx = 0;
		vy = ITEM_FALLING_Y;
		break;
	}
}




