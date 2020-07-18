#include "Bone.h"
#include "StairManager.h"
#include "PlayerManager.h"
#include "SubWeaponManager.h"

Bone::Bone() {

	//Lấy Animation set trong danh sách AnimationSet
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	//Dựa vào ID 
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_BONE);

	//Set animationSet cho Object này
	SetAnimationSet(ani_set);

	//Vận tốc bay của Bone 
	vy = -BONE_FLYING_SPEED_Y;

}


void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	CGameObject::Update(dt, listObjects);



	//object có khả năng va chạm với Bone
	vector<LPGAMEOBJECT> coObject;

	GetCollisionObject(listObjects, coObject, simon);

	//Simple falldown
	//Làm cho Bone rơi
	vy += BONE_GRAVITY * dt;


	//Nếu đang hướng về chiều dương 
	if (orientation == OR_PLUS)
		//Đặt vận tốc Bone dương (bay qua phải)
		vx = BONE_FLYING_SPEED_X;
	//Nếu không
	else
		//Đặt vận tốc Bone âm (bay qua trái)
		vx = -BONE_FLYING_SPEED_X;



	//Cập nhật tọa độ mới của Bone
	x += dx;
	y += dy;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	//Lấy ra objects tiềm năng có thể va chạm đưa vào coEvents
	CalcPotentialCollisions(&coObject, coEvents);



	// kiểm tra va chạm với object
	float min_tx, min_ty, nx = 0, ny;
	float rdx = 0;
	float rdy = 0;



	//Tìm object thực sự va chạm với Bone từ list coEvents
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);



	//Kiểm tra xem Bone va chạm với object nào 
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];


		if (dynamic_cast<Simon*>(e->obj))
		{
			//Nếu không bất tử thì mới xét va chạm với AXE
			simon->Attacked(BONE_ATK);	
		}

	}


	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];


}




void Bone::Render()
{
	animation_set->at(ENEMY_WORKING)->Render(INSIDE_CAMERA, SubWeaponManager::GetInstance()->isUseStopWatch, orientation, x, y);
}





//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với Bone gán vào coObjects
void Bone::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {
	StairManager* stairManager = StairManager::GetInstance();

	//Nếu simon đang đứng trên stair và đang trong trạng thái bất tử thì không đưa nó vào list va chạm
	if (!stairManager->GetIsSimonStandingOnStair() && !simon->getIsImmortal()) {
		coObjects.push_back(simon); 
	}


}


void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	right = left + BONE_WIDTH;
	bottom = top + BONE_HEIGHT;

}

