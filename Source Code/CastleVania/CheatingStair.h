#pragma once
#include <Windows.h>
#include "Simon.h"

class CheatingStair
{
	static CheatingStair* _instance;
	bool ableToCheat = true; //Mỗi màn chơi chỉ cheat một lần (nếu cho phép)
public:
	static CheatingStair* GetInstance();


	//Kiểm tra màn chơi có cheat stair không, nếu có thì cheat
	void CheckForCheat(int idScene, Simon *simon);


	//Set lại trạng thái cheat (khi simon chết)
	void SetAbleToCheat(bool ableToCheat) { this->ableToCheat = ableToCheat; }
};

