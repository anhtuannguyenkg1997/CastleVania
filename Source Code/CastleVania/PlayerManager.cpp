#include "PlayerManager.h"
#include "PlayScene.h"
#include "SceneManager.h"

#include "CUtils.h"
#include "CStandardized.h"
#include "SubWeaponManager.h"

//=================================== START LOAD HUD FROM FILE ===================================
void PlayerManager::_ParseSection_HP(string line)
{
	CSprites* sprites = CSprites::GetInstance();

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines 

	int id_HP = atoi(tokens[0].c_str());


	//Bao nhiên HP thì lặp bấy nhiêu lần để lấy sprite HP (1 sprite tương ứng với 1 đơn vị HP trong game)
	switch (id_HP)
	{
	case HUD_HP_PLAYER_ID:
		for (int i = 0; i < HP_SIZE; i++)
		{
			string spriteHP = (tokens[1].c_str());
			SimonHP.push_back(sprites->Get(spriteHP));
		}
		break;
	case HUD_HP_PLAYER_LOSE_ID:
		for (int i = 0; i < HP_SIZE; i++)
		{
			string spriteHP = (tokens[1].c_str());
			HPLost.push_back(sprites->Get(spriteHP));
		}
		break;
	case HUD_HP_ENEMY_ID:
		for (int i = 0; i < HP_SIZE; i++)
		{
			string spriteHP = (tokens[1].c_str());
			BossHP.push_back(sprites->Get(spriteHP));
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

	if (tokens.size() < 2) return; // skip invalid lines

	int id_type = atoi(tokens[0].c_str());


	switch (id_type)
	{
	case HUD_FRAME_ID: //Khung vẽ subweapon đang dùng
	{
		string sprite_id = (tokens[1].c_str());

		subWeaponBox = sprites->Get(sprite_id);
		break;
	}
	case HUD_SUBWEAPON_ID: //load toàn bộ sprite chứa subweapon có thể có vào subWeaponList
		for (int i = 1; i < tokens.size(); i += 1)
		{
			string spriteHP = (tokens[i].c_str());

			//Dựa vào index (lấy ra từ biến subWeapon) mà render nó
			subWeaponList.push_back(sprites->Get(spriteHP));
		}
		break;
	case HUD_ITEM_ID: //load toàn bộ sprite chứa item double shot và tripple shot vào itemList
		for (int i = 1; i < tokens.size(); i += 1)
		{
			string spriteHP = (tokens[i].c_str());

			//Dựa vào index (lấy ra từ biến doubleShotOrTrippleShot) mà render nó
			itemList.push_back(sprites->Get(spriteHP));
		}
		break;
	default:
		break;
	}

}


void PlayerManager::_ParseSection_SCORE(string line)
{
	CSprites* sprites = CSprites::GetInstance();

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int id_type = atoi(tokens[0].c_str());

	string nameSprite = tokens[1].c_str();

	//Lấy sprite điểm 
	LPSPRITE spriteScore = sprites->Get(nameSprite);

	//Đưa vào map dựa vào id_type (key). 
	scoreListDisplay[id_type] = spriteScore;
}

//=================================== END LOAD HUD FROM FILE ===================================





PlayerManager* PlayerManager::_instance = NULL;


PlayerManager* PlayerManager::GetInstance()
{
	if (_instance == NULL) _instance = new PlayerManager();
	return _instance;
}

PlayerManager::~PlayerManager()
{
}




void PlayerManager::Update()
{


	//Lấy thể hiện của SceneManager
	SceneManager* scene = SceneManager::GetInstance();


	// Lấy thông tin để update lên HUD
	idScene = scene->GetIDShow();





	// Ép kiểu từ int sang string để viết lên HUD
	string scoreString = to_string(score);
	string timeString = to_string(scene->GetCurrentScene()->GetRemainTime()/1000);
	string subweaponCounterString = to_string(subweaponCounter);
	string liveString = to_string(live);


	//Chuẩn hóa lại các giá trị để viết lên HUD
	StandardizedText(scoreString, timeString, subweaponCounterString, liveString);


	//Truyền các giá trị vào đây để vẽ lên HUD
	DrawingTextHUD(scoreString, timeString, idScene, subweaponCounterString, liveString);


	//Nếu đang hiển thị điểm trên màn hình mà vượt quá thời gian
	if (idScoreShow != -1 && GetTickCount() - timeStartRenderScore > HUD_TIME_SCORE_DISPLAY) {
		
		//Không render nữa
		idScoreShow = -1;
		timeStartRenderScore = -1;
	}

}






void PlayerManager::CalculateScore() {


	int TimeRemain = SceneManager::GetInstance()->GetCurrentScene()->GetRemainTime();

	if (timeCalcScore == -1) timeCalcScore = GetTickCount();

	if (TimeRemain > 0  && GetTickCount() - timeCalcScore > HUD_TIME_SUB_PER_TIMESEC) {
		

		TimeRemain -= 1000;

		score += HUD_SCORE_PER_SEC;

		if (TimeRemain < 0) TimeRemain = 0;

		SceneManager::GetInstance()->GetCurrentScene()->SetRemainTime(TimeRemain);

		SoundManager::GetInstance()->Play(SOUND_SCORE_BY_TIMEREMAIN);


		timeCalcScore = GetTickCount();

	}else if(TimeRemain <= 0 && GetTickCount() - timeCalcScore > HUD_TIME_SUB_PER_SMALLHEART && subweaponCounter > 0){

		subweaponCounter--;

		score += HUD_SCORE_PER_SMALLHEART;

		if (subweaponCounter < 0) subweaponCounter = 0;

		SoundManager::GetInstance()->Play(SOUND_SCORE_BY_SMALLHEART);
	
		timeCalcScore = GetTickCount();
	}

	

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
	subWeaponBox->Draw(OUTSIDE_CAMERA, NOT_FLIPPING, SUBWEAPON_FRAME_X, SUBWEAPON_FRAME_Y);


	//Nếu người chơi chưa ăn đc item có subweapon
	if (subWeapon != -1)
		//Lấy ID subweapon . Nhờ ID này duyệt trong list subWeaponList và vẽ nó lên màn hình
		subWeaponList[subWeapon]->Draw(OUTSIDE_CAMERA, NOT_FLIPPING, SUBWEAPON_INSIDE_X, SUBWEAPON_INSIDE_Y);


	//Nếu người chơi ăn đc item TroubleShot hoặc TrippleShot
	if (doubleShotOrTrippleShot != -1)
		//Lấy ID để xác định xem là doubleshot hay trippleshot để render ra màn hình
		itemList[doubleShotOrTrippleShot]->Draw(OUTSIDE_CAMERA, NOT_FLIPPING, ITEM_HUD_X, ITEM_HUD_Y);


	//Lượng HP mà simon còn
	for (int i = 0; i < simonHP; i++)
		//Vẽ nó lên HUD
		SimonHP[i]->Draw(OUTSIDE_CAMERA, NOT_FLIPPING, HP_PLAYER_POSITION_X + i * HP_PLAYER_POSITION_X_SPACE, HP_PLAYER_POSITION_Y);


	//Lượng HP mà simon đã mất
	for (int i = simonHP; i < HP_SIZE; i++)
		//Vẽ nó lên màn hình
		HPLost[i]->Draw(OUTSIDE_CAMERA, NOT_FLIPPING, HP_PLAYER_POSITION_X + i * HP_PLAYER_POSITION_X_SPACE, HP_PLAYER_POSITION_Y);


	// Boss HP còn
	for (int i = 0; i < bossHP; i++)
		BossHP[i]->Draw(OUTSIDE_CAMERA, NOT_FLIPPING, HP_BOSS_POSITION_X + i * HP_BOSS_POSITION_X_SPACE, HP_BOSS_POSITION_Y);

	// Boss HP mất
	for (int i = bossHP; i < HP_SIZE; i++)
		HPLost[i]->Draw(OUTSIDE_CAMERA, NOT_FLIPPING, HP_BOSS_POSITION_X + i * HP_BOSS_POSITION_X_SPACE, HP_BOSS_POSITION_Y);



	//Hiển thị số điểm khi ăn được túi tiền
	if (idScoreShow!=-1) {
		scoreListDisplay.at(idScoreShow)->Draw(INSIDE_CAMERA, NOT_FLIPPING, positionScoreShow.x, positionScoreShow.y);
	}
	


}







//Text vẽ trên HUD
void PlayerManager::DrawingTextHUD(string score, string timeRemain, string sceneID, string subweaponCounter, string live) {
	info = "SCORE-" + score + " TIME " + timeRemain + " SCENE " + sceneID + "\n";
	info += "PLAYER                  -" + subweaponCounter + "\n";
	info += "ENEMY                   -" + live + "\n";
}



//Khởi tạo
void PlayerManager::Init() {
	score = SIMON_INITIAL_SCORE;								//Điểm mà người chơi ghi được

	doubleShotOrTrippleShot = SIMON_INITIAL_DOUBLE_OR_TRIPLE;	//ID Xác định là doubleshot hay trippleshot

	subweaponCounter = SIMON_INITIAL_SUBWEAPON_COUNTER;			//Số lượng quả tim nhỏ để dùng vũ khí phụ

	live = SIMON_INITIAL_LIVE;									//Số mạng còn lại của Simon

	subWeapon = SIMON_INITIAL_SUBWEAPON_ID;						//ID Vũ khí phụ trong subWeaponList mà người chơi đang sử dụng 

	simonHP = SIMON_INITIAL_HP;									//Máu mà simon còn

	MSLevel = MS_LEVEL1;										//Level của MorningStar

	bossHP = PHANTOMBAT_HP;										//HP của Boss
}




//Reset lại manager khi simon chết
void PlayerManager::ResetData() {

	live--;


	//Hết mạng => reset tất cả giá trị 
	if (live == 0) {
		subweaponCounter = SIMON_INITIAL_SUBWEAPON_COUNTER;
		subWeapon = SIMON_INITIAL_SUBWEAPON_ID;
		simonHP = SIMON_INITIAL_HP;
		MSLevel = MS_LEVEL1;
		doubleShotOrTrippleShot = SIMON_INITIAL_DOUBLE_OR_TRIPLE;
		live = SIMON_INITIAL_LIVE;
		score = SIMON_INITIAL_SCORE;
		bossHP = PHANTOMBAT_HP;
	}
	//Còn mạng => reset một số giá trị
	else {
		subweaponCounter = SIMON_INITIAL_SUBWEAPON_COUNTER;
		subWeapon = SIMON_INITIAL_SUBWEAPON_ID;
		simonHP = SIMON_INITIAL_HP;
		MSLevel = MS_LEVEL1;
		doubleShotOrTrippleShot = SIMON_INITIAL_DOUBLE_OR_TRIPLE;
		bossHP = PHANTOMBAT_HP;
	}


}



//Load sprite HUD
void PlayerManager::Load(LPCWSTR gameFile)
{
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
		if (line == "[SCORE]") { section = HUD_SCORE; continue; }
		//
		// data section
		//
		switch (section)
		{
		case HUD_TEXTURE: _ParseSection_TEXTURES(line); break;
		case HUD_SPRITE: _ParseSection_SPRITES(line); break;
		case HUD_HP: _ParseSection_HP(line); break;
		case HUD_ITEM: _ParseSection_ITEM(line); break;
		case HUD_SCORE: _ParseSection_SCORE(line); break;
		}
	}
	f.close();


}




//Clear HUD
void PlayerManager::Clear() {

	for (int i = 0; i < SimonHP.size(); i++) {
		SimonHP[i] = NULL;
		delete SimonHP[i];
	}

	SimonHP.clear();


	for (int i = 0; i < BossHP.size(); i++) {
		BossHP[i] = NULL;
		delete BossHP[i];
	}

	BossHP.clear();



	for (int i = 0; i < HPLost.size(); i++) {
		HPLost[i] = NULL;
		delete HPLost[i];
	}

	HPLost.clear();

	subWeaponBox = NULL;				
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