#pragma once
#include <Windows.h>
#include <vector>
#include "CGameObject.h"
#include "CObjectCell.h"



class EnemyRevival
{
	static EnemyRevival* _instance;
	vector<CObjectCell*> listObjectRevival;
public:

	static EnemyRevival* GetInstance();


	void Update();

	void AddToListRevival(CObjectCell* obj) { listObjectRevival.push_back(obj); }

	void Clear();
};

