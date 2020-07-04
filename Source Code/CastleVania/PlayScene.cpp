#include <iostream>
#include <fstream>

#include "define.h"

#include "PlayScene.h"
#include "CUtils.h"
#include "CTextures.h"
#include "CSprites.h"
#include "CAnimations.h"
#include "CAnimation.h"


#include "PlayScenceKeyHandler.h"


using namespace std;



//===================BEGIN SECTION LOAD DATA FROM FILE===================
void PlayScene::_ParseSection_OBJECTS(string line)
{

	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	objectCreater->CreateObject(tokens, simon, morningStar, resetBySimonDead, simonPositionStart, simonState, simonOrientation);

}



//Đọc File chứa [Texture, sprites, animation, animation_set, object] 
void PlayScene::LoadFromFile(LPCWSTR filePath)
{
	//DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	char str[MAX_GAME_LINE];
	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = GAME_FILE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

}



//Load toàn bộ scene (object, map, init )
void PlayScene::Load() {
	//Load những object chung cho toàn bộ game(Simon, MorningStar,....)
	LoadFromFile(GENERAL_OBJECT_FILEPATH);

	//Khởi tạo HUD
	playerManager->Load(HUD_FILEPATH);


	//Khởi tạo grid
	grid->InitGrid(mapWidth, CAMERA_HEIGHT);

	//Load những object riêng cho từng màn chơi(Brick,Torch....)
	LoadFromFile(objectSceneFilePath);


	//Khởi tạo map & camera
	camera->SetCameraPosition(cameraPosition);

	//Load map
	tilemap->LoadMap(mapTexturePath, mapDataPath, mapWidth, mapHeight);



}


//===================END SECTION LOAD DATA FROM FILE===================









//=================== BEGIN PLAY SCENE INIT ===================

PlayScene::PlayScene(int id, string idShow, DWORD timeToPlay) :CScene(id, idShow, timeToPlay)
{

	key_handler = new PlayScenceKeyHandler(this);

	timePassed = 0;		//Khởi tạo thời gian đã trôi qua của màn chơi = 0
}







//=================== END PLAY SCENE INIT ===================








//=================== BEGIN PLAY SCENE UPDATE ===================
void PlayScene::Update(DWORD dt)
{

	//GET OBJECTS IN GRID & GRID UPDATE
	// Lấy danh sách object từ grid để update và render
	//Chọn những object trong vùng camera
	grid->GetListObject(listObjectCells, listObjects);


	grid->Update(listObjectCells);


	stairManager->LoadListStair(&listObjects);




	//SIMON UPDATE
	//Nếu simon không bằng NULL
	if (simon != NULL && simon->GetState() != DEAD) {

		//Xử lý va chạm giữa simon và các object đang có 
		simon->Update(dt, listObjects);

		//Xử lí việc simon dùng subWeapon
		subWeaponManager->Update(simon, dt);

		cheatStair->CheckForCheat(id, simon);
		
		hiddenObjectManager->Update();
	}

	enemyRivival->Update();


	//Nếu đang dùng Rosary & thời gian Rosary còn tác dụng 
	if (rosaryEffect->isRosaryEffect && rosaryEffect->IsRosary()) {

		//Duyệt qua tất cả GameObject
		for (UINT i = 0; i < listObjects.size(); i++) {

			//Tìm đến class loại Enemy
			if (dynamic_cast<Enemy*>(listObjects[i])) {

				//Phá hủy nó
				listObjects[i]->SetState(ENEMY_DESTROYED);

				//Tạo ra Item
				listObjects[i]->setIsDisappearByWeapn(true);
			}

		}

	}
	else {

		//Tắt trạng thái dùng Rosary
		rosaryEffect->isRosaryEffect = false;


		//OTHERS OBJECT UPDATE
		//Duyệt qua tất cả GameObject
		for (UINT i = 0; i < listObjects.size(); i++)
		{
			//Kiểm tra xem object có phải là Subweapon hay không
			if (dynamic_cast<SubWeapon*>(listObjects[i])) {
				SubWeapon* subweapon = dynamic_cast<SubWeapon*>(listObjects[i]);
				subweapon->Update(dt, listObjects, simon);
			}
			//Kiểm tra xem object có phải là Item hay không
			else if (dynamic_cast<Item*>(listObjects[i])) {
				Item* item = dynamic_cast<Item*>(listObjects[i]);
				item->Update(dt, listObjects, simon);
			}
			//Kiểm tra xem object có phải là Enemy hay không
			else if (dynamic_cast<Enemy*>(listObjects[i])) {
				Enemy* enemy = dynamic_cast<Enemy*>(listObjects[i]);
				enemy->Update(dt, listObjects, simon);
			}
			//Kiểm tra xem object có phải là Bone( vũ khí của WhiteSkeleton) hay không
			else if (dynamic_cast<Bone*>(listObjects[i])) {
				Bone* bone = dynamic_cast<Bone*>(listObjects[i]);
				bone->Update(dt, listObjects, simon);
			}
			else if (dynamic_cast<MovingStair*>(listObjects[i])) {
				MovingStair* mvStair = dynamic_cast<MovingStair*>(listObjects[i]);
				mvStair->Update(dt, listObjects, simon);
			}
			//Các object còn lại (Torch, Candle, Portal, Stair, Moving Stair)
			else {
				listObjects[i]->Update(dt, listObjects);
			}

		}

	}




	//MORNINGSTAR UPDATE
	if (morningStar != NULL) {
		//Update MS theo Simon
		morningStar->Update(dt, simon, listObjects);


		//Quản lý việc tạo ra Item
		itemManager->Update(listObjects);
	}


	//CAMERA AND  STATE GAME UPDATE
	if (simon != NULL) {
		//Cập nhật vị trí của camera
		camera->Update(simon);

		//Kiểm tra state màn chơi
		UpdateStatePlayScene(dt);
	}




	//Cập nhật HUD
	playerManager->Update();

	//Cập nhật effect khi Weapon của simon đánh trúng mục tiêu
	attackEffectManager->Update();

	//Show số lượng object có trong từng Cell ở trong vùng camera
	//grid->ShowSizeCells();
}



