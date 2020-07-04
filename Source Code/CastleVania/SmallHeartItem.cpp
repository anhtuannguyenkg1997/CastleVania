#include "SmallHeartItem.h"
#include "PlayerManager.h"
#include "SoundManager.h"


SmallHeartItem::SmallHeartItem(float x, float y) : Item(x, y) {

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	animation_set = animation_sets->Get(ITEM_SMALLHEART);


	deltaVX = ITEM_SMALLHEART_DELTAVX;

	vy = ITEM_SMALLHEART_FALLING_SPEED;

}


void SmallHeartItem::Render() {
	Item::Render();
}

void SmallHeartItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Lúc này quả tim đang rơi xuống
	if (vy != 0)
	{
		//Tăng vận tốc theo chiều x lên
		vx += deltaVX;

		//Nếu đạt đến vận tốc lớn nhất (theo chiều dương và âm)
		//Tức là lúc này quả tim di chuyển được con đường lớn nhất theo trục X
		if (vx >= ITEM_SMALLHEART_MAXVX || vx <= -ITEM_SMALLHEART_MAXVX)
			deltaVX = -deltaVX; // đổi chiều để cho quả tim đi ngược về (tạo hiệu ứng rơi vòng vòng)
	}

	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);

	PlayerManager* manager = PlayerManager::GetInstance();

	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_ITEM);

		//Item biến mất
		setActive(false);

		//Set số smallheart trên HUD 
		int temp = manager->GetSubweaponCounter();
		temp += ITEM_SMALLHEART_VALUE;
		manager->SetSubweaponCounter(temp);
	}

}


void SmallHeartItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_SMALLHEART_WIDTH;
	bottom = top + ITEM_SMALLHEART_HEIGHT;
}