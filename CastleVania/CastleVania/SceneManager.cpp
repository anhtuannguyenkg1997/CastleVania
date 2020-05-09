#include <iostream>
#include <fstream>

#include "SceneManager.h"
#include "CAnimations.h"
#include "CTextures.h"

#include "CDebug.h"
#include "CUtils.h"
#include "CGame.h"
#include "PlayScene.h"


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

	if (tokens.size() < 8) return; //skip invalid line


	int id = atoi(tokens[0].c_str());	//id Scene
	LPCWSTR sceneObjectPath = ToLPCWSTR(tokens[1]);
	LPCWSTR sceneMapTexturePath = ToLPCWSTR(tokens[2]);
	LPCWSTR sceneMapDataPath = ToLPCWSTR(tokens[3]);
	float cameraPosX = atof(tokens[4].c_str());
	float cameraPosY = atof(tokens[5].c_str());
	int mapWidth = atoi(tokens[6].c_str());
	int mapHeight = atoi(tokens[7].c_str());
	float simonPosX = atof(tokens[8].c_str());
	float simonPosY = atof(tokens[9].c_str());

	D3DXVECTOR2 camPosition = D3DXVECTOR2(cameraPosX, cameraPosY);
	D3DXVECTOR2 simonPositionStart = D3DXVECTOR2(simonPosX, simonPosY);

	LPSCENE scene = new PlayScene(id, sceneObjectPath, sceneMapTexturePath, sceneMapDataPath, camPosition, simonPositionStart, mapWidth, mapHeight);
	scenes[id] = scene;

	//SetGameState(id);
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

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene(current_scene);
}


void SceneManager::SwitchScene(int scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	//clear all data
	scenes[current_scene]->Unload();

	CTextures::GetInstance()->Clear();
	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();
	CAnimationSets::GetInstance()->Clear();

	//switch to scene id
	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	CGame::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());


	//load màn chơi
	scenes[scene_id]->Load();

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






