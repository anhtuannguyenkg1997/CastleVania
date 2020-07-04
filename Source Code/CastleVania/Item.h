#pragma once

#include "CGameObject.h"
#include "Simon.h"

class Simon;

class Item : public CGameObject
{
protected:
	DWORD timeAppear;							//Thời gian lúc item xuất hiện
	bool isEffectToSimon = false;				//Cho phép xác nhận rằng simon đã ăn item
	int isMoving;								//Loại item Moving
	DWORD time;						
	bool isFalling = false;						//
public:
	Item(float x, float y);						//Vị trí xuất hiện của Item


	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects){}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;


	//Lấy Object có khả năng va chạm với Item
	virtual void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon);

	//Kiểm tra va chạm Item với các Object khác
	bool CheckCollision(float otherLeft, float otherTop, float otherRight, float otherBottom);


	//SETTER
	void SetIsMoving(int isMoving) { this->isMoving = isMoving; }

	//GETTER
	int GetIsMoving() { return this->isMoving; }


	

};

