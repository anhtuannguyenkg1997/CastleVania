#pragma once
#include "CGameObject.h"


class Enemy : public CGameObject
{
protected:
	int HP;				//Máu của Enemy
public:

	Enemy();			
	~Enemy();

	
	//Xét va chạm của enemy
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);



	//Vẽ Enemy
	virtual void Render() = 0;


	//Thay đổi trạng thái của Enemy
	virtual void SetState(int state);				


	//Lấy vùng bao của Enemy
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;


	//Tính số HP còn lại của Enemy
	virtual void LoseHP(int x);


	//Lấy HP của Enemy
	int GetHP() { return HP; }



};

