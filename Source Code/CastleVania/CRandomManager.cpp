#include "CRandomManager.h"
#include "CDebug.h"
#include "PlayerManager.h"

CRandomManager* CRandomManager::_instance = NULL;

CRandomManager* CRandomManager::GetInstance()
{
	if (_instance == NULL) _instance = new CRandomManager();
	return _instance;
}


void CRandomManager::Init() {

	if (isInit) return;

	//khỡi tạo random cho 2 trạng thái bay của phantombat
	CRandomGroup* phantomBatFlyingAttack = new CRandomGroup();
	phantomBatFlyingAttack->AddRandomItem(new CRandomItem(PHANTOMBAT_FLYING_ATTACK_TYPE1, PHANTOMBAT_FLYING_ATTACK_TYPE1_RATIO));
	phantomBatFlyingAttack->AddRandomItem(new CRandomItem(PHANTOMBAT_FLYING_ATTACK_TYPE2, PHANTOMBAT_FLYING_ATTACK_TYPE2_RATIO));
	CRandomManager::GetInstance()->AddGroup(phantomBatFlyingAttack);


	//khỡi tạo random cho trạng thái bay thứ 1 của phantombat
	CRandomGroup* phantomBatFlyingAttack1 = new CRandomGroup();
	phantomBatFlyingAttack1->AddRandomItem(new CRandomItem(PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND1, PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND1_RATIO));
	phantomBatFlyingAttack1->AddRandomItem(new CRandomItem(PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND2, PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND2_RATIO));
	phantomBatFlyingAttack1->AddRandomItem(new CRandomItem(PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND3, PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND3_RATIO));
	CRandomManager::GetInstance()->AddGroup(phantomBatFlyingAttack1);



	//khỡi tạo random cho trạng thái bay thứ 2 của phantombat
	CRandomGroup* phantomBatFlyingAttack2 = new CRandomGroup();
	phantomBatFlyingAttack2->AddRandomItem(new CRandomItem(PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND1, PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND1_RATIO));
	phantomBatFlyingAttack2->AddRandomItem(new CRandomItem(PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND2, PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND2_RATIO));
	CRandomManager::GetInstance()->AddGroup(phantomBatFlyingAttack2);



	//khỡi tạo random type cho item 
	CRandomGroup* itemTypeRandom = new CRandomGroup();
	itemTypeRandom->AddRandomItem(new CRandomItem(NORMAL_ITEMS, RD_ITYPE_NORMAL_MSLEVEL1_RATIO));
	itemTypeRandom->AddRandomItem(new CRandomItem(SPECIAL_ITEMS, RD_ITYPE_SPECIAL_MSLEVEL1_RATIO));
	CRandomManager::GetInstance()->AddGroup(itemTypeRandom);


	//Khởi tạo random item Normal
	CRandomGroup* normalItemRandom = new CRandomGroup();
	normalItemRandom->AddRandomItem(new CRandomItem(ITEM_MORNINGSTAR, RD_NORMAL_ITEM_MORNINGSTAR_RATIO));
	normalItemRandom->AddRandomItem(new CRandomItem(ITEM_SMALLHEART, RD_NORMAL_ITEM_SMALLHEART_RATIO));
	normalItemRandom->AddRandomItem(new CRandomItem(ITEM_BIGHEART, RD_NORMAL_ITEM_BIGHEART_RATIO));
	normalItemRandom->AddRandomItem(new CRandomItem(ITEM_MONEYBAG_RED, RD_NORMAL_ITEM_MB_RED_RATIO));
	normalItemRandom->AddRandomItem(new CRandomItem(ITEM_MONEYBAG_PURPLE, RD_NORMAL_ITEM_MB_PURPLE_RATIO));
	normalItemRandom->AddRandomItem(new CRandomItem(ITEM_MONEYBAG_WHITE, RD_NORMAL_ITEM_MB_WHITE_RATIO));
	CRandomManager::GetInstance()->AddGroup(normalItemRandom);


	//Khởi tạo random item Special
	CRandomGroup* specialItemRandom = new CRandomGroup();
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_ROSARY, RD_SPECIAL_ITEM_ROSARY_RATIO));
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_INVISIBILITYPOTION, RD_SPECIAL_ITEM_INVISIBILITYPOTION));
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_DOUBLESHOT, RD_SPECIAL_ITEM_DOUBLESHOT));
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_TRIPLESHOT, RD_SPECIAL_ITEM_TRIPLESHOT));

	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_HOLYWATER, RD_SPECIAL_ITEM_HOLYWATER));
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_AXE, RD_SPECIAL_ITEM_AXE));
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_DAGGER, RD_SPECIAL_ITEM_DAGGER));
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_BOOMERANG, RD_SPECIAL_ITEM_BOOMERANG));
	specialItemRandom->AddRandomItem(new CRandomItem(ITEM_STOPWATCH, RD_SPECIAL_ITEM_STOPWATCH));
	CRandomManager::GetInstance()->AddGroup(specialItemRandom);



	//khỡi tạo random cho white skeleton nhảy tới hay bắn xương
	CRandomGroup* rdWhiteSkeleton = new CRandomGroup();
	rdWhiteSkeleton->AddRandomItem(new CRandomItem(RD_WHITESKELETON_CREATEBONE, RD_WHITESKELETON_CREATEBONE_RATIO));
	rdWhiteSkeleton->AddRandomItem(new CRandomItem(RD_WHITESKELETON_JUMPING, RD_WHITESKELETON_JUMPING_RATIO));
	CRandomManager::GetInstance()->AddGroup(rdWhiteSkeleton);


	//khỡi tạo random cho trạng thái đánh trúng simon hoặc không
	CRandomGroup* phantomBatHitSimon = new CRandomGroup();
	phantomBatHitSimon->AddRandomItem(new CRandomItem(PHANTOMBAT_HIT_SIMON, PHANTOMBAT_HIT_SIMON_RATIO));
	phantomBatHitSimon->AddRandomItem(new CRandomItem(PHANTOMBAT_NO_HIT_SIMON, PHANTOMBAT_NO_HIT_SIMON_RATIO));
	CRandomManager::GetInstance()->AddGroup(phantomBatHitSimon);



	//Set vùng random cho tất cả các item trong list group random
	for (int i = 0; i < listRandomGroup.size(); i++) {

		listRandomGroup.at(i)->SetRangeRandom();

	}

	isInit = true;
}




