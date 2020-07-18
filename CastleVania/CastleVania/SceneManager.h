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
	unordered_map<int, LPSCENE> scenes;								//ID màn chơi và con trỏ đến màn chơi tương ứng (LIST)
	int current_scene;												//ID màn chơi hiện tại
	string before_scene_show = "";									//Stage của màn chơi (2 scene có thể cùng 1 stage)
																	//Dùng biến này sẽ xử lí được thời gian & sound khi chuyển tiếp giữa 2 scene trong 1 stage
	DWORD timeBeforeScene = 0;										//Thời gian của màn chơi trước (lưu để có thể dùng cho màn sau nếu cùng 1 stage)

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

