#include <iostream>
#include <fstream>

#include "define.h"

#include "PlayScene.h"
#include "CUtils.h"
#include "CTextures.h"
#include "CSprites.h"
#include "CAnimations.h"
#include "CAnimation.h"
#include "CPlayScenceKeyHandler.h"


using namespace std;



//===================BEGIN SECTION LOAD DATA FROM FILE===================
void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;


	//Tạo các Object thông thường
	if (object_type != OBJECT_TYPE_PORTAL) {

		D3DXVECTOR2 positionStart = D3DXVECTOR2(x, y);

		int state = atof(tokens[4].c_str());
		//bool active = atof(tokens[5].c_str());
		int idItem = atof(tokens[6].c_str());


		CreateObject(object_type, obj, positionStart, state, idItem);
	}
	//Tạo Portal để chuyển cảnh
	else if (object_type == OBJECT_TYPE_PORTAL) {
		float right = atof(tokens[4].c_str());
		float bottom = atof(tokens[5].c_str());
		int sceneTarget = atoi(tokens[6].c_str());

		obj = new Portal(x, y, right, bottom, sceneTarget);
		portal = (Portal*)obj;

		portal->Active(true);
		objectCell = new CObjectCell(portal, x, y);
	}

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);

}





//Đọc File chứa [Texture, sprites, animation, animation_set, object] 
void PlayScene::LoadFromFile(LPCWSTR filePath)
{
	//DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
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
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

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



	//Load những object riêng cho từng màn chơi(Brick,Torch....)
	grid->InitGrid(mapWidth, CAMERA_HEIGHT);

	LoadFromFile(objectSceneFilePath);


	//Khởi tạo map & camera
	camera->SetCameraPosition(cameraPosition);

	tilemap->LoadMap(mapTexturePath, mapDataPath, mapWidth, mapHeight);

}


//===================END SECTION LOAD DATA FROM FILE===================









//=================== BEGIN PLAY SCENE INIT ===================

PlayScene::PlayScene(int id, LPCWSTR sceneFilePath, LPCWSTR mapTexturePath, LPCWSTR mapDataPath, D3DXVECTOR2 camPosition, D3DXVECTOR2 simonPosStart, int mapWidth, int mapHeight) :CScene(id, sceneFilePath, mapTexturePath, mapDataPath, camPosition, simonPosStart, mapWidth, mapHeight)
{

	key_handler = new CPlayScenceKeyHandler(this);


}




void PlayScene::CreateObject(int object_type, CGameObject*& obj, D3DXVECTOR2 positionStart, int state, int idItem) {

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		obj = new Simon();
		simon = (Simon*)obj;
		simon->SetPosition(simonPositionStart.x, simonPositionStart.y);
		simon->SetState(STANDING);
		break;
	case OBJECT_TYPE_MORNINGSTAR:
		obj = new MorningStar();
		morningStar = (MorningStar*)obj;
		morningStar->SetState(MS_LEVEL1);
		break;
	case OBJECT_TYPE_TORCH:
		obj = new Torch();
		torch = (Torch*)obj;
		torch->SetPosition(positionStart.x, positionStart.y);
		torch->SetState(state);
		torch->Active(true);
		torch->SetIDItem(idItem);
		objectCell = new CObjectCell(torch, positionStart.x, positionStart.y);
		break;
	case OBJECT_TYPE_SOLIDER:
		obj = new Solider();
		solider = (Solider*)obj;
		solider->SetPosition(positionStart.x, positionStart.y);
		solider->SetState(0); //INACTIVE
		solider->Active(true);
		solider->SetIDItem(idItem);
		objectCell = new CObjectCell(solider, positionStart.x, positionStart.y);
		break;
	case OBJECT_TYPE_CANDLE:
		obj = new Candle();
		candle = (Candle*)obj;
		candle->SetPosition(positionStart.x, positionStart.y);
		candle->SetState(state);
		candle->Active(true);
		candle->SetIDItem(idItem);
		objectCell = new CObjectCell(candle, positionStart.x, positionStart.y);
		break;
	case OBJECT_TYPE_GROUND:
		obj = new Brick();
		brick = (Brick*)obj;
		brick->SetPosition(positionStart.x, positionStart.y);
		brick->SetState(state);
		brick->Active(true);
		objectCell = new CObjectCell(brick, positionStart.x, positionStart.y);
		break;


	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
}


//=================== END PLAY SCENE INIT ===================








