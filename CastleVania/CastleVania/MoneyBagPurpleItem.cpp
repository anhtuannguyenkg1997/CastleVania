#include "MoneyBagPurpleItem.h"
#include "PlayerManager.h"
#include "SoundManager.h"


MoneyBagPurpleItem::MoneyBagPurpleItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_MONEYBAG_PURPLE);

}


void MoneyBagPurpleItem::Render() {
	Item::Render();
}



void MoneyBagPurpleItem::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon) {
	
	//Gọi hàm Update Item
	Item::Update(dt, listObjects,simon);


	PlayerManager* playerManager = PlayerManager::GetInstance();


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Phát âm thanh Chạm Item
		SoundManager::GetInstance()->Play(SOUND_TOUCH_ITEM);
		
		//Item biến mất
		setActive(false);

		//Set điểm 
		int temp = playerManager->GetScore();
		temp += ITEM_MONEYBAG_PURPLE_SCORE;
		playerManager->SetScore(temp);

		//Hiển thị số điểm lên màn hình game
		playerManager->idScoreShow = ITEM_MONEYBAG_PURPLE_SCORE;

		//Set thời gian để biến mất số điểm hiển thị
		playerManager->timeStartRenderScore = GetTickCount();

		//Set vị trí điểm hiển thị lên màn hình game
		D3DXVECTOR2 positionScore = D3DXVECTOR2(x + ITEM_MONEYBAG_WIDTH, y);
		playerManager->positionScoreShow = positionScore;

	}
}


void MoneyBagPurpleItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_MONEYBAG_WIDTH;
	bottom = top + ITEM_MONEYBAG_HEIGHT;
}