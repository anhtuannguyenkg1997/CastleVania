#include "Item.h"
#include "Brick.h"
#include "HiddenObjectManager.h"
#include "PlayerManager.h"


Item::Item(float x, float y) {
	timeAppear = -1;
	this->x = x;
	this->y = y;

	active = true;

	state = STATIC_OBJECT_ACTIVE;

	vx = 0;

	vy = ITEM_FALLING_SPEED;
	
	type = STATIC_OBJECT;
}


void Item::Render()
{
	int alpha = 255;

	//Xử lí Item nhấp nháy khi hết 1/2 thời gian xuất hiện
	//Nếu không phải là Hidden Item
	//Không cần phải chờ time Item chồi lên từ Brick
	if (type != HIDDEN_OBJECT) {
		if (GetTickCount() - timeAppear > ITEM_TIME_DISAPEAR / 2)
			alpha = rand() % 255 + 0;
	}
	//Nếu là Hidden Item
	//Phải chờ time Item chồi lên từ Brick
	else {
		if (HiddenObjectManager::GetInstance()->isStartCountingTimeDisappear && GetTickCount() - timeAppear > ITEM_TIME_DISAPEAR / 2) {
			alpha = rand() % 255 + 0;
		}
	}



	animation_set->at(state)->Render(INSIDE_CAMERA, false, NOT_FLIPPING, x, y, alpha);


}


//Xét va chạm của Item
void Item::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Kiểm tra Item còn thời gian xuất hiện hay không???
	//TH1: Nếu là item bình thường 
	//TH2: Nếu là Hidden item nhưng bắt đầu được đếm thời gian để biến mất(như item bình thường)
	//Xử lí chung 1 logic
	if (type != HIDDEN_OBJECT || HiddenObjectManager::GetInstance()->isStartCountingTimeDisappear) {

		//Bắt đầu xuất hiện
		if (timeAppear == -1)
			//Bắt đầu tính thời gian hiển thị item
			timeAppear = GetTickCount();
		else
		{
			//Vượt quá thời gian hiển thị item
			if (GetTickCount() - timeAppear > ITEM_TIME_DISAPEAR)
			{
				//Item biến mất
				active = false;	
			}
		}
	}



	CGameObject::Update(dt, listObjects);

	vector<LPGAMEOBJECT> coObjects;

	coObjects.clear();


	//Lấy object có khả năng va chạm với Item
	GetCollisionObject(listObjects, coObjects, simon);



	//Xử lí va chạm
	for (UINT i = 0; i < coObjects.size(); i++)
	{
		LPGAMEOBJECT obj = coObjects.at(i);

		//Xử lí Item không phải là Hidden Item
		//Hoặc nếu là Hidden Item thì phải chổi hẳn lên Brick
		if (type != HIDDEN_OBJECT || 
			HiddenObjectManager::GetInstance()->isStartCountingTimeDisappear)
		{
			//Va chạm với Simon
			if (dynamic_cast<Simon*>(obj)) {

				Simon* simon = dynamic_cast<Simon*>(obj);

				float left, top, right, bottom;

				simon->GetBoundingBox(left, top, right, bottom);

				if (CheckCollision(left, top, right, bottom) == true)
				{
					//Va chạm với simon thì bật cờ để các class con của Item xử lí
					isEffectToSimon = true;
				}
			}

			//Va chạm với Brick
			if (dynamic_cast<Brick*>(obj)) {
				Brick* brick = dynamic_cast<Brick*>(obj);

				float left, top, right, bottom;

				brick->GetBoundingBox(left, top, right, bottom);

				if (CheckCollision(left, top, right, bottom) == true)
				{
					//Làm cho item không bị rơi
					vy = 0.0f;
					vx = 0.0f;
				}
			}

		}
	}

	//Cập nhật tọa độ của Item
	x += dx;
	y += dy;

}



//Lấy những object có khả năng va chạm với Item. Đưa vào list coObjects
void Item::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {

	coObjects.push_back(simon);

	for (auto obj : listObjects)
	{
		if (dynamic_cast<Brick*>(obj))
			coObjects.push_back(obj);
	}
}



//Kiểm tra va chạm giữa MorningStar và các Object khác
bool Item::CheckCollision(float otherLeft, float otherTop, float otherRight, float otherBottom)
{
	//Boundingbox của item
	float iLeft, iTop, iRight, iBottom;

	//Lấy vị trí của MorningStar
	GetBoundingBox(iLeft, iTop, iRight, iBottom);


	//Xét va chạm giữa MS với Object khác. 2 object tĩnh
	return BroadPhaseTestAABB(iLeft, iTop, iRight, iBottom, otherLeft, otherTop, otherRight, otherBottom);
}
