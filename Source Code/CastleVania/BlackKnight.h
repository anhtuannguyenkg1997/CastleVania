#pragma once
#include "Enemy.h"


class BlackKnight : public Enemy
{
public:
	BlackKnight(float x, float y, int Type, bool isActive, int State);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon);

	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom);

	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);

	virtual void Attacked(int lostHP);

};

