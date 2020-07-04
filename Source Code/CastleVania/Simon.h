#pragma once

#include <vector>
#include "CGameObject.h"

using namespace std;

class Simon : public CGameObject
{
public:
	//Thời gian simon bắt đầu chết (để reset lại)
	DWORD timeDied;		

	bool standing = false;							//Simon đang đứng trên mặt đất?????

	bool isUpgradeMorningStar = false;				//Simon có được nâng cấp Morning Star hay không???


	Simon();

	//Update simon, các object , simon có dừng di chuyển hay không(trạng thái auto walk)
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects);
	void Whopping();
	void UsingSubWeapon();


	//Vẽ Simon lên màn hình
	void Render();


	//Set trạng thái của Simon (Stand,Jump,Sit,....)
	void SetState(int state);


	//Lấy những object có khả năng va chạm với simon từ listObjects và đưa vào danh sách coObjects
	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);
	

	//Lấy vùng bao của Simon
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	//Kiểm tra đã render xong những animation đặc biệt chưa????
	bool isFinishRenderAnimation();


	// Properties change (HUD)
	void LoseHP(int x);
	void AddHP(int x);

};

