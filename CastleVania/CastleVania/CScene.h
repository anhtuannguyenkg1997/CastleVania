#pragma once

#include <d3dx9.h>
#include "CKeyEventHandler.h"

class CScene
{
protected:
	CKeyEventHandler* key_handler; //Bàn phím dành cho màn chơi
	int id; //Mã của màn chơi

	LPCWSTR objectSceneFilePath; //Đường dẫn đến file chứa các object của màn chơi
	LPCWSTR mapTexturePath; //Đường dẫn đến Texture của map (tile map)
	LPCWSTR mapDataPath;	//Đường dẫn đến Data của map (tọa độ từng tile map trên thế giới game)
	
	D3DXVECTOR2 cameraPosition; //Tọa độ của Camera trong màn này

	D3DXVECTOR2 simonPositionStart; //Tọa độ của Simon khi bắt đầu màn chơi

	int mapWidth, mapHeight; //Kích thước map của scene này



public:
	//Khởi tạo màn chơi 
	CScene(int id, LPCWSTR objectSceneFilePath, LPCWSTR mapTexturePath, LPCWSTR mapDataPath,D3DXVECTOR2 cameraPosition, D3DXVECTOR2 simonPositionStart, int mapWidth, int mapHeight);

	//Lấy bàn phím của trò chơi
	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }

	//Load màn chơi
	virtual void Load() = 0;	//Load toàn bộ scene


	//Xóa nội dung của màn chơi. Dọp dẹp texture,sprite,.....
	virtual void Unload() = 0;

	//Update màn chơi
	virtual void Update(DWORD dt) = 0;

	//Render màn chơi
	virtual void Render() = 0;


};
typedef CScene* LPSCENE;


