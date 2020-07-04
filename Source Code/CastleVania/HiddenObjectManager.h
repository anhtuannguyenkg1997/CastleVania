#pragma once
#include "Simon.h"
#include "Item.h"

class HiddenObjectManager
{
	static HiddenObjectManager* _instance;				//Thể hiện duy nhất của Camera

	Item* item;											//Lưu Hidden Item của màn hiện tại

	bool isActiveHiddenObject = false;					//Mỗi màn chỉ được active hidden object 1 lần

	bool isMoving = false;								//Object này có đi từ dưới viên gạch lên hay không

	DWORD timeStartActive =0;							//Thời gian mà hidden object được kích hoạt
public:
	bool isStartCountingTimeDisappear = false;			//Cho phép đếm thời gian để hidden item biến mất (trừ hao thời gian item chui từ brick lên trên)

	bool isTouchedBrick = false;						//Xác nhận Weapon đã chạm vào Brick để tạo hidden Item

	
	static HiddenObjectManager* GetInstance();			//Lấy thể hiện duy nhất của HiddenObjectManager

	
	void CreateHiddenObject(float x, float y, int idItem, int isMoving);	//Tạo Hidden Object


	void Update();										//Update

	
	void Reset();										//Reset tất cả value trong instace HiddenObjectManager (reset màn chơi khi simon died)



	//SETTER
	void SetActiveHiddenObject(bool isActiveHiddenObject) { this->isActiveHiddenObject = isActiveHiddenObject; }


	//GETTER
	bool GetIsMoving() { return this->isMoving; }
};

