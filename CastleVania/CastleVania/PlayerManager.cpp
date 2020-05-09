#include "PlayerManager.h"
#include "PlayScene.h"
#include "SceneManager.h"

#include "CUtils.h"
#include "CStandardized.h"



PlayerManager* PlayerManager::_instance = NULL;


PlayerManager* PlayerManager::GetInstance()
{
	if (_instance == NULL) _instance = new PlayerManager();
	return _instance;
}

PlayerManager::~PlayerManager()
{
}




void PlayerManager::Update(DWORD dt, Simon* simon)
{
	//Lấy thể hiện của SceneManager
	SceneManager* scene = SceneManager::GetInstance();


	// Lấy thông tin để update lên HUD
	score = simon->GetScore();
	subweaponCounter = simon->GetsubweaponCounter();
	live = simon->GetLive();
	subWeapon = simon->GetSubWeapon();
	idScene = scene->GetIDScene();
	simonHP = simon->GetHP();
	doubleShotOrTrippleShot = simon->GetDoubleShotOrTrippleShot();


	//Cộng dồn thời gian lại sau mổi lần update=> thời gian từ lúc bắt đầu màn chơi
	time += dt;

	//Tính thời gian còn lại
	//time / CLOCKS_PER_SEC => giây
	int remainTime = DEFAULT_TIME_PLAY - time / CLOCKS_PER_SEC;


	// Ép kiểu từ int sang string
	string scoreString = to_string(score);
	string timeString = to_string(remainTime);
	string sceneString = to_string(idScene);
	string subweaponCounterString = to_string(subweaponCounter);
	string liveString = to_string(live);


	//Chuẩn hóa lại các giá trị để viết lên HUD
	StandardizedText(scoreString, timeString, sceneString, subweaponCounterString, liveString);


	//Truyền các giá trị vào đây để vẽ lên HUD
	DrawingTextHUD(scoreString, timeString, sceneString, subweaponCounterString, liveString);
}





void PlayerManager::Render()
{

	CGame* game = CGame::GetInstance();

	//vùng chữ nhật mà ta sẽ vẽ toàn bộ HUD vào đây
	RECT rect;

	//Lấy kích thước của HUD để vẽ lên màn hình
	SetRect(&rect, HUD_LEFT, HUD_TOP, HUD_RIGHT, HUD_BOTTOM);


	//Lấy font chữ và vẽ lên màn hình
	if (game->GetFont() != NULL)
		game->GetFont()->DrawTextA(NULL, info.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));


	//Vẽ khung Subweapon
	subWeaponBox->Draw(INDEPENDENT_ON_CAMERA, NOT_FLIPPING, SUBWEAPON_FRAME_X, SUBWEAPON_FRAME_Y);


	//Nếu người chơi chưa ăn đc item có subweapon
	if (subWeapon != -1)
		//Lấy ID subweapon . Nhờ ID này duyệt trong list subWeaponList và vẽ nó lên màn hình
		subWeaponList[subWeapon]->Draw(INDEPENDENT_ON_CAMERA, NOT_FLIPPING, SUBWEAPON_INSIDE_X, SUBWEAPON_INSIDE_Y);


	//Nếu người chơi ăn đc item TroubleShot hoặc TrippleShot
	if (doubleShotOrTrippleShot != -1)
		//Lấy ID để xác định xem là doubleshot hay trippleshot để render ra màn hình
		itemList[doubleShotOrTrippleShot]->Draw(INDEPENDENT_ON_CAMERA, NOT_FLIPPING, ITEM_HUD_X, ITEM_HUD_Y);


	//Lượng HP mà simon còn
	for (int i = 0; i < simonHP; i++)
		//Vẽ nó lên HUD
		playerHP[i]->Draw(INDEPENDENT_ON_CAMERA, NOT_FLIPPING, HP_PLAYER_POSITION_X + i * HP_PLAYER_POSITION_X_SPACE, HP_PLAYER_POSITION_Y);


	//Lượng HP mà simon đã mất
	for (int i = simonHP; i < HP_SIZE; i++)
		//Vẽ nó lên màn hình
		PlayerHPLost[i]->Draw(INDEPENDENT_ON_CAMERA, NOT_FLIPPING, HP_PLAYER_POSITION_X + i * HP_PLAYER_POSITION_X_SPACE, HP_PLAYER_POSITION_Y);

}



