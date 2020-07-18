#pragma once
#include "CSprites.h"


//Mở rộng của Sprite
//Thực chất là sprite nhưng ta thêm 1 biến để xác định thời gian của sprite này hiển thị là time
// 1 animation tập hợp gồm nhiều frame vẽ liên tiếp nhau. mỗi frame xuất hiện một thời gian nhất định và biến mất, 
// sprite khác lập tức hiện lên và cứ như thế cho đến hết animation
class CAnimationFrame
{
	LPSPRITE sprite;		//Sprite của 1 frame animation
	DWORD time;				//Thời gian hiển thị frame này lên màn hình

public:
	//Hàm tạo 1 frame => đưa sprite và thời gian mà nó hiển thị
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }


	//Lấy ra sprite của aniamtion frame này
	LPSPRITE GetSprite() { return this->sprite; }


	//Lấy thời gian hiển thị của animation frame này
	DWORD GetTime() { return this->time; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

