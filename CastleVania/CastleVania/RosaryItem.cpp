#include "RosaryItem.h"
#include "RosaryItemEffect.h"
#include "SoundManager.h"

RosaryItem::RosaryItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_ROSARY);

}


void RosaryItem::Render() {
	Item::Render();
}



void RosaryItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);

	RosaryItemEffect* rosaryEffect = RosaryItemEffect::GetInstance();


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_ROSARY_EFFECT);

		//Item biến mất
		setActive(false);

		//Phá hủy tất cả Enemy
		rosaryEffect->StartRosary(GetTickCount());
	}

}


void RosaryItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_ROSARY_WIDTH;
	bottom = top + ITEM_ROSARY_HEIGHT;
}