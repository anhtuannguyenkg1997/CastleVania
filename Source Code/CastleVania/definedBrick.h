#pragma once

//OBJECT TYPE ID
#define OBJECT_TYPE_BRICK		8				//ID TEXTURE Brick


//SPECIAL BRICK
#define BRICK_BROKEN				0			//Type Brick có khả năng bị bể khi va chạm với Weapon
#define BRICK_ACTIVE_HIDDEN_OBJECT	1			//Type Brick có khả năng kích hoạt hidden item nếu chạm vào

//WIDTH & HEIGHT
#define BRICK_WIDTH			32					//Chiều dài Brick
#define BRICK_HEIGHT		32					//Chiều rộng Brick


												//Điều chỉnh tọa độ của Sprite Broken Brick vào trung tâm viên gạch bị bể
#define BROKEN_EFFECT_X	50						//Kích thước dời ra theo chiều X tính từ tọa độ X của Brick bị bể
#define BROKEN_EFFECT_Y	30						//Kích thước dời ra theo chiều Y tính từ tọa độ X của Brick bị bể
#define BROKEN_BLOCK_TIME		300				//Thời gian hiệu ứng broken brick