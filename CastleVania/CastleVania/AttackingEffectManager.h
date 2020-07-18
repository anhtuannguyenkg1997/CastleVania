#pragma once
#include "AttackingEffect.h"

class AttackingEffectManager
{
	//Thể hiện duy nhất của AttackingEffectManager
	static AttackingEffectManager* _instance;

	//Danh sách effect đang có
	vector<AttackingEffect*> listEffect;
public:
	//Lấy thể hiện duy nhất của AttackingEffectManager
	static AttackingEffectManager* GetInstance();

	void Update();

	void Render();


	//Lấy vị trí render effect (các va chạm dùng sweptAABB của Subweapon)
	D3DXVECTOR2 GetPositionEffectSweptAABB(int nx, int ny, float left, float top, float right, float bottom);

	//Lấy vị trí render effect (các va chạm dùng BroadPhaseTest của MorningStar)
	D3DXVECTOR2 GetPositionEffectBroadPhaseTest(int nx, float leftOther, float rightOther, float topObject);


	//Thêm một effect vào list
	void AddListEffect(AttackingEffect* effect) { listEffect.push_back(effect); }

	//Xóa một effect trong list
	void DeleteEffect(AttackingEffect* effect);
};

