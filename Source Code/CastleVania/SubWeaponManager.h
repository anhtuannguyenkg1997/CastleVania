#pragma once
#include "SubWeapon.h"
#include "Simon.h"


class SubWeaponManager
{
	static SubWeaponManager* _instance;
public:

	static SubWeaponManager* GetInstance();


	int maxSubWeaponCreate = SW_NORMAL_CREATED;		//Số lượng subweapon tối đa mà simon có thể ném ra
	int numSubWeaponCreated = 0;					//Số lượng subweapon đang xuất hiện


	bool isCreateNextSubWeapon = true;				//Cho phép tạo thêm Subweapon nữa hay không???
	DWORD startTimeCreateSubweapon = -1;			//Thời gian bắt đầu tạo ra một Subweapon (-1 để chạy lần đầu hàm getTickCount)
													//Nếu quá thời gian cần đợi để tạo subweapon thì cho phép tạo subweapon
													//Dùng để loại bug tạo ra quá nhiều subweapon liên tiếp nhau. Vì vậy mỗi thời điểm ta chỉ cho tạo 1 subweapon


	bool createSubweapon = false;					//Kiểm tra có phải đánh bằng subweapon hay không??
	bool isUseStopWatch = false;					//Đang dùng stop watch

	bool isHitBySubweapon = false;



	void Update(Simon* simon, DWORD dt);



	void CheckStopWatchOverTime();



	//Hàm kiểm tra đủ điều kiện để tạo StopWatch chưa 
	bool isAvailableCreateStopWatch();


	//Hàm kiểm tra đủ điều kiện để tạo SubWeapon khác chưa 
	bool isAvailableCreateOtherSubWeapon();


};

