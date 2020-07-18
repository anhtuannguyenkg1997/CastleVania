#pragma once
#include "Simon.h"
#include "MorningStar.h"
#include "Torch.h"
#include "Brick.h"
#include "Portal.h"
#include "CGrid.h"
#include "BlackKnight.h"
#include "Candle.h"
#include "VampireBat.h"
#include "Stair.h"
#include "MovingStair.h"
#include "Ghost.h"
#include "HunchBack.h"
#include "WhiteSkeleton.h"
#include "Zombie.h"
#include "Raven.h"
#include "PhantomBat.h"


class ObjectCreater
{
	static ObjectCreater* _instance;

	CObjectCell* objectCell;

	Stair* stair;
	MovingStair* movingStair;
	Brick* brick;
	Torch* torch;
	Candle* candle;
	Portal* portal;
	BlackKnight* blackKnight;
	VampireBat* vampireBat;
	HunchBack* hunchBack;
	Ghost* ghost;
	WhiteSkeleton* whiteSkeleton;
	Raven* raven;
	Zombie* zombie;
	PhantomBat* phantomBat;

public:
	static ObjectCreater* GetInstance();


	//MS và Simon là 2 object đã tạo ở PlayScene, Đưa vào hàm này chỉ đế gắn thêm thuộc tính mà thôi
	void CreateObject(vector<string> tokens,Simon* &simon,MorningStar* &morningStar, D3DXVECTOR2 simonPositionStart, int simonState, int simonOrientation);
	

	//Tạo object đơn giản như brick, torch,....
	void CreateSimpleObject(int object_type, CGameObject*& obj, D3DXVECTOR2 position, int idItem, int objectType, int colCell, int rowCell);
};

