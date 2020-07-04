#pragma once

#include "CGame.h"
#include "Simon.h"
#include "PlayScene.h"
#include "CScenceKeyHandler.h"
#include "PlaySceneKeyForwardAction.h"



class PlayScenceKeyHandler : public CScenceKeyHandler
{
	PlaySceneKeyForwardAction* forwardAction = PlaySceneKeyForwardAction::GetInstance();
	CGame* game = CGame::GetInstance();
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};


	PlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

