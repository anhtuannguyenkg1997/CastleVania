#pragma once
#include <Windows.h>
#include "definedItems.h"

class RosaryItemEffect
{
	static RosaryItemEffect* _instance;			//Thể hiện duy nhất của RosaryItemEffect
	
	DWORD timeStartRosary;						//Thời gian bắt đầu Rosary
	
public:
	static RosaryItemEffect* GetInstance();		//Lấy thể hiện duy nhất của RosaryItemEffect

	bool isRosaryEffect = false;				//Đang sử dụng Rosary hay không


	//Ghi lại thời gian bắt đầu sử dụng Rosary
	void StartRosary(DWORD time) { timeStartRosary = time; isRosaryEffect = true; }


	//Đang sử dụng Rosary hay không
	bool IsRosary() { return GetTickCount() - timeStartRosary < ITEM_ROSARY_TIME; }
};

