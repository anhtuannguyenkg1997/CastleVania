#pragma once


class CRandomItem
{
	int Value;
	int Range;

public:
	int beginRan, endRan;

	CRandomItem() { Value = 0; Range = 0; }

	CRandomItem(int Value, int Range);


	int GetValue() { return this->Value; }
	int GetRange() { return this->Range; }

	void SetRange(int Range) { this->Range = Range; }

};

