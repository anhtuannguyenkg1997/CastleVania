#include "Solider.h"



Solider::Solider() : Enemy()
{
	HP = SOLIDER_HP;
}





void Solider::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	//Gọi đến update của CGameObject
	Enemy::Update(dt);

	//Cập nhật vị trí solider theo trục X 
	x += dx;


	//Giới hạn đường đi của Solider 
	//Nếu tới giới hạn thì quay ngược lại
	if (x > SOLIDER_REGION_RIGHT) {
		vx = -vx;
		orientation = -orientation;
	}else if (x < SOLIDER_REGION_LEFT) {
		vx = -vx;
		orientation = -orientation;
	}


	//Nếu Solider bị tiêu diệt
	if (state == SOLIDER_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) 
		this->active = false;

}


//Vẽ Solider
void Solider::Render()
{
	animation_set->at(state)->Render(DEPEND_ON_CAMERA, orientation, x, y);

}



void Solider::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case SOLIDER_WALKING:
		if (orientation == OR_PLUS) vx = SOLIDER_WALKING_SPEED;	//Solider đang quay theo chiều dương
		else vx = -SOLIDER_WALKING_SPEED;	//Solider đang quay theo chiều âm
		isDroppedItem = false;	//Có khả năng tạo ra item khi bị tiêu diệt
		break;
	case SOLIDER_DESTROYED:
		vx = 0;
		//Set thời gian hiệu ứng phá hủy
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}



//Lấy vùng bao của Solider
void Solider::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SOLIDER_WIDTH;
	bottom = top + SOLIDER_HEIGHT;
}



//Tính lượng máu còn lại của Solider
void Solider::LoseHP(int x)
{
	Enemy::LoseHP(x);

	//Solider không còn máu
	if (HP == 0)
		//Trạng thái render hiệu ứng biến mất
		SetState(SOLIDER_DESTROYED);


	DebugOut(L"HP = %d \n", HP);
}