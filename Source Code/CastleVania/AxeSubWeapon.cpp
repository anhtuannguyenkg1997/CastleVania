#include "AxeSubWeapon.h"
#include "CObjectCell.h"
#include "HiddenObjectManager.h"
#include "SoundManager.h"

AxeSubWeapon::AxeSubWeapon(Simon* simon) : SubWeapon(simon)
{
	//Nếu đang hướng về chiều dương
	if (orientation == OR_PLUS)
		//Đặt vận tốc AXE dương
		vx = WEAPONS_AXE_SPEED;
	//Nếu không
	else
		//Đặt vận tốc AXE âm
		vx = -WEAPONS_AXE_SPEED;

	//Vận tốc bay của AXE 
	vy = -WEAPONS_AXE_FLYING_SPEED;


	//Damage AXE gây ra cho Enemy
	damage = W_STRONG_DAMAGE;

}



//Xét va chạm giữa subweapon và các Object khác
void AxeSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Lấy instance của SubWeaponManager để tính số subweapon còn lại
	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();

	PlayerManager* playerManager = PlayerManager::GetInstance();

	//Gọi hàm update của CGameObject
	SubWeapon::Update(dt, listObjects, simon);

	//object có khả năng va chạm với AXE
	vector<LPGAMEOBJECT> coObject;

	//Lấy object có khả năng va chạm với AXE
	SubWeapon::GetCollisionObject(listObjects, coObject, simon);


	//Simple falldown
	//Làm cho AXE rơi
	vy += WEAPONS_AXE_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	//Lấy ra objects tiềm năng có thể va chạm đưa vào coEvents
	CalcPotentialCollisions(&coObject, coEvents);




	//Có va chạm
	if (coEvents.size() != 0) {
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		//Tìm object thực sự va chạm với AXE từ list coEvents
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//Kiểm tra xem AXE va chạm với object nào 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			float left, top, right, bottom;

			if (dynamic_cast<Candle*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);

				Candle* candle = dynamic_cast<Candle*>(e->obj);

				candle->SetState(STATIC_OBJECT_DESTROYED);	//CHuyển trạng thái của Candle
				candle->setIsDisappearByWeapn(true);		//Xác định Object biến mất do Weapon gây ra

				candle->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));
			}
			else if (dynamic_cast<Torch*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);

				Torch* torch = dynamic_cast<Torch*>(e->obj);
				torch->SetState(STATIC_OBJECT_DESTROYED);	//CHuyển trạng thái của Torch
				torch->setIsDisappearByWeapn(true);			//Xác định Object biến mất do Weapon gây ra

				torch->GetBoundingBox(left, top, right, bottom);
				//Render hiệu ứng đánh trúng Torch
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));
			}
			else if (dynamic_cast<Enemy*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);

				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);

				enemy->Attacked(this->damage);	//Trừ HP của Enemy
				enemy->setIsDisappearByWeapn(true);//Xác định Object biến mất do Weapon gây ra
				enemy->SetImmortal();				//Set trạng thái bất tử					

				enemy->GetBoundingBox(left, top, right, bottom);
				//Render hiệu ứng đánh trúng Enemy
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));
			}
			//Va chạm AXe và Brick
			else if (dynamic_cast<Brick*>(e->obj))
			{
				Brick* brick = dynamic_cast<Brick*> (e->obj);
				SoundManager::GetInstance()->Play(BROKEN_BRICK);

				//Chuyển trạng thái render animation broken block viên gạch
				brick->SetState(STATIC_OBJECT_DESTROYED);

				//Nếu là loại hidden item không chồi lên từ mặt đất thì chỉ cần va chạm giữa AXe và gạch 
				//Thì ta sẽ cho phép active hidden item đó
				if (!HiddenObjectManager::GetInstance()->GetIsMoving()) {
					HiddenObjectManager::GetInstance()->isTouchedBrick = true;
				}
			}

		}

	}


	//Cập nhật tọa độ mới của AXE
	x += dx;
	y += dy;


	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];



	//Nếu AXE không còn hoạt động nữa
	if (!isActive()) {

		//Giảm số lượng suweapon ở subWeaponManager
		subweaponManager->numSubWeaponCreated--;

		//Dừng phát sound của AXE (do nó luôn lặp lại nên cần tắt đi nếu AXE đã bị hủy)
		if (SoundManager::GetInstance()->isPlayingSound(SOUND_SW_AXE))
			SoundManager::GetInstance()->Stop(SOUND_SW_AXE);

	}



}




void AxeSubWeapon::Render()
{
	SubWeapon::Render();
}





void AxeSubWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	right = left + W_AXE_WIDTH;
	bottom = top + W_AXE_HEIGHT;

}