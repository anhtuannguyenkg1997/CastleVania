#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

//Quản lý từng sprite
//Được cắt ra từ texture
class CSprite
{
	//Mã Srpite (tất cả sprite sẽ nằm cùng trong 1 mảng sprites, vì vậy các id sprite này không được trùng nhau)
	//Chính giá trị này sẽ giúp ta truy xuất đến sprite để render nó ra 
	string id;			


	//Một Sprite cần lấy trong Texture là một hình chữ nhật được xác định bằng 4 tham số này
	//left: từ cạnh trái texture đến cạnh trái sprite
	//top: từ cạnh trên texture đến cạnh trên sprite
	//right: từ cạnh phải texture đến cạnh phải sprite
	//left: từ cạnh dưới texture đến cạnh dưới sprite
	int left, top, right, bottom;


	//Texture mà ta cần cắt để tạo sprite
	LPDIRECT3DTEXTURE9 texture;

public:
	//Tạo một sprite mới
	CSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);


	//Vẽ sprite lên màn hình
	//accordingCam: có vẽ theo camera hay không????
	//nx: hướng của sprite
	void Draw(int isCameraMoving, int direction, float x, float y, int alpha = 255);
};

typedef CSprite* LPSPRITE;

