#include "Enemy.h"



Enemy::Enemy()
{

}

Enemy::~Enemy()
{
}



void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects)
{
	CGameObject::Update(dt, listObjects);
}


void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	CGameObject::Update(dt, listObjects);
}



//Set trạng thái của enemy
void Enemy::SetState(int state)
{
	CGameObject::SetState(state);
}




//Tính lượng HP còn lại
void Enemy::Attacked(int lHP)
{
	HP -= lHP;

	if (HP <= 0)
		HP = 0;
}