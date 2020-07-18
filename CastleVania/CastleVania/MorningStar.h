#pragma once

#include "CGameObject.h"
#include "Simon.h"
#include "AttackingEffect.h"

class MorningStar : public CGameObject
{

	int damage = 0; //Sát thương mà MS gây ra

	bool playSound = false;

	float topMS; //Vị trí top của MS. Dùng để render ra effect attack

	AttackingEffect* effect;	//Tạo effect khi đánh trúng object

public:

	//Constructor
	MorningStar();	

	//Lấy vị trí MS để render dựa vào vị trí của Simon
	void SetPosition(Simon* simon);

	//Override CGameObject
	void Update(DWORD dt, vector<LPGAMEOBJECT> objects) {};

	//Update theo trạng thái của Simon
	//Xử lí va chạm giữa MorningStar với các đối tượng khác
	void Update(DWORD dt, Simon* simon, vector<LPGAMEOBJECT> &listObjects);
	


	//Overide lại hàm ở lớp CGameobject
	//Hàm này không cần gọi vì dùng hàm render bên dưới để đồng bộ với cánh tay của Simon
	virtual void Render() {}


	//Hàm để render morningStar theo cử động tay của Simon
	void Render(int currentFrameSimon = -1);


	//Xét va chạm giữa MorningStar và 1 object nào đó
	//trả về True nếu có , false nếu không
	bool CheckCollision(float otherLeft, float otherTop, float otherRight, float otherBottom);

	//Lấy object có thể va chạm với MS
	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);

	//Lấy vùng bao của MorningStar
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	//Upgrade level của MorningStar
	void UpgradeMorningStar();

};

