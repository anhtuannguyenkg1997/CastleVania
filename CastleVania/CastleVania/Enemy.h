#pragma once
#include "CGameObject.h"
#include "Simon.h"

class Enemy : public CGameObject
{
protected:
	int HP;				//Máu của Enemy
	int ATK;			//Lượng máu mà Enemy tấn công
public:

	Enemy();	
	Enemy(float x, float y, int Type, bool isActive, int State) :CGameObject(x, y,Type,isActive,State) {}


	~Enemy();

	
	//Xét va chạm của enemy
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);


	//Vẽ Enemy
	virtual void Render() = 0;


	//Thay đổi trạng thái của Enemy
	virtual void SetState(int state);				


	//Lấy vùng bao của Enemy
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom) = 0;


	//Tính số HP còn lại của Enemy
	virtual void Attacked(int lostHP);

	int GetATK() { return ATK; }

	//Lấy HP của Enemy
	int GetHP() { return HP; }



};

