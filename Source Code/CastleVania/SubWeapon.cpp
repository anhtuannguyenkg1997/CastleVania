#include "SubWeapon.h"


SubWeapon::SubWeapon(Simon* simon) {
	float sx, sy;
	simon->GetPosition(sx, sy);

	if (simon->GetState() == WHOPPING_SITTING) sy += 25.0f; // khớp vị trí tay
	else sy += 10.0f;

	if (simon->GetOrientation() < 0) sx += 30.0f;


	//Vị trí xuất hiện của Subweapon
	SetPosition(sx, sy);

	// Subweapon đánh ra theo hướng của Simon
	SetOrientation(simon->GetOrientation());

	// Hiển thị nó lên
	setActive(true);

	//Dynamic object
	SetType(DYNAMIC_OBJECT);

}





void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects, Simon* simon)
{
	CGameObject::Update(dt, listObjects);
}




void SubWeapon::Render()
{
	if (isActive() && state != WEAPONS_STOP_WATCH)
		animation_set->at(SUBWEAPON_ACTIVE)->Render(INSIDE_CAMERA, false , orientation, x, y);
}


//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với simon gán vào coObjects
void SubWeapon::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {
	coObjects.push_back(simon); // dùng để xét va chạm của Simon với boomerang


	for (auto obj : listObjects)
	{
		if (dynamic_cast<Candle*>(obj) || dynamic_cast<Torch*>(obj) ||(dynamic_cast<Brick*>(obj) && obj->GetIDItem() == BRICK_BROKEN))
			coObjects.push_back(obj);
		else if ((dynamic_cast<Enemy*>(obj))
			&& obj->GetState() == ENEMY_WORKING && !obj->getIsImmortal())
			coObjects.push_back(obj);
	}
}
