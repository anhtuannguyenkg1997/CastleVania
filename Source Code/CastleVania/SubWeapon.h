#pragma once
#include "CGameObject.h"
#include "Sound.h"

#include "Simon.h"
#include "Torch.h"
#include "Candle.h"
#include "BlackKnight.h"
#include "Brick.h"
#include "Torch.h"
#include "VampireBat.h"
#include "Ghost.h"
#include "HunchBack.h"
#include "WhiteSkeleton.h"
#include "Raven.h"
#include "Zombie.h"
#include "PhantomBat.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "AttackingEffectManager.h"

class SubWeapon : public CGameObject
{
protected:
	int damage;			//Sát thương mà Subweapon gây ra

	AttackingEffect* effect;

	AttackingEffectManager* aEManager = AttackingEffectManager::GetInstance();
public:
	SubWeapon(Simon* simon);
	~SubWeapon() {};


	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon);

	virtual void Render();


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)=0;


	//Lấy những object có khả năng va chạm với simon từ listObjects và đưa vào danh sách coObjects
	void GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon);

};

