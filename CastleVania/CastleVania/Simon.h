#pragma once

#include <vector>
#include "CGameObject.h"

using namespace std;

class Simon : public CGameObject
{
public:
	DWORD timeDied;									//Thời gian simon bắt đầu died. (để đếm thời gian phát sound died simon)

	bool isUpgradeMorningStar = false;				//Simon có được nâng cấp Morning Star hay không???

	Simon();

	//Update Simon
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects);

	//Simon vung roi
	void Whopping();

	//Simon dùng vũ khí phụ
	void UsingSubWeapon();


	//Vẽ Simon lên màn hình
	void Render();


	//Set trạng thái của Simon (Stand,Jump,Sit,....)
	void SetState(int state);


	//Lấy những object có khả năng va chạm với simon từ listObjects và đưa vào danh sách coObjects
	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);
	

	//Lấy vùng bao của Simon
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	//Kiểm tra đã render hết animation (vung roi, leo thang,...)
	//Những animation này cần thời gian chờ đế render hết, nếu không nó sẽ chuyển về trạng thái STAND rất nhanh mà k quan sát được
	bool isFinishRenderAnimation();


	// Simon bị mất máu
	void Attacked(int HP);

	//Simon được thêm máu
	void AddHP(int HP);

};

