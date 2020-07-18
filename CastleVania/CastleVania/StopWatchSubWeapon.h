#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "definedSubWeapon.h"

class StopWatchSubWeapon
{
	//Thể hiện duy nhất của StopWatch
	//Do chỉ có thể dùng 1 lần và cách nhau một khoảng thời gian tương đối dài
	static StopWatchSubWeapon* _instance;

	//Thời gian bắt đầu stopWatch, lưu lại để biết khi nào hết thời gian dùng SW
	DWORD timeStart; 
public:

	//Lấy thể hiện của StopWatch
	static StopWatchSubWeapon* GetInstance();
	
	//Bắt đầu đểm thời gian dùng SW. Lúc này là lúc bắt đầu dùng SW
	void StartStopWatch(DWORD time) { timeStart = time; }


	//Hàm kiểm tra xem đã hết thời gian dùng SW chưa????
	bool IsStopWatchTimeUp() { return GetTickCount() - timeStart > WEAPONS_STOP_WATCH_TIME; }


};

