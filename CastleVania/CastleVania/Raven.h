#pragma once
#include "Enemy.h"
#include "Simon.h"

class Raven : public Enemy
{
	DWORD timeStartAttack=-1; //Thời gian raven bắt đầu tấn công (bay xuống/lên một đoạn)
	DWORD timeBeforeAttack=-1; //Thời gian trước khi raven tấn công

public:
	Raven(float x, float y, int Type, bool isActive, int State);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject) {}


	virtual void Render();

	virtual void SetState(int state);

	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom);

	virtual void Attacked(int lostHP);

};