void CRandomManager::EditRangeInList(int idListRandom, int idValue, int Range) {

	listRandomGroup.at(idListRandom)->EditRangeRandomItem(idValue, Range);

}




int CRandomManager::Random(int idListGroup) {

	return listRandomGroup.at(idListGroup)->Random();
}



void CRandomManager::UpdateRandomMS() {

	int MSLevel = PlayerManager::GetInstance()->GetMSLevel();

	if (MSLevel == MS_LEVEL2) {
		listRandomGroup.at(LIST_RANDOM_ITEM_TYPE)->EditRangeRandomItem(NORMAL_ITEMS, RD_ITYPE_NORMAL_MSLEVEL2_RATIO);
		listRandomGroup.at(LIST_RANDOM_ITEM_TYPE)->EditRangeRandomItem(SPECIAL_ITEMS, RD_ITYPE_SPECIAL_MSLEVEL2_RATIO);
	}
	else if (MSLevel == MS_LEVEL3) {
		listRandomGroup.at(LIST_RANDOM_ITEM_TYPE)->EditRangeRandomItem(NORMAL_ITEMS, RD_ITYPE_NORMAL_MSLEVEL3_RATIO);
		listRandomGroup.at(LIST_RANDOM_ITEM_TYPE)->EditRangeRandomItem(SPECIAL_ITEMS, RD_ITYPE_SPECIAL_MSLEVEL3_RATIO);
	}

	listRandomGroup.at(LIST_RANDOM_ITEM_TYPE)->SetRangeRandom();
}