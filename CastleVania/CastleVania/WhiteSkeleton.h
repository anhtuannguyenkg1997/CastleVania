#pragma once
#include "Enemy.h"
#include "Simon.h"
#include "Bone.h"

class WhiteSkeleton : public Enemy
{
	int jumpState = WHITE_SKELETON_NO_JUMP;				//Trạng thái nhảy của Simon
	bool isStanding = true;								//Kiểm tra có phải simon đang đứng trên brick hay không
	bool isCreateBone = false;							//Cho phép đánh bằng Bone

	DWORD timeStartAttack = -1;							//Thời gian Bắt đầu đếm lùi đến sự kiện tấn công

	float deltaVX;										//Biến thiên vận tốc VX cho phép WhiteSkeleton tiến tới lui khi đứng

	int allowJumpLeft, allowJumpRight;					//Cho phép nhảy về bên phải hay bên trái

	Bone* bone;
public:
	WhiteSkeleton(float x, float y, int Type, bool isActive, int State, int allowJumpLeft,int allowJumpRight);


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject){ }


	virtual void Render();

	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects);

	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom);

	virtual void Attacked(int lostHP);

};