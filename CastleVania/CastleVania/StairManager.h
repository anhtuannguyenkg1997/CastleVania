#pragma once
#include "Stair.h"
#include "Simon.h"

class StairManager
{
	static StairManager* _instance;		//Thể hiện duy nhất của StairManager

	vector<Stair*> listStair;			//Danh sách các bậc thang của màn chơi

	bool isStandOnStair = false;		// trạng thái đang đứng trên cầu thang 
	bool ableWalkingUp = false;			// sẵn sàng để di chuyển lên cầu thang
	bool ableWalkingDown = false;		// sẵn sàng để chuyển xuống cầu thang
	int stairOrientation = 0;			//Hướng bậc thang breakpoint (1 - trái qua phải, -1 từ phải qua trái) - dựa vào state mà lấy
public:
	bool isTouchingBreakDoubleStair = false; //Va chạm bậc thang để đảo chiều simon khi đi thang đôi 
	static StairManager* GetInstance();	//Lấy thể hiện duy nhất của StairManager

	Stair* stairCollided = nullptr; // lưu bậc thang va chạm với simon -> để xét vị trí đi tới cầu thang để lên - xuống
	

	//Load toàn bộ stair từ listObjects
	void LoadListStair(vector<LPGAMEOBJECT> *listObjects);



	//Luôn kiểm tra va chạm giữa Simon và breakPoint cầu thang mỗi khi player nhấn phím
	//Xử lí nếu thực sự có va chạm
	void Update(Simon* simon);


	//Xử lí simon đi lên/xuống/đang đứng trên cầu thang
	void SimonWalkingDownStair(Simon* simon);
	void SimonWalkingUpStair(Simon* simon);
	void SimonStandingOnStair(Simon* simon);
	



	//GETTER
	bool GetIsSimonStandingOnStair() { return this->isStandOnStair; }
	bool GetAbleSimonWalkingUp() { return this->ableWalkingUp; }
	bool GetAbleSimonWalkingDown() { return this->ableWalkingDown;  }
	int GetStairOrientation() { return this->stairOrientation; }

	//SETTER
	void SetIsSimonStandingOnStair(bool isStandOnStair) { this->isStandOnStair= isStandOnStair; }
	void SetAbleSimonWalkingUp(bool ableWalkingUp) { this->ableWalkingUp = ableWalkingUp; }
	void SetAbleSimonWalkingDown(bool ableWalkingDown) { this->ableWalkingDown= ableWalkingDown; }
	void SetStairOrientation(int stairOrientation) { this->stairOrientation = stairOrientation; }
};

