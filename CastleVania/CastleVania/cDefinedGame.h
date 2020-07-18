#pragma once
//ID TEXTURE BOUNDING BOX
#define ID_TEX_BBOX -100


//GAME DEFINITION
#define MAX_FRAME_RATE		90							//Số Frame tối đa render trong 1s


//CAMERA SIZE
#define CAMERA_WIDTH		512							//Chiều dài Camera
#define CAMERA_HEIGHT		384							//Chiều rộng Camera


//GRID & CELL
#define DEFAULT_CELL_WIDTH		256						//Chiều dài một Cell
#define DEFAULT_CELL_HEIGHT		192						//Chiều rộng một Cell


//TILEMAP
#define TILE_WIDTH	32									//Chiều dài một BlockTile
#define TILE_HEIGHT	32									//Chiều dài một BlockTile
#define ID_TEX_TILEMAP 10000							//ID Texture của TileMap




//OBJECT TYPE
#define STATIC_OBJECT	0								//Object tĩnh (không di chuyển)
#define DYNAMIC_OBJECT	1								//Object động (di chuyển)
#define NOT_RENDER		6								//Object không render
#define HIDDEN_OBJECT	3								//Object Item ẩn

//OBJECT TYPE BRICK
#define BOSS_BRICK		2								//Brick làm vùng bao để simon không ra khỏi phạm vi đấu Boss
#define SLIM_BRICK		4								//Brick có boundingbox bottom rất mỏng 2px để không va chạm item khi rớt xuống
#define ENEMY_BRICK		5								//Brick dành riêng để xét va chạm với enemy (BlackKnight)



//ORIENTATION DEFINITION								//hướng của một ObjectGame										
#define OR_MINUS		-1								///quay về chiều âm
#define OR_PLUS			1								///quay về chiều dương



//COLISION DIRECTION									//Hướng va chạm - định nghĩa cho object đem ra xét va chạm
#define COLLISION_DIRECTION_LEFT		 1.0f			//Hướng va chạm bên trái object đang xét
#define COLLISION_DIRECTION_RIGHT		-1.0f			//Hướng va chạm bên phải object đang xét
#define COLLISION_DIRECTION_TOP			 1.0f			//Hướng va chạm bên trên object đang xét
#define	COLLISION_DIRECTION_BOTTOM		-1.0f			//Hướng va chạm bên dưới object đang xét



//ON CAMERA	???											//Object này có nằm trong camera hay không????
#define INSIDE_CAMERA	1								//Nằm trong camera (GameObject, Tilemap)
#define OUTSIDE_CAMERA	0								//Nằm ngoài camera (HUD)


//FLIPPING ????											//Có lật sprite hay không???
#define FLIPPING		 1								//Lật sprire
#define NOT_FLIPPING	-1								//Không lật sprite


//TIME
#define DISAPPEAR_TIME		450					//Thời gian cho hiệu ứng biến mất của object khác