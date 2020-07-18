#pragma once
#include <Windows.h>
#include <vector>
#include "CGameObject.h"
#include "CObjectCell.h"



class EnemyRevival
{
	static EnemyRevival* _instance;
	vector<CObjectCell*> listObjectRevival;			//Danh sách object sẽ được hồi sinh
public:

	static EnemyRevival* GetInstance();


	void Update();

	//Thêm vào Danh sách object sẽ được hồi sinh
	void AddToListRevival(CObjectCell* obj) { listObjectRevival.push_back(obj); }

	void Clear();
};

