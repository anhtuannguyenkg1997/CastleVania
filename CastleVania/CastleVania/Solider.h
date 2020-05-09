#pragma once
#include "Enemy.h"


class Solider : public Enemy
{
public:
	Solider();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void LoseHP(int x);
};

