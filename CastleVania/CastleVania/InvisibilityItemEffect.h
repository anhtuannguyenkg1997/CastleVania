#pragma once
#include <Windows.h>
#include "definedItems.h"

class InvisibilityItemEffect
{
	DWORD timeStartInvisibility;				//Thời gian Simon bắt đầu vô hình

	static InvisibilityItemEffect* _instance;	//Thể hiện duy nhất của RosaryItemEffect

public:
	bool isInvisibility = false;				//Simon có đang vô hình hay không

	static InvisibilityItemEffect* GetInstance();//Thể hiện duy nhất của RosaryItemEffect

	//Ghi lại thời gian bắt đầu sử dụng Invisibility
	void StartInvisibility(DWORD time) { timeStartInvisibility = time; isInvisibility = true; }

	//Simon có đang vô hình hay không
	bool IsInvisibility() { return GetTickCount() - timeStartInvisibility > ITEM_INVISIBILITY_TIME; }
};

