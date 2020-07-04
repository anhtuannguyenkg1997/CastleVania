#pragma once


//LIST GROUP RANDOM
//Bất kì xử lí nào trong game cần random thì tạo một list chứa các phần tử có thể xảy ra
#define LIST_RANDOM_PHANTOMBAT_FLYING_ATTACK 0					//Random cách PB tấn công theo loại 1 hay loại 2
#define LIST_RANDOM_PHANTOMBAT_FLYING_ATTACK_TYPE1 1			//PB tấn công theo loại 1 Random theo 2 cách 
#define LIST_RANDOM_PHANTOMBAT_FLYING_ATTACK_TYPE2 2			//PB tấn công theo loại 2 Random theo 3 cách
#define LIST_RANDOM_PHANTOMBAT_HIT_SIMON 7						//Random liệu PB có đánh trúng simon hay là đánh random

#define LIST_RANDOM_ITEM_TYPE 3									//Random xuất hiện loại item đặc biệt hay item thường
#define LIST_RANDOM_NORMAL_ITEM 4								//Random xuất hiện 1 trong các item thường
#define LIST_RANDOM_SPECIAL_ITEM 5								//Random xuất hiện 1 trong các item đặc biệt

#define LIST_RANDOM_WHITESKELETON_STATE 6						//Random 1 trong 2 trạng thái tấn công của whiteskeleton





//==================================== START VALUE PHANTOMBAT RANDOM ====================================
//PHANTOMBAT FLYING ATTACK 1
#define PHANTOMBAT_FLYING_ATTACK_TYPE1 1							//ID Loại tấn công 1 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE1_RATIO 50						//Tỉ lệ xuất hiện Loại tấn công 1 của PB

#define PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND1	1					//ID kiểu 1 trong loại tấn công 1 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND2	2					//ID kiểu 2 trong loại tấn công 1 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND3	3					//ID kiểu 3 trong loại tấn công 1 của PB

#define PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND1_RATIO	33				//tỉ lệ xuất hiện kiểu 1 trong loại tấn công 1 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND2_RATIO	33				//tỉ lệ xuất hiện kiểu 2 trong loại tấn công 1 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND3_RATIO	34				//tỉ lệ xuất hiện kiểu 3 trong loại tấn công 1 của PB




//PHANTOMBAT FLYING ATTACK 2
#define PHANTOMBAT_FLYING_ATTACK_TYPE2 2							//ID Loại tấn công 2 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE2_RATIO 50						//Tỉ lệ xuất hiện Loại tấn công 2 của PB

#define PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND1	1					//ID kiểu 1 trong loại tấn công 2 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND2	2					//ID kiểu 2 trong loại tấn công 2 của PB

#define PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND1_RATIO	50				//tỉ lệ xuất hiện kiểu 1 trong loại tấn công 2 của PB
#define PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND2_RATIO	50				//tỉ lệ xuất hiện kiểu 2 trong loại tấn công 2 của PB


//PHANTOMBAT HIT SIMON
#define PHANTOMBAT_HIT_SIMON 1										//ID PB tấn công trúng Simon
#define PHANTOMBAT_NO_HIT_SIMON 2									//ID PB tấn công không trúng Simon

#define PHANTOMBAT_HIT_SIMON_RATIO 65								//tỉ lệ PB tấn công trúng Simon
#define PHANTOMBAT_NO_HIT_SIMON_RATIO 35							//tỉ lệ PB tấn công không trúng Simon



//==================================== END VALUE PHANTOMBAT RANDOM ====================================







//==================================== START VALUE ITEM RANDOM ====================================
//ITEM TYPE
//MS LEVEL 1
#define RD_ITYPE_NORMAL_MSLEVEL1_RATIO	75				//Tỉ lệ xuất hiện item thường khi MS ở level 1
#define RD_ITYPE_SPECIAL_MSLEVEL1_RATIO	25				//Tỉ lệ xuất hiện item hiếm khi MS ở level 1
//MS LEVEL 2
#define RD_ITYPE_NORMAL_MSLEVEL2_RATIO	70				//Tỉ lệ xuất hiện item thường khi MS ở level 2
#define RD_ITYPE_SPECIAL_MSLEVEL2_RATIO	30				//Tỉ lệ xuất hiện item hiếm khi MS ở level 2
//MS LEVEL 3
#define RD_ITYPE_NORMAL_MSLEVEL3_RATIO	60				//Tỉ lệ xuất hiện item thường khi MS ở level 3
#define RD_ITYPE_SPECIAL_MSLEVEL3_RATIO	40				//Tỉ lệ xuất hiện item hiếm khi MS ở level 3




//NORMAL ITEM
#define RD_NORMAL_ITEM_MORNINGSTAR_RATIO	10			//Tỉ lệ xuất hiện item MorningStar
#define RD_NORMAL_ITEM_SMALLHEART_RATIO		40			//Tỉ lệ xuất hiện item Smallheart
#define RD_NORMAL_ITEM_BIGHEART_RATIO		20			//Tỉ lệ xuất hiện item BigHeart
#define RD_NORMAL_ITEM_MB_RED_RATIO			15			//Tỉ lệ xuất hiện item MoneyBag red
#define RD_NORMAL_ITEM_MB_PURPLE_RATIO		10			//Tỉ lệ xuất hiện item MoneyBag purple
#define RD_NORMAL_ITEM_MB_WHITE_RATIO		5			//Tỉ lệ xuất hiện item MoneyBag white


//SPECIAL ITEM
//40% - OTHER
#define RD_SPECIAL_ITEM_ROSARY_RATIO		18			//Tỉ lệ xuất hiện item tiêu diệt mọi enemy
#define RD_SPECIAL_ITEM_INVISIBILITYPOTION	12			//Tỉ lệ xuất hiện item vô hình
#define RD_SPECIAL_ITEM_DOUBLESHOT			7			//Tỉ lệ xuất hiện item doubleshot
#define RD_SPECIAL_ITEM_TRIPLESHOT			3			//Tỉ lệ xuất hiện item trippleshot

//60% - SUBWEAPON
#define RD_SPECIAL_ITEM_HOLYWATER	20					//Tỉ lệ xuất hiện item Holywater
#define RD_SPECIAL_ITEM_AXE			16					//Tỉ lệ xuất hiện item Axe
#define RD_SPECIAL_ITEM_DAGGER		14					//Tỉ lệ xuất hiện item Dagger
#define RD_SPECIAL_ITEM_BOOMERANG	5					//Tỉ lệ xuất hiện item Boomerang
#define RD_SPECIAL_ITEM_STOPWATCH	5					//Tỉ lệ xuất hiện item stop watch

//==================================== END VALUE ITEM RANDOM ====================================





//==================================== START VALUE WHITESKELETON RANDOM ====================================
//WHITE SKELETON STATE BONE AND JUMP
#define RD_WHITESKELETON_CREATEBONE 1					//ID WS tấn công bằng cách bắn xương
#define RD_WHITESKELETON_JUMPING 2						//ID WS tấn công bằng cách nhảy đến trước

#define RD_WHITESKELETON_CREATEBONE_RATIO 70			//tỉ lệ WS tấn công bằng cách bắn xương
#define RD_WHITESKELETON_JUMPING_RATIO 30				//tỉ lệ WS tấn công bằng cách nhảy đến trước
//==================================== END VALUE WHITESKELETON RANDOM ====================================