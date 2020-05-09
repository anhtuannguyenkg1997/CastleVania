#include "CSprite.h"
#include "CGame.h"


//Tạo một sprite mới
CSprite::CSprite(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}


//Vẽ sprite lên màn hình
void CSprite::Draw(int dependOnCam, int direction, float x, float y, int alpha)
{
	//Lấy thể hiện duy nhất của CGame (để vẽ)
	CGame* game = CGame::GetInstance();

	//Gọi đến hàm vẽ của lớp CGame
	game->Draw(dependOnCam, direction, x, y, texture, left, top, right, bottom, alpha);
}

