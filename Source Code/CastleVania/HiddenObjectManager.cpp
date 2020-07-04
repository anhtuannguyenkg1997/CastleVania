#include "HiddenObjectManager.h"
#include "MoneyBagItem.h"
#include "DoubleShotItem.h"
#include "TripleShotItem.h"
#include "PorkChopItem.h"
#include "ChestItem.h"
#include "DiademItem.h"
#include "CObjectCell.h"
#include "SoundManager.h"


HiddenObjectManager* HiddenObjectManager::_instance = NULL;


//Lấy thể hiện duy nhất của camera
HiddenObjectManager* HiddenObjectManager::GetInstance()
{
	if (_instance == NULL) _instance = new HiddenObjectManager();
	return _instance;
}


void HiddenObjectManager::CreateHiddenObject(float x, float y, int idItem, int isMoving) {


	//Set loại hidden object trong màn này
	isMoving == HDO_MOVING ? this->isMoving = true : this->isMoving = false;


	//Tạo item tương ứng và lưu nó vào trong con trỏ item
	switch (idItem) {
	case ITEM_DOUBLESHOT:
		item = new DoubleShotItem(x, y);
		break;
	case ITEM_TRIPLESHOT:
		item = new TripleShotItem(x, y);
		break;
	case ITEM_PORKCHOP:
		item = new PorkChopItem(x, y);
		break;
	case ITEM_MONEYBAG:
		item = new MoneyBagItem(x, y);
		break;
	case ITEM_CHEST:
		item = new ChestItem(x, y);
		break;
	case ITEM_DIADEM:
		item = new DiademItem(x, y);
		break;
	default:
		break;
	}


	//Loại hidden object. Để phân biệt với item khác mà ta sẽ xử lí bên hàm update của class Item
	item->SetType(HIDDEN_OBJECT);

	//Làm cho item không bị rơi khi chưa kích hoạt. Vì ban đầu item sẽ nằm ẩn ở bên dưới viên gạch 
	item->SetSpeed(0.0f, 0.0f);


	//Thêm hidden object này vào grid
	CObjectCell* objectCell = new CObjectCell(item, x, y);



}




void HiddenObjectManager::Update() {
	
	//if (simon == NULL) return;

	//Loại Hidden Object di chuyển từ dưới Brick lên trên
	if (isMoving) {

		//Nếu Hidden Object của màn này chưa được Active 1 lần nào => cần kiểm tra 
		if (!isActiveHiddenObject) {

			//Nếu Simon đã chạm vào viên gạch giữ chức năng Active Hidden Object
			if (isTouchedBrick) {

				//Đếm thời gian kể từ lúc Hidden Object bắt đầu chồi lên từ Brick
				timeStartActive = GetTickCount();		

				//Lấy vùng bao của Item Hidden
				float leftI, topI, rightI, bottomI;
				item->GetBoundingBox(leftI, topI, rightI, bottomI);

				//Vì top của item đang nằm tại top của Brick
				//=> quảng đường item trồi lên chính bằng với hiệu của bounding bottom-top của Item
				float dy = bottomI - topI;

				//Thời gian chồi từ dưới Brick lên trên là 2000s
				//=> Tìm được vận tốc
				item->SetVY(-(dy / HDO_TIME_MOVING));

				//Phát âm thanh xuất hiện Hidden Object
				SoundManager::GetInstance()->Play(SOUND_SHOW_HIDDEN_ITEM);

				//Không cho Hidden Item xuất hiện lần nào nữa
				isActiveHiddenObject = true;
			}
		}

		//Hết thời gian để Item chồi từ dưới đất lên
		if (isActiveHiddenObject && GetTickCount() - timeStartActive > HDO_TIME_MOVING) {
			
			//Set Vy = 0 để nó không đi lên nữa
			item->SetVY(0.0f);

			//Bắt đầu đếm thời gian để Item biến mất
			isStartCountingTimeDisappear = true;
		}
	}


	//Loại Hidden Object nằm trong lòng Brick
	else {

		//Cho phép Hidden Item xuất hiện
		if (!isActiveHiddenObject) {
			isActiveHiddenObject = true;
		}

		//Khi phá hủy viên gạch chứa Hidden Item
		if (isActiveHiddenObject && isTouchedBrick) {
			
			//Bắt đầu đếm thời gian để Item biến mất
			isStartCountingTimeDisappear = true;

			//Set vận tốc để item rơi xuống đất
			item->SetVY(ITEM_FALLING_SPEED);

			//tắt cờ isTouchedBrick
			isTouchedBrick = false;
		}
		
	}

}



//Reset lại tất cả các biến quản lý hidden Object
//Xem như hidden object chưa xuất hiện
void HiddenObjectManager::Reset() {
	item = NULL;
	isActiveHiddenObject = false;
	isStartCountingTimeDisappear = false;
	isTouchedBrick = false;
	isMoving = false;

	timeStartActive = 0;
}

