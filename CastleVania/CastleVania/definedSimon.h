#pragma once


//OBJECT TYPE ID
#define OBJECT_TYPE_SIMON	0


//WIDTH & HEIGHT
#define SIMON_WIDTH			60
#define SIMON_HEIGHT			60
#define SIMON_SIT_HEIGHT			46

//STATES ID
#define STANDING			0
#define WALKING			1
#define SITTING				2
#define JUMPING			3
#define WHOPPING_SITTING			4
#define WHOPPING_STANDING		5
#define UPGRADE_MS		6
#define	PUSHED			7


//PROPERTIES STATE VALUE
#define SIMON_WALKING_SPEED			0.12f	//Vận tốc đi của Simon
#define SIMON_JUMPING_SPEED			0.50f	//Vận tốc nhảy của Simon
#define SIMON_GRAVITY				0.002f	//Trọng lực của Simon
#define SIMON_PUSHED_X		0.08f
#define SIMON_PUSHED_Y		0.2f


//EFFECT
#define WHOPPING_TIME			300		//Thời gian chờ để Simon đánh xong 
#define UPGRADING_TIME		300		//Thời gian chờ để Simon Upgrade MorningStar (hiệu ứng đổi màu)
#define PUSHING_TIME		600		//Thời gian chờ để Simon bị đẫy ra khi chạm enemy (hiệu ứng nhấp nháy)