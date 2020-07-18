#pragma once

#include <Windows.h>
#include <unordered_map>

#include <d3d9.h>
#include <d3dx9.h>

#include "CDebug.h"
#include "CGame.h"

using namespace std;

//Quản lý danh sách textures của toàn bộ game
class CTextures
{
	//Thể hiện duy nhất của texture
	static CTextures* _instance;


	//danh sách textures
	//ID của texture và Texture đó
	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:

	//Thêm một texture vào danh sách textures. Truyền vào ID và đường dẫn đến file ảnh texture
	//LPCWSTR là một con trỏ 32 bit đến một chuỗi ký tự Unicode 16 bit không đổi
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor = D3DCOLOR_XRGB(255, 0, 255));


	//Lấy texture dựa vào ID của nó
	LPDIRECT3DTEXTURE9 Get(int id) { return textures[id]; }


	//Làm sạch danh sách textures
	void Clear();


	//Lấy thể hiện duy nhất của texture
	static CTextures* GetInstance();
};
