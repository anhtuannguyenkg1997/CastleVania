#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Simon.h"
#include "Enemy.h"


class CCamera
{
	//Thể hiện duy nhất của Camera
	static CCamera* _instance;

	//Vị trí camera 
	D3DXVECTOR2		position;		

public:

	//Lấy thể hiện duy nhất của camera
	static CCamera* GetInstance();

	//Lấy vị trí của camera
	D3DXVECTOR2 GetCameraPositon() { return this->position; }


	//Cập nhật camera theo vị trí của Simon
	void Update(Simon *simon);

	//Thiết đặt vị trí của camera
	void SetCameraPosition(D3DXVECTOR2 position);

	//Kiểm tra Object có nằm trong vùng Camera không
	bool checkObjectInCamera(LPGAMEOBJECT object);

	//Kiểm tra Vị trí ban đầu của Enemy có nằm trong vùng Viewport hay không
	bool checkObjectInCameraRevival(Enemy* enemy);
};

