#pragma once
#include "Enemy.h"
#include "Simon.h"
#include "Brick.h"
#include "CObjectCell.h"
#include "CRandomManager.h"


class PhantomBat : public Enemy
{
	//Random trạng thái bay cho PhantomBat
	CRandomManager* rdManager = CRandomManager::GetInstance();

	bool isCreateLimitRegion = false;	//Tạo brick giới hạn vùng bao đấu boss

	bool isFallingUp = false;			//Trọng lực hướng lên trên tạo độ cong khi bay ngang cho phantomBat

	DWORD timeFlying = -1;				//Ghi lại thời gian bắt đầu bay. Để biết khi nào bắt đầu chờ
	DWORD timeWaiting = -1;				//Ghi lại thời gian bắt đầu chờ. Để biết khi nào bắt đầu bay

	int SequenceFlyingType=-1;			//Type Flying mà phantomBat sẽ bay hiện tại
	int SFType1_ATK = -1;				//Loại tấn công theo Type 1
	int SFType2_ATK = -1;				//Loại tấn công theo Type 2

	//Trạng thái bay hiện tại (kiểm soát xem chạy hết 1 squence type flying hay chưa)
	int StateFlyingCurrent = PHANTOMBAT_FLY_STATE_BEGIN;			

public:

	PhantomBat(float x, float y, int Type, bool isActive, int State);


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& coObject){}


	//Lấy Object có khả năng va chạm với PhantomBat
	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon);

	virtual void Render();

	virtual void SetState(int state);


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxStart(float& left, float& top, float& right, float& bottom){}


	int GetDirectionNeedAttack(Simon *simon);		//Trả về hướng cần đánh trúng simon

	bool IsHitSimon();								//Có đánh trúng simon hay không

	void WaitingForNextFlying(Simon *simon);		//Xử lí việc chờ đến trạng thái Bay
	void WaitingForNextIdling();					//Xử lí việc chờ đên trạng thái Đứng yên

	void FlyingSquenceType1();						//Bay theo loại 1
	void FlyingSquenceType2();						//Bay theo loại 2


	virtual void Attacked(int lostHP);				//Bị tấn công
};
