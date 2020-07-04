#pragma once
#include "Item.h"

//All Items
#include "MorningStarItem.h"
#include "SmallHeartItem.h"
#include "BigHeartItem.h"
#include "MoneyBagRedItem.h"
#include "MoneyBagPurpleItem.h"
#include "MoneyBagWhiteItem.h"
#include "RosaryItem.h"
#include "InvisibilityPotionItem.h"
#include "DoubleShotItem.h"
#include "TripleShotItem.h"
#include "PorkChopItem.h"
#include "MoneyBagItem.h"
#include "ChestItem.h"
#include "DiademItem.h"
#include "DaggerItem.h"
#include "AxeItem.h"
#include "HolyWaterItem.h"
#include "BoomerangItem.h"
#include "StopWatchItem.h"
#include "MagicCrystalItem.h"

//Object tạo Item
#include "Torch.h"
#include "Candle.h"
#include "BlackKnight.h"
#include "VampireBat.h"
#include "Ghost.h"
#include "HunchBack.h"
#include "WhiteSkeleton.h"
#include "PhantomBat.h"
#include "Raven.h"
#include "Zombie.h"

//
#include "MorningStar.h"
#include "CObjectCell.h"
#include "SubWeaponManager.h"

class ItemManager
{
	static ItemManager* _instance;			//Thể hiện duy nhất của ItemManager
public:

	static ItemManager* GetInstance();		//Lấy thể hiện duy nhất của ItemManager

	Item* CreateItem(LPGAMEOBJECT object);	//Tạo 1 Item dựa vào 1 Object đưa vào

	int RandomItem();						//Random  1 Item bất kì


	//Liên tục Update theo PlayScene
	void Update(vector<LPGAMEOBJECT> listObjects);
};

