#include "HolyWaterSubWeapon.h"
#include "CObjectCell.h"
#include "HiddenObjectManager.h"
#include "SoundManager.h"


HolyWaterSubWeapon::HolyWaterSubWeapon(Simon* simon) :SubWeapon(simon)
{
	//Nếu đang hướng về chiều dương
	if (orientation == OR_PLUS)
		//Đặt vận tốc AXE dương
		vx = WEAPONS_HOLY_WATER_SPEED_X;
	//Nếu không
	else
		//Đặt vận tốc AXE âm
		vx = -WEAPONS_HOLY_WATER_SPEED_X;


	//Vận tốc bay của lọ HolyWater (lúc simon quăng nó ra)
	vy = -WEAPONS_HOLY_WATER_SPEED_Y;


	//Trạng thái render là cái lọ 
	state = W_HOLYWATER_BOTTLE_STATE;

	//Lượng damage mà Holywater gây ra
	damage = W_NORMAL_DAMAGE;

}




//Xét va chạm giữa subweapon và các Object khác
void HolyWaterSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Lấy instance của SubWeaponManager để tính số subweapon còn lại
	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();

	//Gọi hàm update của CGameObject
	CGameObject::Update(dt, listObjects);


	//Nếu trạng thái của subweapon này đang là đang cháy
	//Và thời gian render hiệu ứng cháy đã hết
	if (isBurning &&
		GetTickCount() - startBurningTime > WEAPONS_HOLY_WATER_TIME_EFFECT)
	{
		isBurning = false;							//Tắt trạng thái đang cháy
		setActive(false);							//Không hiển thị nó lên màn hình
		subweaponManager->numSubWeaponCreated--;	//Giảm số lượng subweapon còn lại của simon
		return;
	}


	vector<LPGAMEOBJECT> coObject;

	//Lấy những object có thể va chạm với HolyWater
	GetCollisionObject(listObjects, coObject);

	//Nếu subweapon còn đang ở trạng thái là cái lọ => cần cho nó rơi xuống
	if (state == W_HOLYWATER_BOTTLE_STATE) {
		//Simple fall down
		vy += WEAPONS_HOLY_WATER_GRAVITY * dt;
	}
	else {
		//Lúc này đang ở trạng thái "vỡ"
		//=>Vận tốc trên các trục bằng 0
		vx = 0;
		vy = 0;
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	//Lấy object có khả năng va chạm với HolyWater
	CalcPotentialCollisions(&coObject, coEvents);


	//Nếu không có vật nào va chạm với HolyWater
	if (coEvents.size() == 0)
	{
		//Cập nhật tọa độ mới
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;


		//Tìm object thực sự va chạm với HolyWater từ list coEvents
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//Kiểm tra xem Holywater va chạm với object nào 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			float left, top, right, bottom;

			if (dynamic_cast<Candle*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);
				Candle* candle = dynamic_cast<Candle*>(e->obj);
				candle->SetState(STATIC_OBJECT_DESTROYED);			//CHuyển trạng thái của Candle
				candle->setIsDisappearByWeapn(true);				//Xác định Object biến mất do Weapon gây ra

				candle->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));

			}
			else if (dynamic_cast<Torch*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);
				Torch* torch = dynamic_cast<Torch*>(e->obj);
				torch->SetState(STATIC_OBJECT_DESTROYED);			//CHuyển trạng thái của Torch
				torch->setIsDisappearByWeapn(true);					//Xác định Object biến mất do Weapon gây ra

				torch->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));
			}
			else if (dynamic_cast<Enemy*>(e->obj))
			{
				DebugOut(L"Va cham enemy \n"); 

				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);

				enemy->Attacked(this->damage);			//Trừ HP của Solider
				enemy->setIsDisappearByWeapn(true);		//Xác định Object biến mất do Weapon gây ra
				enemy->SetImmortal();						//Set trạng thái bất tử

				enemy->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));

			}
			else if (dynamic_cast<Brick*>(e->obj))
			{
				Brick* brick = dynamic_cast<Brick*>(e->obj);

				//Nếu va chạm với broken brick
				if (brick->GetIDItem() == BRICK_BROKEN) {
					SoundManager::GetInstance()->Play(BROKEN_BRICK);

					//Chuyển trạng thái render animation broken block viên gạch
					brick->SetState(STATIC_OBJECT_DESTROYED);

					//Nếu là loại hidden item không chồi lên từ mặt đất thì chỉ cần va chạm giữa Holywater và gạch 
					//Thì ta sẽ cho phép active hidden item đó
					if (!HiddenObjectManager::GetInstance()->GetIsMoving()) {
						HiddenObjectManager::GetInstance()->isTouchedBrick = true;
					}
				}
				else {
					//Nếu đang ở trạng thái "lọ" mà va chạm với brick theo hướng từ trên xuống (lọ rơi từ trên xuống nên sẽ va chạm theo hướng bottom)
					if (state == W_HOLYWATER_BOTTLE_STATE && e->ny == COLLISION_DIRECTION_BOTTOM) {
						isBurning = true;								//Bật cờ xác định trạng thái đang cháy
						startBurningTime = GetTickCount();				//Bắt đầu lưu thời gian render lửa cháy
						SetState(W_HOLYWATER_FIRE_STATE);				//Chuyển trạng thái cho chính weapon này
					}

					//Tọa độ sẽ render lửa cháy
					x += dx;

					//Lấy Y của gạch bị va chạm và - chiều cao của Holywater cháy
					y = brick->GetY() - W_HOLYWATER_FIRE_HEIGHT;
				}
				
			}




		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];


}




void HolyWaterSubWeapon::Render()
{
	animation_set->at(state)->Render(INSIDE_CAMERA, false, orientation, x, y);

}


void HolyWaterSubWeapon::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects) {
	for (auto obj : listObjects)
	{
		if (dynamic_cast<Candle*>(obj) || dynamic_cast<Brick*>(obj) || dynamic_cast<Torch*>(obj))
			coObjects.push_back(obj);
		else if ((dynamic_cast<Enemy*>(obj)) 
			&& obj->GetState() == ENEMY_WORKING && state == W_HOLYWATER_FIRE_STATE && !obj->getIsImmortal())
			coObjects.push_back(obj);
	}
}


void HolyWaterSubWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (state == W_HOLYWATER_BOTTLE_STATE) {
		right = left + W_HOLYWATER_BOTTLE_WIDTH;
		bottom = top + W_HOLYWATER_BOTTLE_HEIGHT;
	}
	else if (state == W_HOLYWATER_FIRE_STATE) {
		right = left + W_HOLYWATER_FIRE_WIDTH;
		bottom = top + W_HOLYWATER_FIRE_HEIGHT;
	}



}