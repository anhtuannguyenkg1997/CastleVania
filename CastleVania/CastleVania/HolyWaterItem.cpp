#include "HolyWaterItem.h"
#include "PlayerManager.h"
#include "SoundManager.h"


HolyWaterItem::HolyWaterItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_HOLYWATER);

}


void HolyWaterItem::Render() {
	Item::Render();
}



void HolyWaterItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);

	PlayerManager* playerManager = PlayerManager::GetInstance();

	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_SUBWEAPON_ITEM);

		//Item biến mất
		setActive(false);

		//Simon được phép dùng HOLYWATER
		playerManager->SetSubweaponID(HOLY_WATER_HUD);
	}

}


void HolyWaterItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_HOLYWATER_WIDTH;
	bottom = top + ITEM_HOLYWATER_HEIGHT;
}