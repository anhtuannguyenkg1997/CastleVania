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
	CGame* game;													//Dùng để lấy công cụ vẽ

	static PlayerManager* _instance;								//Thể hiện duy nhất của PlayerManager

	string info;													//Chuỗi Text hiển thị trên HUD

	vector<LPSPRITE> SimonHP;										//Lượng HP Sprite mà simon có (size = 16)
	vector<LPSPRITE> HPLost;										//Lượng HP Sprite  mà Simon mất (size = 16)
	vector<LPSPRITE> BossHP;										//Lượng HP Sprite mà Boss có (size = 16)
	vector<LPSPRITE> subWeaponList;									//Danh sách Sprite vũ khí mà simon có thể sử dụng
	vector<LPSPRITE> itemList;										//Doubleshot và trippleshot
	unordered_map<int, LPSPRITE> scoreListDisplay;					//map sprite score có thể nhận được

	LPSPRITE subWeaponBox;											//Sprite Khung mà ta sẽ vẽ Subweapon vào đó

	
	

	//Giá trị lưu cho Simon
	int score = SIMON_INITIAL_SCORE;								//Điểm mà người chơi ghi được
	string idScene;													//ID của Màn chơi hiện tại
	int doubleShotOrTrippleShot = SIMON_INITIAL_DOUBLE_OR_TRIPLE;	//ID Xác định là doubleshot hay trippleshot
	int subweaponCounter = SIMON_INITIAL_SUBWEAPON_COUNTER;			//Số lượng quả tim nhỏ để dùng vũ khí phụ
	int live = SIMON_INITIAL_LIVE;									//Số mạng còn lại của Simon
	int subWeapon = SIMON_INITIAL_SUBWEAPON_ID;						//ID Vũ khí phụ trong subWeaponList mà người chơi đang sử dụng 
	int simonHP = SIMON_INITIAL_HP;									//Máu mà simon còn
	int bossHP = PHANTOMBAT_HP;														//Máu mà Boss còn
	int MSLevel = MS_LEVEL1;										//Level của MorningStar

	

	//LOAD DATA FROM FILE
	void _ParseSection_HP(string line);
	void _ParseSection_ITEM(string line);
	void _ParseSection_SCORE(string line);


	//Cấu hình chữ viết trên HUD
	void DrawingTextHUD(string score, string time, string sceneID, string enemy, string life);

public:

	//Lấy thể hiện duy nhất của PlayerManager
	static PlayerManager* GetInstance();


	int idScoreShow = -1;														//Điểm hiển thị lên màn hình khi ăn MoneyBag, Chest hoặc Diadiem. Xác định scoreListDisplay
	D3DXVECTOR2 positionScoreShow;												//Tọa độ render score 
	DWORD timeStartRenderScore = -1;											//Thời gian bắt đầu render Score


	bool isFightingBoss = false;												//Có đang đấu boss hay không
	bool isFinishGame = false;													//Đánh thắng boss
	DWORD timeFinishGame;														//Thời gian  từ khi ăn MagicCrystal. Để tính thời gian phát nhạc chiến thắng
	DWORD timeCalcScore = -1;													//Temp time để tính từng timeRemain và SmallHeart còn lại



	//Khởi tạo giá trị cho Simon
	void Init();



	//Load toàn bộ HUD
	void Load(LPCWSTR gameFile);

	//Update HUD
	void Update();


	//Tính điểm sau khi đánh bại boss
	void CalculateScore();

	//Khi simon chết sẽ reset lại giá trị trong Manager
	void ResetData();

	//Vẽ HUD
	void Render();

	//Làm sạch HUD
	void Clear();





	//GETTER

	int GetScore() { return this->score; }

	int GetDoubleShotOrTrippleShot() { return this->doubleShotOrTrippleShot; }

	int GetSubweaponCounter() { return this->subweaponCounter; }

	int GetLive() { return this->live; }

	int GetSubweaponID() { return this->subWeapon; }

	int GetSimonHP() { return this->simonHP; }

	int GetMSLevel() { return this->MSLevel; }

	bool GetIsFightingBoss() { return this->isFightingBoss; }

	int GetBossHp() { return this->bossHP; }


	//SETTER

	void SetScore(int score) { this->score = score; }

	void SetDoubleShotOrTrippleShot(int doubleShotOrTrippleShot) { this->doubleShotOrTrippleShot = doubleShotOrTrippleShot; }

	void SetSubweaponCounter(int subweaponCounter) { this->subweaponCounter = subweaponCounter; }

	void SetLive(int live) { this->live = live; }

	void SetSubweaponID(int subWeapon) { this->subWeapon = subWeapon; }

	void SetSimonHP(int simonHP) { this->simonHP= simonHP; }

	void SetMSLevel(int MSLevel) { this->MSLevel = MSLevel; }

	void SetBossHP(int HP) { this->bossHP = HP; }

	void SetIsFightingBoss(bool isFightingBoss) { this->isFightingBoss = isFightingBoss; }



	~PlayerManager();
};

