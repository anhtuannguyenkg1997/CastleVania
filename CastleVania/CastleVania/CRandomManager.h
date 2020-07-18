#pragma once
#include <stdio.h>      
#include <stdlib.h>    

#include "CRandomGroup.h"


using namespace std;

class CRandomManager
{
	vector<CRandomGroup*> listRandomGroup;									//Tập hợp nhóm Random của toàn bộ game
	static CRandomManager* _instance;

public:
	bool isInit = false;													//Chỉ được khởi tạo 1 lần trong toàn bộ thời gian sống của Game

	static CRandomManager* GetInstance();

	void Init();															//Khởi tạo các giá trị random (vùng giá trị random)

	void AddGroup(CRandomGroup* group) { this->listRandomGroup.push_back(group); } //Thêm 1 nhóm Random mới
	
	void EditRangeInList(int idListRandom, int idValue, int Range);			//Chỉnh sửa lại tỉ lệ trong 1 nhóm random

	int Random(int idListGroup);											//Truyền vào ID nhóm Random và trả về giá trị của Item trong nhóm random đó

	void UpdateRandomMS();													//Cập nhật lại tỉ lệ random ra Item khi update MorningStar

};

