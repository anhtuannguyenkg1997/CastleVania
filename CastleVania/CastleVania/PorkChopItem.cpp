#include "PorkChopItem.h"
#include "PlayerManager.h"
#include "SoundManager.h"


PorkChopItem::PorkChopItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_PORKCHOP);

}


void PorkChopItem::Render() {
	Item::Render();
}



void PorkChopItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_ITEM);
		

		//Item biến mất
		setActive(false);


		PlayerManager* playerManager = PlayerManager::GetInstance();


		//Tăng HP cho simon thêm 6 đơn vị
		int HP = playerManager->GetSimonHP();
		HP += ITEM_PORKCHOP_VALUE;

		if (HP > HP_SIZE)
			HP = HP_SIZE;

		playerManager->SetSimonHP(HP);
	}

}


void PorkChopItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_PORKCHOP_WIDTH;
	bottom = top + ITEM_PORKCHOP_HEIGHT;
}