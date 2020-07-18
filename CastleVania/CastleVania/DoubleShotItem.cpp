#include "DoubleShotItem.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "SoundManager.h"

DoubleShotItem::DoubleShotItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_DOUBLESHOT);

}


void DoubleShotItem::Render() {
	Item::Render();
}



void DoubleShotItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_ITEM);


		//Item biến mất
		setActive(false);


		//Simon được phép dùng DoubleShot
		PlayerManager::GetInstance()->SetDoubleShotOrTrippleShot(DOUBLE_SHOT_HUD); 

	}

}


void DoubleShotItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_DOUBLE_TRIPLE_SHOT_WIDTH;
	bottom = top + ITEM_DOUBLE_TRIPLE_SHOT_HEIGHT;
}