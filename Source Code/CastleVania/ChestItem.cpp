#include "ChestItem.h"
#include "SoundManager.h"
#include "PlayerManager.h"


ChestItem::ChestItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_CHEST);

}


void ChestItem::Render() {
	Item::Render();
}

void ChestItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);

	PlayerManager* manager = PlayerManager::GetInstance();


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Item biến mất
		setActive(false);

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_ITEM);

		//Set điểm 
		int temp = manager->GetScore();
		temp += ITEM_CHEST_SCORE;
		PlayerManager::GetInstance()->SetScore(temp);

		//Hiển thị số điểm lên màn hình game
		manager->idScoreShow = ITEM_CHEST_SCORE;

		//Set thời gian để biến mất số điểm hiển thị
		manager->timeStartRenderScore = GetTickCount();

		//Set vị trí điểm hiển thị lên màn hình game
		D3DXVECTOR2 positionScore = D3DXVECTOR2(x + ITEM_CHEST_WIDTH, y);
		manager->positionScoreShow = positionScore;

	}
}


void ChestItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_CHEST_WIDTH;
	bottom = top + ITEM_CHEST_HEIGHT;
}