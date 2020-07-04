#pragma once
#include <Windows.h>
#include <unordered_map>


#include "CScene.h"
#include "CGame.h"
#include "CTileMap.h"

using namespace std;

class SceneManager
{
	static SceneManager* _instance;
	unordered_map<int, LPSCENE> scenes;
	int current_scene;
	string before_scene_show = ""; 
	
	DWORD timeBeforeScene = 0;

	//LOAD DATA FROM FILE
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
	void _ParseSection_DATAPATH(string line);
	void _ParseSection_CAMERA(string line);
	void _ParseSection_SIMON(string line);

public:
	//GETTER
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	static SceneManager* GetInstance();
	string GetIDShow() { return scenes[current_scene]->GetIDShow(); }
	int GetCurrentSceneID() { return this->current_scene; }
	void SetBeforeScene(string sceneShow) { this->before_scene_show = sceneShow; }
	void SetTimeBeforeScene(DWORD time) { timeBeforeScene = time; }

	//LOAD MANAGER SCENE FROM FILE
	void Load(LPCWSTR gameFile);
	void SwitchScene(int toSceneID);



	//UPDATE
	void Update(DWORD dt);



	//RENDER
	void Render();


};

