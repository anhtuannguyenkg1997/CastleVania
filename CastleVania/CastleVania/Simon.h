#pragma once

#include <vector>
#include "CGameObject.h"
#include "Item.h"

using namespace std;

class Simon : public CGameObject
{
	//Để hiển thị trên HUD
	int score;									//Điểm số
	int doubleShotOrTrippleShot;				//ID Double shot or tripple shot
	int subweaponCounter;						//Lượng vũ khí phụ được sử dụng
	int live;									//Mạng
	int subWeapon;								//ID Vũ khí phụ của Simon
	int HP;										//Máu

public:


	bool standing = false;						//Simon đang chạm đất?????
	bool falling = false;							//Simon đang rơi?????


	bool isUpgradeMorningStar = false;				//Simon có được nâng cấp Morning Star hay không???

	bool isGotDoubleShotItem = false;				//Đang kích hoạt Double shot?????
	bool isGotTripleShotItem = false;				//Đang kích hoạt Triple shot?????

	Simon();

	//Update simon, các object , simon có dừng di chuyển hay không(trạng thái auto walk)
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> listObjects);



	//Vẽ Simon lên màn hình
	void Render();


	//Set trạng thái của Simon (Stand,Jump,Sit,....)
	void SetState(int state);


	//Xử lí khi Simon chạm vào Item
	void touchItem(Item* item);


	//Lấy những object có khả năng va chạm với simon từ listObjects và đưa vào danh sách coObjects
	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);
	

	//Lấy vùng bao của Simon
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	//Kiểm tra đã render xong những animation đặc biệt chưa????
	bool isFinishRenderAnimation();


	// Get function (HUD)
	int GetsubweaponCounter() { return this->subweaponCounter; }
	int GetLive() { return this->live; }
	int GetScore() { return this->score; }
	int GetDoubleShotOrTrippleShot() { return this->doubleShotOrTrippleShot; }
	int GetSubWeapon() { return this->subWeapon; }
	int GetHP() { return this->HP; }

	// Properties change (HUD)
	void AddScore(int x) { score += x; }
	void LoseSubweaponCounter(int x) { subweaponCounter -= x; }
	void LoseHP(int x);
	void SetLive(int x) { live = x; }
	void AddHP(int x);
	void SetHP(int x) { HP = x; }
	void SetSubweaponCounter(int x) { subweaponCounter = x; }
	void SetSubWeapon(int x) { subWeapon = x; }



};