//=================== BEGIN PLAY SCENE UPDATE ===================
void PlayScene::Update(DWORD dt)
{
	//GET OBJECTS IN GRID & GRID UPDATE
	// Lấy danh sách object từ grid để update và render
	//Chọn những object trong vùng camera => đưa vào các danh sách object đã được phân loại giúp dễ quản lý
	grid->GetListObject(listObjectCells, listObjects, listObjectsRender);
	grid->Update(listObjectCells);

	//SIMON UPDATE
	//Nếu simon không bằng NULL
	if (simon != NULL) {
		//Xử lý va chạm giữa simon và các object có thể va chạm 
		simon->Update(dt, listObjects);
	}

	//MORNINGSTAR UPDATE
	if (morningStar != NULL) {
		morningStar->Update(dt, simon, listObjects);
	}



	//OTHERS OBJECT UPDATE
	//Duyệt qua tất cả Object
	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];
		vector<LPGAMEOBJECT> coObjects;

		//Lấy danh sách vật có thể va chạm với object hiện tại . Đưa nó vào list coObjects
		GetCollisionObjects(object, coObjects);

		//Xử lí va chạm giữa object này với coObjects
		object->Update(dt, &coObjects);
	}


	//CAMERA, WEAPON AND  HUD UPDATE
	// update camera
	if (simon != NULL) {
		camera->Update(simon);	//Cập nhật vị trí của camera
		playerManager->Update(dt, simon);	//Cập nhật HUD

	}




	//SETTING ITEM FALL DOWN
	//Set để item rơi xuống khi simon tiêu diệt mục tiêu phải thõa đồng thời 3 điều kiện
	//1. Object phải bị tiêu diệt (active= false)
	//2. Object bị tiêu diệt có khả năng tạo ra item (VD: torch, candle,..) (idItem khác -1)
	//3.  Object bị tiêu diệt phải chưa bị rớt Item ra (isDroppedItem = false)
	for (UINT i = 0; i < listObjects.size(); i++)
	{
		if (!listObjects[i]->isActive() && listObjects[i]->idItem != -1 && !listObjects[i]->isDroppedItem) {

			//Tạo một Item mới
			Item* item = new Item(listObjects[i]);

			//Đưa item này vào listItem 
			listObjects.push_back(item);


			//Biến tạm lưu vị trí của item 
			float x, y;

			//Lấy vị trí của item 
			item->GetPosition(x, y);

			//Đưa item này vào Grid
			objectCell = new CObjectCell(item, x, y);

		}
	}







}







//Lấy danh sách vật có thể va chạm 
//Vật cần xét va chạm curObj
//Danh sách vật va chạm trả về coObjects
void PlayScene::GetCollisionObjects(LPGAMEOBJECT object, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Item*>(object))
	{
		for (auto obj : listObjects)
		{
			if (dynamic_cast<Brick*>(obj))
				coObjects.push_back(obj);
		}
	}


}







//=================== END PLAY SCENE UPDATE ===================








//=================== START PLAY SCENE RENDER ===================

void PlayScene::Render()
{

	//Render HUD
	playerManager->Render();


	//Render Map
	tilemap->Render();


	//Render list Render Object
	for (auto obj : listObjectsRender)
	{
		//Kiểm tra object có trong vùng camera không
		if (camera->checkObjectInCamera(obj)) {
			obj->Render();
		}
	}

	//DebugOut(L"Size:%d \n", listObjectsRender.size());


	//render Simon
	simon->Render();

	////Render MorningStar
	////Nếu Simon đang đánh thì mới vẽ MorningStar
	//if (simon->IsHit() == true)
	//	//Render Morningstar dựa vào animation của simon. 
	//	//Tức là sprite hành động của simon thế nào thì sprite của morningstar render đến đó
	if (simon->GetState() == WHOPPING_SITTING || simon->GetState() == WHOPPING_STANDING)
		morningStar->Render(simon->animation_set->at(simon->GetState())->GetCurrentFrame());


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


	//Xóa list Object Render
	for (int i = 0; i < listObjectsRender.size(); i++) {
		listObjectsRender[i] = NULL;
		delete listObjectsRender[i];
	}
	listObjectsRender.clear();





	//Xóa list ObjectCell
	for (int i = 0; i < listObjectCells.size(); i++) {
		listObjectCells[i] = NULL;
		delete listObjectCells[i];
	}

	listObjectCells.clear();


	//Xóa List Object
	listObjects.clear();



	//NULL pointer Simon and MorningStar
	simon = NULL;
	morningStar = NULL;


	//DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

//=================== START PLAY SCENE DESTRUCTOR ===================






























