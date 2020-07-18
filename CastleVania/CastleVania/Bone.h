#pragma once
#include "CGameObject.h"
#include "Simon.h"


class Bone:public CGameObject
{
protected:
	int damage;			//Sát thương mà Subweapon gây ra
public:
	Bone();
	~Bone() {};

	int GetATK() { return this->damage; }


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);

	virtual void Render();


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	//Lấy những object có khả năng va chạm với simon từ listObjects và đưa vào danh sách coObjects
	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon);
};

