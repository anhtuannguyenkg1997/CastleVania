#pragma once

//OBJECT TYPE ID
#define OBJECT_TYPE_STAIR	11


//ANI SET ID STAIR
#define STAIR_LEFT_RIGHT_ANI 0		//Animation set ID của stair dẫn từ trái qua phải
#define STAIR_RIGHT_LEFT_ANI 1		//Animation set ID của stair dẫn từ phải qua trái


//ORIENTATION 
//Tính từ chân của thang đến đỉnh của thang
#define STAIR_ORI_LEFT_TO_RIGHT	1	//Hướng từ trái qua phải (hướng chiều dương)
#define STAIR_NORMAL 0				//Bậc thang bình thường
#define STAIR_ORI_RIGHT_TO_LEFT	-1	//Hướng từ phải qua trái (hướng chiều ãm)
#define STAIR_DOUBLE	2			//Chuyển tiếp trạng thái cho Simon khi đi thang đôi 


//BREAK POINT STAIR
#define STAIR_TOP		1			//Đỉnh cầu thang
#define STAIR_BOTTOM	-1			//Chân cầu thang


//WIDTH & HEIGHT
#define STAIR_WIDTH		16
#define STAIR_HEIGHT	16