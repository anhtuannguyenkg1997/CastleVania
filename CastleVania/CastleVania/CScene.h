#pragma once

#include <d3dx9.h>
#include "CKeyEventHandler.h"
#include <string>

using namespace std;

class CScene
{
protected:
	CKeyEventHandler* key_handler; //Bàn phím dành cho màn chơi
	int id; //Mã của màn chơi
	string IDShow; //Mã hiển thị trên màn hình chính

	LPCWSTR objectSceneFilePath; //Đường dẫn đến file chứa các object của màn chơi
	LPCWSTR mapTexturePath; //Đường dẫn đến Texture của map (tile map)
	LPCWSTR mapDataPath;	//Đường dẫn đến Data của map (tọa độ từng tile map trên thế giới game)
	
	D3DXVECTOR2 cameraPosition; //Tọa độ của Camera trong màn này

	D3DXVECTOR2 simonPositionStart; //Tọa độ của Simon khi bắt đầu màn chơi

	int mapWidth, mapHeight; //Kích thước map của scene này

	int simonOrientation; //Hướng của Simon khi bắt đầu màn chơi
	int simonState;			//Trạng thái của simon khi bắt đầu màn chơi

	DWORD timeToPlay;							//Thời gian còn lại của màn chơi
	DWORD remainTime;							//Thời gian còn lại (Vẽ lên HUD)

public:
	//Khởi tạo màn chơi 
	CScene(int id, string IDShow, DWORD timetoPlay);

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


	//SETTER
	void SetDataPath(LPCWSTR objectSceneFilePath, LPCWSTR mapTexturePath, LPCWSTR mapDataPath) {
		this->objectSceneFilePath = objectSceneFilePath;
		this->mapTexturePath = mapTexturePath;
		this->mapDataPath = mapDataPath;
	};

	void SetCameraPositionMapSize(D3DXVECTOR2 cameraPosition, int mapWidth, int mapHeight) 
	{
		this->cameraPosition = cameraPosition;
		this->mapWidth = mapWidth;
		this->mapHeight = mapHeight;
	}

	//GETTER

	void SetSimonPositionStart(D3DXVECTOR2 simonPositionStart) {this->simonPositionStart = simonPositionStart;}
	void SetSimonState(int simonState) { this->simonState = simonState; }
	void SetSimonOrientation(int simonOrientation) { this->simonOrientation = simonOrientation; }

	//DWORD getTimeToPlay() { return this->timeToPlay; }
	void SetRemainTime(DWORD remainTime) {this->remainTime = remainTime;}
	void SetTimeToPlay(DWORD timeToPlay) { this->timeToPlay = timeToPlay; }
	DWORD GetRemainTime() { return this->remainTime; }
	string GetIDShow() { return this->IDShow; }
};
typedef CScene* LPSCENE;


