#include "BigHeartItem.h"
#include "PlayerManager.h"
#include "SoundManager.h"


BigHeartItem::BigHeartItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_BIGHEART);

}


void BigHeartItem::Render() {
	Item::Render();
}




void BigHeartItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);

	PlayerManager* playerManager = PlayerManager::GetInstance();


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Item biến mất
		setActive(false);

		//Tăng số SmallHeart
		int temp = playerManager->GetSubweaponCounter();
		temp += 5;
		playerManager->SetSubweaponCounter(temp);


		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_ITEM);
	}

}


void BigHeartItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_BIGHEART_WIDTH;
	bottom = top + ITEM_BIGHEART_HEIGHT;
}