#include "InvisibilityPotionItem.h"
#include "InvisibilityItemEffect.h"
#include "SoundManager.h"



InvisibilityPotionItem::InvisibilityPotionItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_INVISIBILITYPOTION);

}


void InvisibilityPotionItem::Render() {
	Item::Render();
}



void InvisibilityPotionItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);

	InvisibilityItemEffect* effect = InvisibilityItemEffect::GetInstance();

	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_INVISIBLE_START);

		//Item biến mất
		setActive(false);

		//Kích hoạt tàng hình Simon
		effect->StartInvisibility(GetTickCount());
	}
}


void InvisibilityPotionItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_INVISIBILITYPOTION_WIDTH;
	bottom = top + ITEM_INVISIBILITYPOTION_HEIGHT;
}