void PlayScene::UpdateStatePlayScene(DWORD dt) {

	if (simon->GetState() == DEAD) {

		//Dừng tất cả sound đang phát
		SoundManager::GetInstance()->StopAll();

		//Phát Sound Simon Dead
		SoundManager::GetInstance()->Play(SOUND_SIMON_DEAD);
	}
	//Nếu đang đấu với Boss
	if (playerManager->GetIsFightingBoss()) {

		//Dừng phát Sound của màn chơi hiện tại
		SoundManager::GetInstance()->Stop(id);

		//Phát Sound màn đánh boss
		SoundManager::GetInstance()->Play(SOUND_FIGHTING_BOSS);
	}
	//Thời gian từ lúc ăn MagicCrystal đến hiện tại > 5s
	//Do khi ăn MagicCrystal xong thì phải chờ 5s để phát nhạc kết thúc màn chơi (sound trong vòng 5s)
	else if (playerManager->isFinishGame && GetTickCount() - playerManager->timeFinishGame > HUD_TIME_PLAY_SOUND_FINISH_GAME) {

		//Tính điểm cho người chơi
		PlayerManager::GetInstance()->CalculateScore();
	}
	//Nếu đánh thắng Boss và Ăn MagicCrystal
	else if (PlayerManager::GetInstance()->isFinishGame) {
		//Dừng phát Sound đấu Boss
		SoundManager::GetInstance()->Stop(SOUND_FIGHTING_BOSS);

		//Phát Sound FinishGame
		SoundManager::GetInstance()->Play(SOUND_FINISH_GAME);
	}


	//Nếu ăn MagicCrystal thì không cho phép simon di chuyển
	if (PlayerManager::GetInstance()->isFinishGame) {
		simon->SetState(STANDING);
		simon->SetVX(0.0f);
	}



	//Nếu simon chưa dead và chưa ăn MagicCrystal thì mới kiểm tra TimetoPlay
	if (simon->GetState() != DEAD && !PlayerManager::GetInstance()->isFinishGame) {
		//Không dùng stop watch thì mới tính thời gian
		if (!subWeaponManager->isUseStopWatch) {
			//Cộng dồn thời gian lại sau mổi lần update=> thời gian từ lúc bắt đầu màn chơi
			timePassed += dt;
		}

		//Tính thời gian còn lại
		remainTime = timeToPlay - timePassed;

		//Hết thời gian dành cho màn chơi
		if (remainTime == 0) {
			simon->SetState(DEAD);
			timePassed = 0;			//Set lại biến này, nếu không time sẽ lấy từ lúc bắt đầu load scene này => sai
			remainTime = 0;
		}


	}


	if (!PlayerManager::GetInstance()->isFinishGame) {
		//Update lại state màn chơi khi thỏa 1 trong hai điều kiện
		//1. Simon chết trong camera và thời gian chờ render state Dead đã hết (hết HP hoặc hết timeToPlay)
		//2. Simon chết ngoài vùng camera (rơi ra khỏi camera , lọt hố)
		if (simon->GetState() == DEAD && GetTickCount() - simon->timeDied > SIMON_DEAD_TIME) {

			//Hết mạng => trở về màn 1 và reset lại tất cả data game
			if (playerManager->GetLive() == 1) {

				SceneManager* sceneManager = SceneManager::GetInstance();
				//Dừng phát nhạc để phát lại màn 1
				SoundManager::GetInstance()->StopAll();


				sceneManager->Load(MAIN_FILEPATH);
			}
			//Còn mạng =>reload lại màn chơi
			else {
				playerManager->ResetData();
				timePassed = 0;					//Bắt đầu đếm lại thời gian chơi
				resetBySimonDead = true;		//Đặt cờ reload lại màn chơi
				cheatStair->SetAbleToCheat(true); //Bật cờ để simon đứng trên thang khi bắt đầu màn chơi (3 &5)
				SoundManager::GetInstance()->Play(id);
				Unload();
				Load();							//Load lại màn chơi này

			}
		}


	}







}




