#pragma once

#include "SceneManager.h"

#include <vector>
#include <string>

#include "Simon.h"
#include "LoadFileHelper.h"
#include "define.h"

using namespace std;

class PlayerManager
{
private:
	CGame* game;						//Dùng để lấy công cụ vẽ
	Simon* simon;						//Lấy thông tin của Simon

	static PlayerManager* _instance;	//Thể hiện duy nhất của PlayerManager

	string info;					//Text hiển thị trên HUD

	vector<LPSPRITE> playerHP;			//Lượng HP Sprite mà simon có (size = 16)
	vector<LPSPRITE> PlayerHPLost;		//Lượng HP Sprite  mà Simon mất (size = 16)

	LPSPRITE subWeaponBox;				//Sprite Khung mà ta sẽ vẽ Subweapon vào đó

	vector<LPSPRITE> subWeaponList;		//Danh sách Sprite vũ khí mà simon có thể sử dụng
	vector<LPSPRITE> itemList;			//Doubleshot và trippleshot Sprite (size = 2)


	int score;		//Điểm mà người chơi ghi được

	int time;		//Thời gian đã trôi qua kể từ lúc bắt đầu màn chơi

	int idScene;	//ID của Màn chơi hiện tại

	int doubleShotOrTrippleShot;	//ID Xác định là doubleshot hay trippleshot

	int subweaponCounter;		//Số lượng quả tim nhỏ để dùng vũ khí phụ

	int live;		//Số mạng còn lại của Simon

	int subWeapon;	//ID Vũ khí phụ trong subWeaponList mà người chơi đang sử dụng 

	int simonHP;	//Máu mà simon còn



	//LOAD DATA FROM FILE
	void _ParseSection_HP(string line);
	void _ParseSection_ITEM(string line);

	//Cấu hình chữ viết trên HUD
	void DrawingTextHUD(string score, string time, string sceneID, string enemy, string life);

public:
	//Lấy thể hiện duy nhất của PlayerManager
	static PlayerManager* GetInstance();

	~PlayerManager();

	//Load toàn bộ HUD
	void Load(LPCWSTR gameFile);

	//Update HUD
	void Update(DWORD dt, Simon* simon);

	//Vẽ HUD
	void Render();

	//Làm sạch HUD
	void Clear();
};

