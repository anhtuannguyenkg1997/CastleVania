#include <iostream>
#include <fstream>

#include "SceneManager.h"
#include "CAnimations.h"
#include "CTextures.h"
#include "CRandomManager.h"
#include "CDebug.h"
#include "CUtils.h"
#include "CGame.h"
#include "PlayScene.h"
#include "PlayerManager.h"

#include "define.h"


//INIT
SceneManager* SceneManager::_instance = NULL;

//GETTER
SceneManager* SceneManager::GetInstance()
{
	if (_instance == NULL) _instance = new SceneManager();
	return _instance;
}


//===========================START LOAD DATA FROM FILE===========================
void SceneManager::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

void SceneManager::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; //skip invalid line


	int id = atoi(tokens[0].c_str());	//id Scene
	string idShow = tokens[1];
	int timePlay = atoi(tokens[2].c_str());



	LPSCENE scene = new PlayScene(id, idShow, timePlay);
	scenes[id] = scene;

}


void SceneManager::_ParseSection_DATAPATH(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; //skip invalid line


	int id = atoi(tokens[0].c_str());	//id Scene



	LPCWSTR sceneObjectPath = ToLPCWSTR(tokens[1]);
	LPCWSTR sceneMapTexturePath = ToLPCWSTR(tokens[2]);
	LPCWSTR sceneMapDataPath = ToLPCWSTR(tokens[3]);


	scenes[id]->SetDataPath(sceneObjectPath, sceneMapTexturePath, sceneMapDataPath);

}



void SceneManager::_ParseSection_CAMERA(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; //skip invalid line


	int id = atoi(tokens[0].c_str());	//id Scene
	float cameraPosX = atof(tokens[1].c_str());
	float cameraPosY = atof(tokens[2].c_str());
	int mapWidth = atoi(tokens[3].c_str());
	int mapHeight = atoi(tokens[4].c_str());

	D3DXVECTOR2 camPosition = D3DXVECTOR2(cameraPosX, cameraPosY);

	scenes[id]->SetCameraPositionMapSize(camPosition, mapWidth, mapHeight);

}


void SceneManager::_ParseSection_SIMON(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; //skip invalid line


	int id = atoi(tokens[0].c_str());	//id Scene
	float simonPosX = atof(tokens[1].c_str());
	float simonPosY = atof(tokens[2].c_str());
	int stateSimon = atoi(tokens[3].c_str());
	int orientationSimon = atoi(tokens[4].c_str());

	D3DXVECTOR2 simonPositionStart = D3DXVECTOR2(simonPosX, simonPosY);

	scenes[id]->SetSimonPositionStart(simonPositionStart);
	scenes[id]->SetSimonState(stateSimon);
	scenes[id]->SetSimonOrientation(orientationSimon);
}



void SceneManager::Load(LPCWSTR gameFile)
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

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }
		if (line == "[DATAPATH]") { section = GAME_FILE_SECTION_DATAPATH; continue; }
		if (line == "[CAMERA_MAPSIZE]") { section = GAME_FILE_SECTION_CAMERA; continue; }
		if (line == "[SIMON]") { section = GAME_FILE_SECTION_SIMON; continue; }

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		case GAME_FILE_SECTION_DATAPATH: _ParseSection_DATAPATH(line); break;
		case GAME_FILE_SECTION_CAMERA: _ParseSection_CAMERA(line); break;
		case GAME_FILE_SECTION_SIMON: _ParseSection_SIMON(line); break;
		}
	}
	f.close();



	
	//Load âm thanh cho game
	SoundManager::GetInstance()->LoadResourceSound(L"Sounds\\SoundData.txt");

	SwitchScene(current_scene);


	//Khởi tạo giá trị của simon mà playerManager sẽ quản lý VD: Live, HP, MorningStar Level, Subweapon, ......
	PlayerManager::GetInstance()->Init();


	//Khởi tạo các giá trị random
	CRandomManager::GetInstance()->Init();


	//DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

}


void SceneManager::SwitchScene(int toScene)
{

	//DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	//clear all data
	if (before_scene_show!="") {
		scenes[current_scene]->Unload();

		CTextures::GetInstance()->Clear();
		CSprites::GetInstance()->Clear();
		CAnimations::GetInstance()->Clear();
		CAnimationSets::GetInstance()->Clear();
	}


	

	//switch to scene id
	current_scene = toScene;
	LPSCENE s = scenes[toScene];
	CGame::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());



	if (before_scene_show == s->GetIDShow()) {
		if (!timeBeforeScene > 5000000) {
			s->SetTimeToPlay(timeBeforeScene);
			timeBeforeScene = 0;
		}
	}
	else {
		SoundManager::GetInstance()->StopAll();
		SoundManager::GetInstance()->Play(current_scene);
	}

	
	//load màn chơi
	s->Load();

}

//===========================END LOAD DATA FROM FILE===========================





//===========================START UPDATE ===========================

void SceneManager::Update(DWORD dt) {
	scenes[current_scene]->Update(dt);
	
}

//===========================END UPDATE ===========================




//===========================START RENDER ===========================
void SceneManager::Render() {
	scenes[current_scene]->Render();
}
//===========================END RENDER ===========================






