#pragma once

#include "CGameObject.h"
#include "Simon.h"

class MorningStar : public CGameObject
{

	int damage = 0; //Sát thương mà MS gây ra

public:

	//Constructor
	MorningStar();	

	//Lấy vị trí MS để render dựa vào vị trí của Simon
	void SetPosition(Simon* simon);

	//Override CGameObject
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};

	//Update theo trạng thái của Simon
	//Xử lí va chạm giữa MorningStar với các đối tượng khác
	void Update(DWORD dt, Simon* simon, vector<LPGAMEOBJECT> listObjects);
	
	//Overide lại hàm ở lớp CGameobject
	//Hàm này không cần gọi vì dùng hàm render bên dưới để đồng bộ với cánh tay của Simon
	virtual void Render() {}


	//Hàm để render morningStar theo cử động tay của Simon
	void Render(int currentFrameSimon = -1);


	//Xét va chạm giữa MorningStar và 1 object nào đó
	//trả về True nếu có , false nếu không
	bool CheckCollision(float otherLeft, float otherTop, float otherRight, float otherBottom);

	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);

	//Lấy vùng bao của MorningStar
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	//Upgrade level của MorningStar
	void UpgradeMorningStar();




};

