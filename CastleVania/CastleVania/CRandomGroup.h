#pragma once
#include <Windows.h>
#include <vector>
#include "CRandomItem.h"

using namespace std;

class CRandomGroup
{
	vector<CRandomItem*> listRandomItem;			//Một nhóm random quản lí các random Item. Ví dụ: 1 phần tử trong list quản lí việc random ra Item thường.
public:
	int totalValue = 0;								//Tổng giá trị vùng random của toàn bộ Item trong list. 


	void AddRandomItem(CRandomItem* randomItem) { listRandomItem.push_back(randomItem); } //Thêm 1 Item mới vào Group

	void SetRangeRandom();							//Set vùng random của các Item dựa vào tỉ lệ xuất hiện của chúng, cập nhật totalValue

	int Random();									//Trả về 1 value của Item bất kì


	void EditRangeRandomItem(int value, int Range); //Đổi tỉ lệ của những Item nằm bên trong
	
	CRandomItem* GetRandomItem(int index) { return this->listRandomItem[index]; } //Con trõ trỏ

	int GetSize() { return this->listRandomItem.size(); } //Lấy số lượng loại random trong game
};

