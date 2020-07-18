#include "CScene.h"

//Hàm khởi tạo Scene với ID và đường dẫn đến file map
CScene::CScene(int id, string idShow, DWORD timeToPlay)
{
	this->id = id;
	this->IDShow = idShow;
	this->timeToPlay = timeToPlay;
}