//=================================== LOAD HUD FROM FILE ===================================
void PlayerManager::_ParseSection_HP(string line)
{
	CSprites* sprites = CSprites::GetInstance();

	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int id_HP = atoi(tokens[0].c_str());


	switch (id_HP)
	{
	case HUD_HP_PLAYER_ID:
		for (int i = 0; i < HP_SIZE; i++)
		{
			string spriteHP = (tokens[1].c_str());

			playerHP.push_back(sprites->Get(spriteHP));	//List bao nhiêu phần tử thì bấy nhiêu HP Simon đc render lên màn hình
		}
		break;
	case HUD_HP_PLAYER_LOSE_ID:
		for (int i = 0; i < HP_SIZE; i++)
		{
			string spriteHP = (tokens[1].c_str());

			PlayerHPLost.push_back(sprites->Get(spriteHP));	//List bao nhiêu phần tử thì bấy nhiêu HP mất đi đc render lên màn hình
		}
		break;
	default:
		break;
	}

}


void PlayerManager::_ParseSection_ITEM(string line)
{
	CSprites* sprites = CSprites::GetInstance();

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	int id_type = atoi(tokens[0].c_str());


	switch (id_type)
	{
	case HUD_FRAME_ID:
	{
		string sprite_id = (tokens[1].c_str());

		// Tạo khung để vẽ subweapon vào đó
		subWeaponBox = sprites->Get(sprite_id);
		break;
	}
	case HUD_SUBWEAPON_ID:
		for (int i = 1; i < tokens.size(); i += 1)
		{
			string spriteHP = (tokens[i].c_str());

			//Danh sách subweapon mà người chơi có thể nhận được. Khi người chơi nhận đc weapon nào
			//Ta sẽ dựa vào index (tương ứng với ID item) mà render nó lên khung weapon
			subWeaponList.push_back(sprites->Get(spriteHP));
		}
		break;
	case HUD_ITEM_ID:
		for (int i = 1; i < tokens.size(); i += 1)
		{
			string spriteHP = (tokens[i].c_str());

			//Danh sách doubleshot và trippleshot mà người chơi có thể nhận được. 
			//Khi người chơi nhận đc item nào. Ta sẽ dựa vào index (tương ứng với biến doubleShotOrTrippleShot ở class Simon) mà render nó lên màn hình
			itemList.push_back(sprites->Get(spriteHP));
		}
		break;
	default:
		break;
	}

}



//Text vẽ trên HUD
void PlayerManager::DrawingTextHUD(string score, string timeRemain, string sceneID, string subweaponCounter, string live) {
	info = "SCORE-" + score + " TIME " + timeRemain + " SCENE " + sceneID + "\n";
	info += "PLAYER                  -" + subweaponCounter + "\n";
	info += "                        -" + live + "\n";
}




void PlayerManager::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURE]") { section = HUD_TEXTURE; continue; }
		if (line == "[SPRITE]") { section = HUD_SPRITE; continue; }
		if (line == "[HP]") { section = HUD_HP; continue; }
		if (line == "[ITEM]") { section = HUD_ITEM; continue; }
		//
		// data section
		//
		switch (section)
		{
		case HUD_TEXTURE: _ParseSection_TEXTURES(line); break;
		case HUD_SPRITE: _ParseSection_SPRITES(line); break;
		case HUD_HP: _ParseSection_HP(line); break;
		case HUD_ITEM: _ParseSection_ITEM(line); break;
		}
	}
	f.close();




	//Khởi tạo thời gian đã trôi qua = 0
	time = 0;
	
	//Khởi tạo textHUD
	DrawingTextHUD("000000", "0000", "00", "00", "00");

	//DebugOut(L"[INFO] Loading HUD successfully\n", gameFile);
}




//Clear HUD
void PlayerManager::Clear() {

	for (int i = 0; i < playerHP.size(); i++) {
		playerHP[i] = NULL;
		delete playerHP[i];
	}

	playerHP.clear();





	for (int i = 0; i < PlayerHPLost.size(); i++) {
		PlayerHPLost[i] = NULL;
		delete PlayerHPLost[i];
	}

	PlayerHPLost.clear();

	subWeaponBox = NULL;				//Khung mà ta sẽ vẽ Subweapon vào đó
	delete subWeaponBox;



	for (int i = 0; i < subWeaponList.size(); i++) {
		subWeaponList[i] = NULL;
		delete subWeaponList[i];
	}

	subWeaponList.clear();


	for (int i = 0; i < itemList.size(); i++) {
		itemList[i] = NULL;
		delete itemList[i];
	}

	itemList.clear();
}