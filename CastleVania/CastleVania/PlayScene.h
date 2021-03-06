﻿ #pragma once
#include "CScene.h"
#include "CGame.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTileMap.h"
#include "EnemyRevival.h"
#include "Simon.h"
#include "MorningStar.h"
#include "Torch.h"
#include "ItemManager.h"
#include "Brick.h"
#include "Portal.h"
#include "PlayerManager.h"
#include "CGrid.h"
#include "BlackKnight.h"
#include "LoadFileHelper.h"
#include "Candle.h"
#include "VampireBat.h"
#include "Stair.h"
#include "MovingStair.h"
#include "StairManager.h"
#include "CheatingStair.h"
#include "WhiteSkeleton.h"
#include "Bone.h"
#include "Raven.h"
#include "Zombie.h"
#include "SubWeapon.h"
#include "SubWeaponManager.h"
#include "RosaryItemEffect.h"
#include "ObjectCreater.h"
#include "SoundManager.h"
#include "HiddenObjectManager.h"
#include "AttackingEffectManager.h"
#include <map>
using namespace std;




class PlayScene : public CScene
{

	CGame* game = CGame::GetInstance();
	CGrid* grid = CGrid::GetInstance();
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	CTileMap* tilemap = CTileMap::GetInstance();
	CCamera* camera = CCamera::GetInstance();
	PlayerManager* playerManager = PlayerManager::GetInstance();
	SubWeaponManager* subWeaponManager = SubWeaponManager::GetInstance();
	ItemManager* itemManager = ItemManager::GetInstance();
	StairManager* stairManager = StairManager::GetInstance();
	CheatingStair* cheatStair = CheatingStair::GetInstance();
	RosaryItemEffect* rosaryEffect = RosaryItemEffect::GetInstance();
	SoundManager* soundManager = SoundManager::GetInstance();
	HiddenObjectManager* hiddenObjectManager = HiddenObjectManager::GetInstance();
	ObjectCreater* objectCreater = ObjectCreater::GetInstance();
	EnemyRevival* enemyRivival = EnemyRevival::GetInstance();
	AttackingEffectManager* attackEffectManager = AttackingEffectManager::GetInstance();


	Simon* simon;
	MorningStar* morningStar;


	vector<LPGAMEOBJECT> listObjects;		//List object xét va chạm, update state,...
	vector<CObjectCell*> listObjectCells;	//List object Phân hoạch không gian. Tham số obj lấy từ listObjects[i] tương ứng

	DWORD timePassed;						//Thời gian đã trôi qua từ khi bắt đầu màn chơi

protected:

	//CREATE OBJECTS FROM FILE
	void _ParseSection_OBJECTS(string line);


public:

	//LOAD DATA FROM FILE
	virtual void LoadFromFile(LPCWSTR filePath);	//Đọc File liên quan đến Sprite, Animation,...
	virtual void Load();							//Load toàn bộ scene (HUD, Map)
	

	//INIT
	PlayScene(int id, string idShow, DWORD timeToPlay);


	//UPDATE
	void Update(DWORD dt);

	//Trạng thái của màn chơi (sound, fightingboss,...)
	void UpdateStatePlayScene(DWORD dt);


	//RENDER
	void Render();


	//DESTRUCTOR
	virtual void Unload();


	//GETTER & SETTER
	Simon* GetSimon() { return this->simon; }
	MorningStar* GetMorningStar() { return this->morningStar; }

};




