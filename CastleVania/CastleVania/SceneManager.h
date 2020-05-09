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


	//LOAD DATA FROM FILE
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);


public:
	//GETTER
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	static SceneManager* GetInstance();
	int GetIDScene() { return current_scene; }


	//LOAD MANAGER SCENE FROM FILE
	void Load(LPCWSTR gameFile);
	void SwitchScene(int scene_id);



	//UPDATE
	void Update(DWORD dt);



	//RENDER
	void Render();


};

