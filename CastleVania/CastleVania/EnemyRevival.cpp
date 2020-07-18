#include "EnemyRevival.h"
#include "CCamera.h"
#include "HunchBack.h"
#include "Ghost.h"
#include "VampireBat.h"
#include "CGrid.h"

EnemyRevival* EnemyRevival::_instance = NULL;



EnemyRevival* EnemyRevival::GetInstance()
{
	if (_instance == NULL) _instance = new EnemyRevival();
	return _instance;
}



void EnemyRevival::Update() {

	//Nếu không có enemy nào cần hồi sinh
	if (listObjectRevival.size() == 0) return;


	//Duyệt danh sách enemy cần hồi sinh
	for (int i = 0; i < listObjectRevival.size(); i++) {

		Enemy* enemy = dynamic_cast<Enemy*>(listObjectRevival[i]->GetObj());


		//Nếu vị trí ban đầu của enemy ra ngoài vùng camera thì ta sẽ hồi sinh cho enemy
		if(!CCamera::GetInstance()->checkObjectInCameraRevival(enemy)){

			enemy->setActive(true);		

			//Lấy lại vị trí ban đầu của enemy
			D3DXVECTOR2 positionStart = enemy->GetPositionStart();
			enemy->SetPosition(positionStart.x, positionStart.y);
			

			//Hồi sinh cho enemy
			//Các trạng thái ban đầu của Enemy sẽ được set ở đây
			if (dynamic_cast<Ghost*>(enemy)) {
				enemy->SetState(ENEMY_INACTIVE);
			}
			if (dynamic_cast<HunchBack*>(enemy)) {
				HunchBack* hunchBack = dynamic_cast<HunchBack*>(enemy);
				hunchBack->SetState(ENEMY_WORKING);
				hunchBack->SetIsActive(false);
				hunchBack->SetTimeJumped(-1);
			}
			else {
				enemy->SetState(ENEMY_WORKING);
			}
			
			//Update lại chính objectCell này 
			//vì lúc này vị trí objectCell ghi nhận là vị trí mà enemy biến mất
			//=> không khớp với vị trí của enemy hiện tại
			CGrid::GetInstance()->UpdateObjectCell(listObjectRevival[i], positionStart.x, positionStart.y);


			//Xóa enemy trong listObjectRevival 
			auto it = std::find(listObjectRevival.begin(), listObjectRevival.end(), listObjectRevival[i]);
			if (it != listObjectRevival.end()) { listObjectRevival.erase(it); }

		}

	}


}



void EnemyRevival::Clear() {

	int size = listObjectRevival.size();

	//Nếu listObjectCells không có element nào thì ta return luôn
	if (size == 0) return;

	listObjectRevival.clear();
}