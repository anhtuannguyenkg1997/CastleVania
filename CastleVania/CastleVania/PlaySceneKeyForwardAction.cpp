#include "PlaySceneKeyForwardAction.h"
#include "PlayerManager.h"

PlaySceneKeyForwardAction* PlaySceneKeyForwardAction::_instance = NULL;


PlaySceneKeyForwardAction* PlaySceneKeyForwardAction::GetInstance()
{
	if (_instance == NULL) _instance = new PlaySceneKeyForwardAction();
	return _instance;
}





//Khi nào được phép đè phím (VD: đè phím qua trái, qua phải,...)
bool PlaySceneKeyForwardAction::canAccessKeyStateAction(Simon* simon) {
	//Simon chết thì không cho nhấn phím
	if (simon->GetState() == DEAD || PlayerManager::GetInstance()->isFinishGame) return false;

	//Chờ render animation
	if (!simon->isFinishRenderAnimation()) return false;

	//Simon chưa nhảy xong (tức là nhảy mà chưa chạm đất)
	if (simon->GetState() == JUMPING && !simon->animation_set->at(JUMPING)->isFinishAnimation(SIMON_JUMPING_TIME))
		return false;


	return true;
}





//Khi nào được phép nhấn phím (VD: đánh, nhảy,...)
bool PlaySceneKeyForwardAction::canAccessKeyDownAction(Simon* simon) {
	
	//Simon chết thì không cho nhấn phím
	if (simon->GetState() == DEAD || PlayerManager::GetInstance()->isFinishGame) return false;

	//Simon chưa nhảy xong (tức là nhảy mà chưa chạm đất)
	if (simon->GetState() == WHOPPING_STANDING && !simon->animation_set->at(JUMPING)->isFinishAnimation(SIMON_JUMPING_TIME)) 
		return true;

	//Chờ render animation
	if (!simon->isFinishRenderAnimation()) return false;

	return true;
}





//Simon đi qua phải
void PlaySceneKeyForwardAction::KeyRightAction(Simon* simon) {
	
	//Nếu simon đang đứng trên cầu thang
	if (stairManager->GetIsSimonStandingOnStair())
	{
		// Và cầu thang từ trái qua phải (chân cầu thang gần X0 hơn)
		if (stairManager->GetStairOrientation() == STAIR_ORI_LEFT_TO_RIGHT) 
			//Xử lí simon đi lên
			stairManager->SimonWalkingUpStair(simon);
		else
		// Nếu cầu thang từ phải qua trái (chân cầu thang xa X0 hơn)
		//Xử lí simon đi xuống
			stairManager->SimonWalkingDownStair(simon);

		//Skip below code
		return;
	}

	//Nếu không thì simon đang đứng trên mặt đất => cho simon đi qua phải
	simon->SetOrientation(OR_PLUS);
	simon->SetState(WALKING);
}




//Simon đi qua trái
void PlaySceneKeyForwardAction::KeyLeftAction(Simon* simon) {
	
	//Nếu simon đang đứng trên cầu thang
	if (stairManager->GetIsSimonStandingOnStair())
	{
		// Và cầu thang từ trái qua phải (chân cầu thang gần X0 hơn)
		if (stairManager->GetStairOrientation() == STAIR_ORI_LEFT_TO_RIGHT)
			//Xử lí simon đi xuống
			stairManager->SimonWalkingDownStair(simon);
		else // Nếu cầu thang từ phải qua trái (chân cầu thang xa X0 hơn)
			//Xử lí simon đi lên
			stairManager->SimonWalkingUpStair(simon);

		//Skip below code
		return;
	}

	//Nếu không thì simon đang đứng trên mặt đất => cho simon đi qua trái
	simon->SetOrientation(OR_MINUS);
	simon->SetState(WALKING);
}



//Khi nhấn phím xuống
void PlaySceneKeyForwardAction::KeyDownAction(Simon* simon) {
	
	//Nếu simon đang trên thang
	if (stairManager->GetAbleSimonWalkingDown())
	{
		//Simon đi xuống
		stairManager->SimonWalkingDownStair(simon);
		return;
	}

	//Nếu không
	simon->SetState(SITTING);
}



//Khi nhấn phím lên
void PlaySceneKeyForwardAction::KeyUpAction(Simon* simon) {
	//Nếu simon đang va chạm với thang và có thể đi lên
	if (stairManager->GetAbleSimonWalkingUp())
	{
		//Simon đi lên 
		stairManager->SimonWalkingUpStair(simon);
		return;
	}

	//Khi simon đã ra khỏi thang
	simon->SetState(STANDING);	//Trở về trạng thái Stand
}




//Khi không nhấn phím nào
void PlaySceneKeyForwardAction::NotPressKeyAction(Simon* simon) {
	subweaponManager->createSubweapon = false;


	//Nếu simon đang đứng trên thang
	if (stairManager->GetIsSimonStandingOnStair())
	{
		//Xử lí khi Simon đang đứng trên thang
		stairManager->SimonStandingOnStair(simon);

		//Skip below code
		return;
	}


	//Lúc này simon đã rời thang
	//Tắt hết flag quản lý simon trên thang
	stairManager->SetAbleSimonWalkingDown(false);
	stairManager->SetAbleSimonWalkingUp(false);
	stairManager->SetIsSimonStandingOnStair(false);

	//Simon đứng trên mặt đất
	simon->SetState(STANDING);
}