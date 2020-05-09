#pragma once

#include "CGame.h"
#include "Simon.h"
#include "PlayScene.h"
#include "CScenceKeyHandler.h"




class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};

	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};


	void Simon_Hit();
	void Simon_Hit_SubWeapon();
};

