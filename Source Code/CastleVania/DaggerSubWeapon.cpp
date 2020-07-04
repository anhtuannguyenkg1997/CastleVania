#include "DaggerSubWeapon.h"
#include "CObjectCell.h"
#include "HiddenObjectManager.h"
#include "SoundManager.h"
#include "HunchBack.h"

DaggerSubWeapon::DaggerSubWeapon(Simon* simon) : SubWeapon(simon)
{

	//Nếu đang hướng về chiều dương
	if (orientation == OR_PLUS)
		//Vận tốc dương
		vx = WEAPONS_DAGGER_SPEED;
	else
		//Vận tốc âm
		vx = -WEAPONS_DAGGER_SPEED;


	//Dagger Chỉ đi theo chiều ngang (trục X) nên lúc này vận tốc trên trục y bằng 0
	vy = 0;


	//Damage mà dagger gây ra
	damage = W_NORMAL_DAMAGE;


}



//Xét va chạm giữa subweapon và các Object khác
void DaggerSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Lấy instance của SubWeaponManager để tính số subweapon còn lại
	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();


	//Gọi hàm update của CGameObject
	SubWeapon::Update(dt, listObjects, simon);

	//object có khả năng va chạm với Dagger
	vector<LPGAMEOBJECT> coObject;


	//Lấy object có khả năng va chạm với AXE
	SubWeapon::GetCollisionObject(listObjects, coObject, simon);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	//Lấy ra objects tiềm năng có thể va chạm đưa vào coEvents
	CalcPotentialCollisions(&coObject, coEvents);


	//Không có va chạm nào
	if (coEvents.size() == 0)
	{
		//Cập nhật tọa độ mới của Dagger
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;


		//Tìm object thực sự va chạm với Dagger từ list coEvents
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);



		//Kiểm tra xem Dagger va chạm với object nào 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			float left, top, right, bottom;

			if (dynamic_cast<Candle*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);
				Candle* candle = dynamic_cast<Candle*>(e->obj);
				candle->SetState(STATIC_OBJECT_DESTROYED);		//CHuyển trạng thái của Candle
				candle->setIsDisappearByWeapn(true);			//Xác định Object biến mất do Weapon gây ra
				setActive(false);								//Làm Dagger biến mất

				candle->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));
			}
			else if (dynamic_cast<Torch*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);
				Torch* torch = dynamic_cast<Torch*>(e->obj);
				torch->SetState(STATIC_OBJECT_DESTROYED);		//CHuyển trạng thái của Torch
				torch->setIsDisappearByWeapn(true);				//Xác định Object biến mất do Weapon gây ra
				setActive(false);								//Làm Dagger biến mất

				torch->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));
			}

			else if (dynamic_cast<Enemy*>(e->obj))
			{
				SoundManager::GetInstance()->Play(ATTACK_TO_OBJECT);
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);

				//Kiểm tra BlackKnight có bất tử không

				enemy->Attacked(this->damage);		//Trừ HP của Solider
				enemy->setIsDisappearByWeapn(true);	//Xác định Object biến mất do Weapon gây ra
				enemy->SetImmortal();					//Set trạng thái bất tử
				setActive(false);							//Làm Dagger biến mất

				enemy->GetBoundingBox(left, top, right, bottom);

				//Render hiệu ứng đánh trúng Candle
				effect = new AttackingEffect(aEManager->GetPositionEffectSweptAABB(e->nx, e->ny, left, top, right, bottom));

			}
			//Va chạm Dagger và Brick
			else if (dynamic_cast<Brick*>(e->obj))
			{
				SoundManager::GetInstance()->Play(BROKEN_BRICK);

				Brick* brick = dynamic_cast<Brick*> (e->obj);

				//Chuyển trạng thái render animation broken block viên gạch
				brick->SetState(STATIC_OBJECT_DESTROYED);

				//Nếu là loại hidden item không chồi lên từ mặt đất thì chỉ cần va chạm giữa Dagger và gạch 
				//Thì ta sẽ cho phép active hidden item đó
				if (!HiddenObjectManager::GetInstance()->GetIsMoving()) {
					HiddenObjectManager::GetInstance()->isTouchedBrick = true;
				}
			}

		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];


	//Nếu Dagger không còn hoạt động nữa
	if (!isActive())
		//Giảm số lượng suweapon ở subWeaponManager
		subweaponManager->numSubWeaponCreated--;


}

void DaggerSubWeapon::Render()
{
	SubWeapon::Render();

}


void DaggerSubWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;


	right = left + W_DAGGER_WIDTH;
	bottom = top + W_DAGGER_HEIGHT;

}