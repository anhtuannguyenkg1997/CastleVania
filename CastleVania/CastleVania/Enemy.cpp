#include "Enemy.h"



Enemy::Enemy()
{

}

Enemy::~Enemy()
{
}



void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
}



//Set trạng thái của enemy
void Enemy::SetState(int state)
{
	CGameObject::SetState(state);
}




//Tính lượng HP còn lại
void Enemy::LoseHP(int x)
{
	HP -= x;

	if (HP <= 0)
		HP = 0;
}