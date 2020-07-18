#include "SubWeapon.h"


SubWeapon::SubWeapon(Simon* simon) {
	
	//Tìm vị trí xuất hiện của Subweapon
	float simonX, simonY;
	simon->GetPosition(simonX, simonY);

	//Simon ngồi thì cần dời y xuống sâu hơn
	if (simon->GetState() == WHOPPING_SITTING) 
		simonY += 25.0f;
	else 
		simonY += 10.0f;

	if (simon->GetOrientation() == OR_PLUS) simonX += 30.0f;


	//Vị trí xuất hiện của Subweapon
	SetPosition(simonX, simonY);

	// Subweapon đánh ra theo hướng của Simon
	SetOrientation(simon->GetOrientation());

	// Hiển thị nó lên
	setActive(true);

	//Dynamic object
	SetType(DYNAMIC_OBJECT);

}




//Update chung cho SW
void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon)
{
	CGameObject::Update(dt, listObjects);
}




void SubWeapon::Render()
{
	//Không render khi dùng stopwatch
	if (isActive() && state != WEAPONS_STOP_WATCH)
		animation_set->at(SUBWEAPON_ACTIVE)->Render(INSIDE_CAMERA, false , orientation, x, y);
}


//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với simon gán vào coObjects
void SubWeapon::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {
	
	coObjects.push_back(simon); // dùng để xét va chạm của Simon với boomerang


	//Object có thể va chạm với SW
	for (auto obj : listObjects)
	{
		if (dynamic_cast<Candle*>(obj) || dynamic_cast<Torch*>(obj) ||(dynamic_cast<Brick*>(obj) && obj->GetIDItem() == BRICK_BROKEN))
			coObjects.push_back(obj);
		else if ((dynamic_cast<Enemy*>(obj))
			&& obj->GetState() == ENEMY_WORKING && !obj->getIsImmortal())
			coObjects.push_back(obj);
	}
}
