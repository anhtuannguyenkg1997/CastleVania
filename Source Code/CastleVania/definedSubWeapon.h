#pragma once


#define OBJECT_TYPE_SUBWEAPON 9


//STATE
#define SUBWEAPON_ACTIVE	0
#define W_HOLYWATER_BOTTLE_STATE	0
#define W_HOLYWATER_FIRE_STATE	1


// SUBWEAPON ANIMARION SET ID
#define WEAPONS_DAGGER		900
#define	WEAPONS_AXE			901
#define WEAPONS_HOLY_WATER	902
#define WEAPONS_BOOMERANG	903
#define	WEAPONS_STOP_WATCH	904
#define WEAPONS_HOLY_WATER_SHATTERED	905




// WIDTH & HEIGHT
#define W_HOLYWATER_BOTTLE_WIDTH	16
#define W_HOLYWATER_BOTTLE_HEIGHT	16

#define W_HOLYWATER_FIRE_WIDTH	32
#define W_HOLYWATER_FIRE_HEIGHT	30


#define W_DAGGER_WIDTH		34
#define W_DAGGER_HEIGHT		18

#define W_AXE_WIDTH			30
#define W_AXE_HEIGHT			28

#define W_BOOMERANG_WIDTH	28
#define W_BOOMERANG_HEIGHT	28






//DAMAGE
#define W_STRONG_DAMAGE	2	//AXE
#define W_NORMAL_DAMAGE	1	//DAGGER, BOOMERANG, HOLYWATER





// SUBWEAPON PROPERTIES
#define WEAPONS_DAGGER_SPEED		0.378f			//Vận tốc di chuyển của Dagger (theo trục X)

#define WEAPONS_AXE_SPEED			0.22f			//Vận tốc di chuyển của AXE theo trục X 
#define WEAPONS_AXE_FLYING_SPEED	0.5f			//Vận tốc di chuyển của AXE theo trục Y
#define WEAPONS_AXE_GRAVITY			0.0015f			//Trọng lực của AXE (làm cho AXE bị rơi)

#define WEAPONS_HOLY_WATER_SPEED_X	0.3f
#define WEAPONS_HOLY_WATER_SPEED_Y	0.18f
#define WEAPONS_HOLY_WATER_GRAVITY	0.001f


#define WEAPONS_BOOMERANG_SPEED				0.48f			//Vận tốc di chuyển của Boomerang (theo trục X)	
#define WEAPONS_BOOMERANG_DELTAVX			0.01f			//Biến thiên vận tốc của Boomerang (Tạo hiệu ứng di chuyển ngược lại)



//SUBWEAPON VALUES 
#define SWC_FOR_STOPWATCH_SUBWEAPON 5						//Số lượng quả tim nhỏ để dùng StopWatch
#define SWC_FOR_OTHER_SUBWEAPON 1							//Số lượng quả tim nhỏ để dùng SubWeapon còn lại


#define SW_NORMAL_CREATED 1 //Số subweapon tối đa mà khi chưa ăn Double hoặc Tripple tạo ra
#define SW_FOR_DOUBLE_SHOT_CREATED 2						//Số subweapon tối đa mà double shot tạo ra
#define SW_FOR_TRIPPLE_SHOT_CREATED 3						//Số subweapon tối đa mà tripple shot tạo ra
	


//SUBWEAPON TIMER
#define WEAPON_CREATED_TIME 300	//Khoảng thời gian chờ giữa 2 lần liên tiếp tạo subweapon (giải quyết cho trouble shot or tripple shot)
#define WEAPONS_STOP_WATCH_TIME		5000	//Thời gian của mỗi lần dùng StopWatch
#define WEAPONS_HOLY_WATER_TIME_EFFECT	1000	//Thời gian HolyWater tác động đến enemy (từ lúc render đốm lửa cháy)