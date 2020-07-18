#pragma once
#include "Enemy.h"
#include "Simon.h"

class HunchBack : public Enemy
{
	bool isActive = false; //Kiểm tra đã active chưa. Ban đầu HB đứng yên, khi cách simon 1 khoảng nhất định thì nó nhảy xuống
	DWORD timeJumped = -1; //Thời gian nhảy gần nhất
public:

	HunchBack(float x, float y, int Type, bool isActive, int State);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject) {}


	virtual void Render();

	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon);

	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom);

	virtual void Attacked(int lostHP);

	void SetTimeJumped(DWORD timeJumped) { this->timeJumped = timeJumped; }
	void SetIsActive(bool isActive) { this->isActive = isActive; }
};
