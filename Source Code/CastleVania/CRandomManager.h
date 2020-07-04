#pragma once
#include <stdio.h>      
#include <stdlib.h>    

#include "CRandomGroup.h"


using namespace std;

class CRandomManager
{
	vector<CRandomGroup*> listRandomGroup;
	static CRandomManager* _instance;

public:
	bool isInit = false;

	static CRandomManager* GetInstance();

	void Init();

	void AddGroup(CRandomGroup* group) { this->listRandomGroup.push_back(group); }
	
	void EditRangeInList(int idListRandom, int idValue, int Range);

	int Random(int idListGroup);

	void UpdateRandomMS();


};

