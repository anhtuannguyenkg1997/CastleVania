#pragma once
#include <Windows.h>
#include <vector>
#include "CRandomItem.h"

using namespace std;

class CRandomGroup
{
	vector<CRandomItem*> listRandomItem;
public:
	int totalValue = 0;

	void AddRandomItem(CRandomItem* randomItem) { listRandomItem.push_back(randomItem); }

	void SetRangeRandom();

	int Random();


	void EditRangeRandomItem(int value, int Range);
	
	CRandomItem* GetRandomItem(int index) { return this->listRandomItem[index]; }

	int GetSize() { return this->listRandomItem.size(); }
};

