#include "MorningStarItem.h"
#include "PlayerManager.h"
#include "SoundManager.h"



MorningStarItem::MorningStarItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_MORNINGSTAR);

}


void MorningStarItem::Render() {
	Item::Render();
}



void MorningStarItem::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects, simon);

	PlayerManager* manager = PlayerManager::GetInstance();


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_SUBWEAPON_ITEM);

		//Item biến mất
		setActive(false);

		// Hiệu ứng đổi màu khi upgrade MorningStar
		simon->SetState(UPGRADE_MS); 

		//Không cho Simon di chuyển
		simon->SetSpeed(0, 0);

		//Bật cờ xác nhận cho upgrade MS. Để class MS xử lí phần upgrade này
		simon->isUpgradeMorningStar = true;

		//Lấy Level hiện tại của MS
		int morningStarLevel = manager->GetMSLevel();


		if (morningStarLevel < MS_LEVEL3) {
			
			//Tăng level MS lên 1
			morningStarLevel++;

			manager->SetMSLevel(morningStarLevel);

		}


	}

}


void MorningStarItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_MORNINGSTAR_WIDTH;
	bottom = top + ITEM_MORNINGSTAR_HEIGHT;
}