//=================== END PLAY SCENE UPDATE ===================








//=================== START PLAY SCENE RENDER ===================

void PlayScene::Render()
{




	//Render Map
	tilemap->Render();



	//Render list Render Object
	for (auto obj : listObjects)
	{
		obj->Render();

	}


	//render Simon
	simon->Render();


	////Render MorningStar
	////Nếu Simon đang đánh thì mới vẽ MorningStar
	//Render Morningstar dựa vào animation của simon. 
	//Tức là sprite hành động của simon thế nào thì sprite của morningstar render đến đó
	if (subWeaponManager->isHitBySubweapon == false)
	{
		if (simon->GetState() == WHOPPING_SITTING || simon->GetState() == WHOPPING_STANDING || simon->GetState() == WALKING_DOWN_STAIR_WHOPPING || simon->GetState() == WALKING_UP_STAIR_WHOPPING || simon->GetState() == 13) {
			morningStar->Render(simon->animation_set->at(simon->GetState())->GetCurrentFrame());
		}
		else
			morningStar->Render(-1);
	}

	//Render HUD
	playerManager->Render();

	attackEffectManager->Render();
}


//=================== END PLAY SCENE RENDER ===================







//=================== START PLAY SCENE DESTRUCTOR ===================
void PlayScene::Unload()
{
	//Xóa Grid
	grid->Clear();

	//Xóa TileMap
	tilemap->Clear();

	//Làm sạch HUD
	playerManager->Clear();


	//Sẵn sàng cheat stair cho màn 3 và 5
	cheatStair->SetAbleToCheat(true);

	//Xóa list ObjectCell
	for (int i = 0; i < listObjectCells.size(); i++) {
		listObjectCells[i] = NULL;
		delete listObjectCells[i];
	}

	listObjectCells.clear();

	//Xóa List Object
	for (int i = 0; i < listObjects.size(); i++) {
		if (listObjects[i] != NULL) {
			listObjects[i] = NULL;
			delete listObjects[i];
		}
	}

	listObjects.clear();

	SceneManager::GetInstance()->SetTimeBeforeScene(remainTime);


	//Set lại các biến quản lí một hidden item về ban đầu để chuẩn bị cho hidden object ở màn sau
	HiddenObjectManager::GetInstance()->Reset();


	//NULL pointer Simon and MorningStar
	simon = NULL;
	morningStar = NULL;


	//DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

//=================== START PLAY SCENE DESTRUCTOR ===================




























