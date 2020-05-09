#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "CTextures.h"
#include "CSprites.h"
#include "CDebug.h"


class CTileMap
{
	CSprites* sprites = CSprites::GetInstance();	//Sprite instance
	vector<vector<LPSPRITE>> tilemap;	//Mảng 2 chiều lưu sprite (từng tileset của map)

	int mapWidth;					// chiều rộng của map
	int mapHeight;					// chiều cao của map

	int rows;					// số hàng của map
	int cols;					// số cột của map

	LPCWSTR tilemapTexture;			// filepath texture (chứa các ô tile texture)
	LPCWSTR tilemapCoordinates;		// filepath Coordinates (chứa ma trận map)


	//Thể hiện duy nhất của TileMap
	static CTileMap* _instance;
public:

	//Lấy thể hiện duy nhất của tilemap
	static CTileMap* GetInstance();

	//Load map của màn chơi
	//Mặc định một tile có width=height=32 pixel
	void LoadMap(LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height);

	//Vẽ Map
	void Render();	// vẽ tất cả các tile nằm trong camera

	//Lấy chiều dài của Map
	int GetMapWidth() { return mapWidth; }

	//Làm sạch map (mảng 2 chiều sprites)
	void Clear();


};

typedef CTileMap* LPTILEMAP;

