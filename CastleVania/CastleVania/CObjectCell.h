#pragma once
#include "CGameObject.h"


class CObjectCell
{

	LPGAMEOBJECT obj;		//Object nằm trong cell

	//Tọa độ của objectcell (trong thé giới game)
	float x;
	float y;

public:
	//Khởi tạo cell
	CObjectCell(LPGAMEOBJECT obj, float x, float y);


	//Update theo tọa độ của CGameObject
	//Tức là object game mà CObject này đang nắm giữ
	void Update(float x, float y);



	//Lấy object nằm trong CObjectCell naỳ
	LPGAMEOBJECT GetObj() { return this->obj; }


	//Getter và Setter
	float GetX() { return x; }
	float GetY() { return y; }
	void SetX(float x) { this->x = x; }
	void SetY(float y) { this->y = y; }
};

