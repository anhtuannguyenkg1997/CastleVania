#pragma once
#include "CGameObject.h"

class Torch : public CGameObject
{
public:
	Torch(float x, float y, int Type, bool isActive, int State);


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

