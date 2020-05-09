#pragma once

//GAME DEFINITION
#define MAX_FRAME_RATE			90


//CAMERA SIZE
#define CAMERA_WIDTH		512
#define CAMERA_HEIGHT		384



//GRID & CELL
#define DEFAULT_CELL_WIDTH		256
#define DEFAULT_CELL_HEIGHT		192



//TILEMAP
#define TILE_WIDTH	32
#define TILE_HEIGHT	32
#define ID_TEX_TILEMAP 10000



//COLISION DEFINITION
// Orientation (hướng của ObjectGame)
#define OR_MINUS		-1			///Hướng về chiều âm
#define OR_PLUS		1				///Hướng về chiều dương

// Collision direction
#define DIR_LEFT		1.0f
#define DIR_RIGHT		-1.0f
#define DIR_TOP		1.0f
#define	DIR_BOTTOM		-1.0f

// IS OBJECT DEPEND ON CAMERA???	(Render Object phụ thuộc vào camera hay không)
#define DEPEND_ON_CAMERA	1		//Phụ thuộc vào camera (GameObject, Tilemap)
#define INDEPENDENT_ON_CAMERA	0	//Không phụ thuộc vào camera (HUD)


//FLIPPING ????
#define FLIPPING	1
#define NOT_FLIPPING	-1



// TIME PLAY MS (Thời gian của một màn chơi)
#define DEFAULT_TIME_PLAY		300


#define ID_TEX_HP					17
#define ID_TEX_RECT					18



//FONT
#define FILEPATH_FONT	L"Font\\prstart.ttf"