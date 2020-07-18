#pragma once


//OBJECT TYPE ID
#define OBJECT_TYPE_SIMON	0


//WIDTH & HEIGHT
#define SIMON_WIDTH			60
#define SIMON_HEIGHT			60
#define SIMON_SIT_HEIGHT			46

//Phần thừa từ sprite cắt ra
#define SIMON_REDUNDANT_HAND 14
#define SIMON_REDUNDANT_ELBOW 32



//STATES ID
#define STANDING			0
#define WALKING			1
#define SITTING				2
#define JUMPING			3
#define WHOPPING_SITTING			4
#define WHOPPING_STANDING		5
#define UPGRADE_MS		6
#define	PUSHED			7
#define DEAD			8
#define WALKING_UP_STAIR		9
#define WALKING_DOWN_STAIR		10
#define WALKING_UP_STAIR_WHOPPING	11
#define WALKING_DOWN_STAIR_WHOPPING	12


//PROPERTIES STATE VALUE
#define SIMON_WALKING_SPEED			0.105f	//Vận tốc đi của Simon
#define SIMON_JUMPING_SPEED			0.62f	//Vận tốc nhảy của Simon
#define SIMON_GRAVITY				0.0025f	//Trọng lực của Simon
#define SIMON_PUSHED_SPEED		0.05f    //Vận tốc simon bị đẩy ra khi chạm phải Enemy

#define SIMON_VX_ON_STAIR	0.08f       //Vận tốc của Simon theo trục X khi đi trên thang
#define SIMON_VY_ON_STAIR	0.08f       //Vận tốc của Simon theo trục Y khi đi trên thang



 
//EFFECT TIME
#define WHOPPING_TIME			300		//Thời gian chờ để Simon đánh xong 
#define UPGRADING_TIME		300		//Thời gian chờ để Simon Upgrade MorningStar (hiệu ứng đổi màu)
#define PUSHING_TIME		600		//Thời gian chờ để Simon bị đẫy ra khi chạm enemy (hiệu ứng nhấp nháy)
#define WALKING_ON_STAIR_TIME	200		//Thời gian chờ để simon bước lên một bậc thang
#define SIMON_IMMORTAL_TIME	1000	//Thời gian bất tử của Simon
#define SIMON_DEAD_TIME 2000        //Thời gian chờ để Reset lại màn chơi
#define SIMON_JUMPING_TIME 500


//INITALIZE SIMON VALUE
#define SIMON_INITIAL_SCORE 0                   //không có điểm
#define SIMON_INITIAL_DOUBLE_OR_TRIPLE -1       //Không có double shot hay triple shot
#define SIMON_INITIAL_SUBWEAPON_COUNTER 10      //Được sử dùng 10 đơn vị vũ khí phụ
#define SIMON_INITIAL_LIVE 3                    //Có 3 mạng
#define SIMON_INITIAL_SUBWEAPON_ID -1           //Không dùng vũ khí phụ nào
#define SIMON_INITIAL_HP 16                     //HP tối đa của Simon