#pragma once
#include "Enemy.h"
#include "Simon.h"

class VampireBat : public Enemy
{
	float deltaVY;	//Biến thiên vận tốc theo trục Y

	float simonPositionX, simonPositionY; //Simon Đến vị trí này thì dơi bắt đầu bay

public:
	VampireBat(float x, float y, int Type, bool isActive, int State);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);

	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom) {}

	virtual void Attacked(int lostHP);


	void SetSimonPointX(float simonPositionX) { this->simonPositionX = simonPositionX; }
	void SetSimonPointY(float simonPositionY) { this->simonPositionY = simonPositionY; }
};

