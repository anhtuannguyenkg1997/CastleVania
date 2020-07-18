#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "CSprite.h"


using namespace std;


//Quản lý các sprites trong toàn bộ game
class CSprites
{
	//Thể hiện duy nhất của Csprites
	static CSprites* _instance;


	//Danh sách từng sprite theo id của nó
	unordered_map<string, LPSPRITE> sprites;		

public:
	//Thêm một sprite mới vào danh sách sprites . ID là duy nhất để phân biệt các sprite với nhau
	//vì ta quản lý tất cả các sprite trong một mảng nên phải phân biệt các sprite qua ID của nó
	void Add(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);


	//Lấy ra sprite dựa vào ID của sprite đó
	LPSPRITE Get(string id) { return sprites[id]; }

	//Làm sạch tất cả texture đã load
	void CSprites::Clear();


	//Lấy ra thể hiện duy nhất của CSprites
	static CSprites* GetInstance();
};

