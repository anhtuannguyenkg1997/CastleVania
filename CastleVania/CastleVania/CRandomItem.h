#pragma once


class CRandomItem
{
	int Value;								//Giá trị của Item (VD: Item MorningStar có giá trị là 200, ...)
	int Range;								//Tỉ lệ % xuất hiện Item này, Ví dụ 5%

public:
	int beginRan, endRan;					//Vùng xác định của Item là 1 số nguyên (VD: từ 1...5 là dành cho Item MorningStar)

	CRandomItem() { Value = 0; Range = 0; } //Constructor 

	CRandomItem(int Value, int Range);		//Constructor


	//GETTER
	int GetValue() { return this->Value; }
	int GetRange() { return this->Range; }


	//SETTER
	void SetRange(int Range) { this->Range = Range; }

};

