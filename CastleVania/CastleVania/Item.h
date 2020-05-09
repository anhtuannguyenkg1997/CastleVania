#pragma once

#include "CGameObject.h"

class Item : public CGameObject
{
	DWORD timeAppear;   // thời gian Item hiện ra
public:
	Item(LPGAMEOBJECT objectTarget);

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

};

