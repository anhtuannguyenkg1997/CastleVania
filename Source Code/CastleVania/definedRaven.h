#pragma once


//OBJECT TYPE ID
#define OBJECT_TYPE_RAVEN	19



//WIDTH & HEIGHT
#define RAVEN_WIDTH			32
#define RAVEN_HEIGHT			32




//PROPERTIES STATE VALUE
#define RAVEN_FLYING_SPEED_X		0.13f
#define RAVEN_FLYING_SPEED_Y			0.18f
#define RAVEN_GRAVITY				0.002f
#define RAVEN_ACTIVE_RANGE 185					//Khoảng cách tối thiểu giữa raven và simon để raven hoạt động (bắt đầu tấn công)
#define RAVEN_FLYING_VERTICAL_RANGE 30		//Khoảng cách tối thiểu theo trục Y để raven chỉ tấn công ngang (trái-phải)

#define RAVEN_TIME_ATTACK	400					//Khoảng thời gian tấn công. (Raven bay từ trên xuống 1 lần)
#define RAVEN_TIME_NEXT_ATTACK	1500			//Khoảng thời gian giữa hai lần tấn công (thời gian chờ)




//PROPERTIES
#define RAVEN_HP	1 
#define RAVEN_ATK	3
#define RAVEN_EXP	200