#include "BoomerangSubWeapon.h"
#include "SoundManager.h"
#include "CObjectCell.h"
#include "HiddenObjectManager.h"
#include "SoundManager.h"



BoomerangSubWeapon::BoomerangSubWeapon(Simon* simon) : SubWeapon(simon)
{
	//Nếu đang hướng về chiều dương
	if (orientation == OR_PLUS)
		//Đặt vận tốc AXE dương
		vx = WEAPONS_BOOMERANG_SPEED;
	//Nếu không
	else
		//Đặt vận tốc AXE âm
		vx = -WEAPONS_BOOMERANG_SPEED;

	//Boomerang chỉ di chuyển theo trục X nên vận tốc trên trục Y bằn 0
	vy = 0;


	//Damage Boomerang gây ra cho Enemy
	damage = W_NORMAL_DAMAGE;

}



//Xét va chạm giữa subweapon và các Object khác
void BoomerangSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Lấy instance của SubWeaponManager để tính số subweapon còn lại
	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();

	//Gọi hàm update của CGameObject
	SubWeapon::Update(dt, listObjects, simon);


	//Nếu quăng boomerang về chiều dương
	if (orientation == OR_PLUS)
		//Vx ban đầu sẽ trừ Biến thiên vận tốc (giảm tốc độ boomerang về 0 => boomerang đứng yên)
		//Vx < 0 thì nó sẽ di chuyển ngược về trục âm . Tạo hiệu ứng quay về của boomerang
		vx -= WEAPONS_BOOMERANG_DELTAVX;
	else //Nếu quăng boomerang về chiều âm
		//vx xét ngược lại với logic tương tự
		vx += WEAPONS_BOOMERANG_DELTAVX;


	//object có khả năng va chạm với Boomerang
	vector<LPGAMEOBJECT> coObject;


	//Lấy object có khả năng va chạm với Boomerang
	SubWeapon::GetCollisionObject(listObjects, coObject, simon);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	//Lấy ra objects tiềm năng có thể va chạm đưa vào coEvents
	CalcPotentialCollisions(&coObject, coEvents);


	//Có va chạm
	if (coEvents.size() != 0)
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;


		//Tìm object thực sự va chạm với Boomerang từ list coEvents
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//Kiểm tra xem Boomerang va chạm với object nào 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			float left, top, right, bottom;


			LPCOLLISIONEVENT e = coEventsResult[i];


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
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);

				enemy->Attacked(this->damage);			//Trừ HP của Solider
				enemy->setIsDisappearByWeapn(true);		//Xác định Object biến mất do Weapon gây ra
				enemy->SetImmortal();					//Set trạng thái bất tử

				enemy->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));
			}
			//Va chạm Boomerang và Brick
			else if (dynamic_cast<Brick*>(e->obj))
			{
				SoundManager::GetInstance()->Play(BROKEN_BRICK);

				Brick* brick = dynamic_cast<Brick*> (e->obj);

				//Chuyển trạng thái render animation broken block viên gạch
				brick->SetState(STATIC_OBJECT_DESTROYED);

				//Nếu là loại hidden item không chồi lên từ mặt đất thì chỉ cần va chạm giữa Boomerang và gạch 
				//Thì ta sẽ cho phép active hidden item đó
				if (!HiddenObjectManager::GetInstance()->GetIsMoving()) {
					HiddenObjectManager::GetInstance()->isTouchedBrick = true;
				}
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				setActive(false);
			}

		}
	}

	//Cập nhật tọa độ mới của Boomerang
	x += dx;
	y += dy;

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];


	//Nếu Boomerang không còn hoạt động nữa
	if (!isActive()) {
		//Giảm số lượng suweapon ở subWeaponManager
		subweaponManager->numSubWeaponCreated--;


		//Dừng phát sound của boomerang (do nó luôn lặp lại nên cần tắt đi nếu boomerang đã bị hủy)
		if (SoundManager::GetInstance()->isPlayingSound(SOUND_SW_BOOMERANG))
			SoundManager::GetInstance()->Stop(SOUND_SW_BOOMERANG);
	}


}







void BoomerangSubWeapon::Render()
{
	SubWeapon::Render();
}


void BoomerangSubWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;


	right = left + W_BOOMERANG_WIDTH;
	bottom = top + W_BOOMERANG_HEIGHT;

}