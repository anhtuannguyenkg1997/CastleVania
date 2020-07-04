#include "MagicCrystalItem.h"
#include "PlayerManager.h"

MagicCrystalItem::MagicCrystalItem(float x, float y) : Item(x, y) {

	//Lấy Animation Của Item
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(ITEM_MAGIC_CRYSTAL);

}


void MagicCrystalItem::Render() {
	Item::Render();
}

void MagicCrystalItem::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon) {
	
	//Vận tốc rơi của MagicCrystal sẽ nhỏ hơn các item khác, làm cho nó rơi từ từ từ trên xuống
	//vy = ITEM_MAGICCRYSTAL_FALLING_SPEED;

	//Luôn ghi lại thời gian xuất hiện để nó k bao giờ biến mất trừ khi simon ăn nó (khác với item khác)
	timeAppear = GetTickCount();

	//Gọi hàm update của item (xử lí như các item khác)
	Item::Update(dt, listObjects, simon);


	PlayerManager* playerManager = PlayerManager::GetInstance();


	//Kiểm tra xem Simon đã va chạm với Item chưa
	if (isEffectToSimon) {

		//Item biến mất
		setActive(false);

		//Kết thúc Game
		PlayerManager::GetInstance()->isFinishGame = true;
		PlayerManager::GetInstance()->SetIsFightingBoss(false);
		PlayerManager::GetInstance()->timeFinishGame = GetTickCount();

		//Làm đầy HP cho Simon
		playerManager->SetSimonHP(HP_SIZE);
	}

}




void MagicCrystalItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	//không đổi giá trị của left , top
	left = x;
	top = y;

	//bottom = top + chiều cao item
	//right = left + chiều rộng item
	right = left + ITEM_MAGICCRYSTAL_WIDTH;
	bottom = top + ITEM_MAGICCRYSTAL_HEIGHT;
}