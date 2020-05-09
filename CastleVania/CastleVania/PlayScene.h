 #pragma once
#include "CScene.h"
#include "CGame.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTileMap.h"

#include "Simon.h"
#include "MorningStar.h"
#include "Torch.h"
#include "Item.h"
#include "Brick.h"
#include "Portal.h"
#include "PlayerManager.h"
#include "CGrid.h"
#include "Solider.h"
#include "LoadFileHelper.h"
#include "Candle.h"

#include <map>
using namespace std;

class PlayScene : public CScene
{
private:
	int IDScene;

	CGame* game = CGame::GetInstance();
	CGrid* grid = CGrid::GetInstance();
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	CTileMap* tilemap = CTileMap::GetInstance();
	CCamera* camera = CCamera::GetInstance();
	PlayerManager* playerManager = PlayerManager::GetInstance();

	CObjectCell* objectCell;



	Simon* simon;
	MorningStar* morningStar;
	Brick* brick;
	Torch* torch;
	Candle* candle;
	Portal* portal;
	Solider* solider;

	vector<CObjectCell*> listObjectCells;

	vector<LPGAMEOBJECT> listObjects;		//List object xét va chạm
	vector<LPGAMEOBJECT> listObjectsRender;	//List object render. được lấy từ listObject

protected:

	//CREATE OBJECTS FROM FILE
	void _ParseSection_OBJECTS(string line);


public:

	//LOAD DATA FROM FILE
	virtual void LoadFromFile(LPCWSTR filePath);	//Đọc File
	virtual void Load();							//Load toàn bộ scene
	

	//INIT
	PlayScene(int id, LPCWSTR sceneFilePath, LPCWSTR mapTexturePath, LPCWSTR mapDataPath, D3DXVECTOR2 camPosition,D3DXVECTOR2 simonPosStart, int mapWidth, int mapHeight);
	void CreateObject(int object_type, CGameObject*& obj, D3DXVECTOR2 position, int state, int idItem);


	//UPDATE
	void Update(DWORD dt);
	void GetCollisionObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects);


	//RENDER
	void Render();


	//DESTRUCTOR
	virtual void Unload();




	//GETTER & SETTER
	int GetIDScene() { return this->IDScene; }
	Simon* GetSimon() { return this->simon; }
	PlayScene* GetPlayScene() { return this; }



};




