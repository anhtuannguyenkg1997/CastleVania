#pragma once
#include <Windows.h>
#include "CAnimations.h"
#include "define.h"

class AttackingEffect
{
public:
	//Vị trí render effect
	D3DXVECTOR2 position;				
	
	//Thời gian bắt đầu render effect
	DWORD timeStartEffect;				

	//Animation set của Attacking Effect
	LPANIMATION_SET animation_set;		

	//Constructor
	AttackingEffect(D3DXVECTOR2 position); 


	//Đã vượt qua thời gian dành cho effect hay chưa???
	bool isFinish() { return GetTickCount() - timeStartEffect > ATTACK_EFFECT_TIME; }


	void Render();
};




