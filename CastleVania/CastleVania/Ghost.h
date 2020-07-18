#pragma once
#include "Enemy.h"
#include "Simon.h"

class Ghost : public Enemy
{
	float deltaVY;
public:
	Ghost(float x, float y, int Type, bool isActive, int State);


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject) {}

	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom);

	virtual void Attacked(int lostHP);

};

