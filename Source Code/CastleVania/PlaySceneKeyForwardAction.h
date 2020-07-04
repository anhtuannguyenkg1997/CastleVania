#pragma once
#include "CGame.h"
#include "Simon.h"
#include "SubWeaponManager.h"
#include "StairManager.h"
#include "PlayScene.h"

class PlaySceneKeyForwardAction
{
	static PlaySceneKeyForwardAction* _instance;

	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();
	StairManager* stairManager = StairManager::GetInstance();

public:
	static PlaySceneKeyForwardAction* GetInstance();


	bool canAccessKeyDownAction(Simon *simon);
	bool canAccessKeyStateAction(Simon* simon);

	void KeyLeftAction(Simon* simon);
	void KeyRightAction(Simon* simon);
	void KeyDownAction(Simon* simon);
	void KeyUpAction(Simon* simon);
	void NotPressKeyAction(Simon* simon);


};

