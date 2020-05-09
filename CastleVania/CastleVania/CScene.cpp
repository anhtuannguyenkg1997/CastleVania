#include "CScene.h"

//Hàm khởi tạo Scene với ID và đường dẫn đến file map
CScene::CScene(int id, LPCWSTR objectSceneFilePath, LPCWSTR mapTexturePath, LPCWSTR mapDataPath, D3DXVECTOR2 cameraPosition, D3DXVECTOR2 simonPositionStart, int mapWidth, int mapHeight)
{
	this->id = id;
	this->objectSceneFilePath = objectSceneFilePath;
	this->mapTexturePath = mapTexturePath;
	this->mapDataPath = mapDataPath;
	this->cameraPosition = cameraPosition;
	this->simonPositionStart = simonPositionStart;
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;

}