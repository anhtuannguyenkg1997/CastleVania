#include "CheatingStair.h"
#include "StairManager.h"

CheatingStair* CheatingStair::_instance = NULL;


CheatingStair* CheatingStair::GetInstance()
{
	if (_instance == NULL) _instance = new CheatingStair();
	return _instance;
}



void CheatingStair::CheckForCheat(int idScene, Simon* simon) {

	StairManager* stairManager = StairManager::GetInstance();

	//Nếu simon khác NULL mới cho cheat (vì cần get và set Simon)
	if (simon != NULL) {

		//Cheat cho hai màn chơi 3 (simon đứng trên cầu thang khi bắt đầu màn chơi)
		if (idScene == 3) {

			//Nếu chưa cheat lần nào thì cheat
			if (ableToCheat) {
				//Cho phép simon đi lên trên thang
				stairManager->SetAbleSimonWalkingUp(true);

				//Trạng thái đang đi lên thang
				simon->SetState(WALKING_UP_STAIR);

				//Hướng chiều dương
				simon->SetOrientation(OR_PLUS);

				//Simon đang đứng trên thang
				stairManager->SetIsSimonStandingOnStair(true);

				//Set hướng cầu thang (nếu không sẽ bị lỗi simon đi sai hướng)
				stairManager->SetStairOrientation(STAIR_ORI_LEFT_TO_RIGHT);
				
				//Không cho cheat nữa, vì nếu không sẽ liên tục chạy vào khối if này => lỗi game
				ableToCheat = false;
			}
		}



		//Cheat cho hai màn chơi 5 (simon đứng trên cầu thang khi bắt đầu màn chơi)
		if (idScene == 5) {


			//Nếu chưa cheat lần nào thì cheat
			if (ableToCheat) {
				//Cho phép simon đi lên trên thang
				stairManager->SetAbleSimonWalkingUp(true);

				//Trạng thái đang đi lên thang
				simon->SetState(WALKING_UP_STAIR);

				//Simon đang đứng trên thang
				stairManager->SetIsSimonStandingOnStair(true);

				//Hướng chiều âm
				simon->SetOrientation(OR_MINUS);

				//Set hướng cầu thang (nếu không sẽ bị lỗi simon đi sai hướng)
				stairManager->SetStairOrientation(STAIR_ORI_RIGHT_TO_LEFT);

				//Không cho cheat nữa, vì nếu không sẽ liên tục chạy vào khối if này => lỗi game
				ableToCheat = false;
			}

		}

		

	}



}