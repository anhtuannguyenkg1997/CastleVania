#include "ItemManager.h"
#include "CRandomManager.h"

ItemManager* ItemManager::_instance = NULL;


ItemManager* ItemManager::GetInstance()
{
	if (_instance == NULL) _instance = new ItemManager();
	return _instance;
}



Item* ItemManager::CreateItem(LPGAMEOBJECT object) {
	
	int idItem = -1;

	float x, y;

	//Nếu là Object tĩnh và đã bị phá hủy
	//Nếu là Enemy và đã bị phá hủy
	if (((dynamic_cast<Torch*>(object) || dynamic_cast<Candle*>(object)) && object->GetState() == STATIC_OBJECT_DESTROYED)
		|| (dynamic_cast<Enemy*>(object) && object->GetState() == ENEMY_DESTROYED))
	{
		//Nếu Item là loại Random
		if (object->GetIDItem() == ITEM_RANDOM) {
			//Random ra 1 ID item bất kì
			idItem = RandomItem();
		}
		//Nếu không là loại Random
		else {
			//Lấy ID Item chứa trong ObjectGame
			idItem = object->GetIDItem();
		}

		//Set vị trí item theo vị trí ObjectGame
		object->GetPosition(x, y);
	}


	//Tạo ra một Item
	switch (idItem) {
	case ITEM_MORNINGSTAR:
		return new MorningStarItem(x, y);
	case ITEM_SMALLHEART:
		return new SmallHeartItem(x, y);
	case ITEM_BIGHEART:
		return new BigHeartItem(x, y);
	case ITEM_MONEYBAG_RED:
		return new MoneyBagRedItem(x, y);
	case ITEM_MONEYBAG_PURPLE:
		return new MoneyBagPurpleItem(x, y);
	case ITEM_MONEYBAG_WHITE:		
		return new MoneyBagWhiteItem(x, y);
	case ITEM_ROSARY:
		return new RosaryItem(x, y);
	case ITEM_INVISIBILITYPOTION:
		return new InvisibilityPotionItem(x, y);
	case ITEM_DOUBLESHOT:
		return new DoubleShotItem(x, y);
	case ITEM_TRIPLESHOT:
		return new TripleShotItem(x, y);
	case ITEM_PORKCHOP:
		return new PorkChopItem(x, y);
	case ITEM_MONEYBAG:
		return new MoneyBagItem(x, y);
	case ITEM_CHEST:
		return new ChestItem(x, y);
	case ITEM_DIADEM:
		return new DiademItem(x, y);
	case ITEM_DAGGER:
		return new DaggerItem(x, y);
	case ITEM_AXE:
		return new AxeItem(x, y);
	case ITEM_HOLYWATER:
		return new HolyWaterItem(x, y);
	case ITEM_BOOMERANG:
		return new BoomerangItem(x, y);
	case ITEM_STOPWATCH:
		return new StopWatchItem(x, y);
	case ITEM_MAGIC_CRYSTAL:
		return new MagicCrystalItem(MAGICCRYSTAL_X_APPEAR, MAGICCRYSTAL_Y_APPEAR);
	default:
		return NULL;
	}

}



int ItemManager::RandomItem() {

	CRandomManager* rdManager = CRandomManager::GetInstance();

	int idItem;

	//Random ra loại Item (special hay normal)
	int typeItem = rdManager->Random(LIST_RANDOM_ITEM_TYPE);

	//Item Random thuộc loại Normal
	if (typeItem == NORMAL_ITEMS) {
		idItem = rdManager->Random(LIST_RANDOM_NORMAL_ITEM);
	}
	//Item Random thuộc loại Special
	else {
		idItem = rdManager->Random(LIST_RANDOM_SPECIAL_ITEM);
	}

	return idItem;
}



void ItemManager::Update(vector<LPGAMEOBJECT> listObjects) {

	SubWeaponManager* manager = SubWeaponManager::GetInstance();

	//Duyệt qua các object đang xuất hiện để tìm object nào bị tiêu diệt để tạo item
	for (UINT i = 0; i < listObjects.size(); i++)
	{
		//Object bị tiêu diệt khi thõa đồng thời 3 điều kiện
		//1. Object phải bị tiêu diệt (active= false)
		//2. Object bị tiêu diệt có khả năng tạo ra item (VD: torch, candle,..) (idItem khác -1)
		//3. Object bị biến mất là do weapon gây ra
		if (!listObjects[i]->isActive()
			&& listObjects[i]->getIsDisappearByWeapon()
			&& listObjects[i]->GetIDItem() != -1)
		{
			//Tạo mới 1 item dựa vào idItem mà object đó đang chứa
			Item* item = CreateItem(listObjects[i]);


			//Đưa item này vào listItem 
			listObjects.push_back(item);

			//Biến tạm lưu vị trí của item 
			float x, y;

			//Lấy vị trí của item 
			item->GetPosition(x, y);

			//Đưa item này vào Grid
			CObjectCell* objectCell = new CObjectCell(item, x, y);

		}

	}


}