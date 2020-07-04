#include "Brick.h"

Brick::Brick(float x, float y, int Type, bool isActive, int State) : CGameObject(x, y, Type, isActive, State)
{
}


void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects) {

	//Nếu brick bị phá và thời gian render hiệu ứng biến mất kết thúc
	if (state == STATIC_OBJECT_DESTROYED && animation_set->at(state)->isFinishAnimation(BROKEN_BLOCK_TIME)) {

		//BRICK biến mất
		setActive(false);
	}


}

void Brick::Render()
{
	//Không render BOSS_BRICK, ENEMY_BRICK  và NOT_RENDER
	if (type != BOSS_BRICK && type!= ENEMY_BRICK && type != NOT_RENDER) {
		animation_set->at(state)->Render(INSIDE_CAMERA, false, NOT_FLIPPING, x, y);
	}

}



void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BRICK_WIDTH;


	//Nếu là loại SLIM_BRICK hoặc NOT_RENDER thì ta giảm boundingbox bottom xuống
	if (type == SLIM_BRICK || type == NOT_RENDER) {
		b = t + 2;
	}
	//Nếu không ta lấy đúng bằng chiều cao của brick
	else {
		b = t + BRICK_HEIGHT;
	}

}




void Brick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	//Active
	case STATIC_OBJECT_ACTIVE:
		vx = vy = 0;
		break;

	//Destroy
	case STATIC_OBJECT_DESTROYED:
		//Set thời gian để bắt đầu render hiệu ứng break
 		animation_set->at(state)->startCountTimeAnimation(GetTickCount());

		//Dời tọa độ để render hiệu ứng vỡ (làm nó bung ra )
		x -= BROKEN_EFFECT_X;
		y -= BROKEN_EFFECT_Y;
		break;
	default:
		break;
	}
}