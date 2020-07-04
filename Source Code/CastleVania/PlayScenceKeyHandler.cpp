#include "PlayScenceKeyHandler.h"



void PlayScenceKeyHandler::KeyState(BYTE* state)
{
	//Lấy simon & stair Manager
	Simon* simon = ((PlayScene*)scence)->GetSimon();
	StairManager* stairManager = StairManager::GetInstance();


	//Có thể kết nối đến bàn phím hay không
	if (!forwardAction->canAccessKeyStateAction(simon)) return;


	////Kiểm tra va chạm giữa simon và các breakpoint stair mỗi khi nhấn phím
	stairManager->Update(simon);


	if (game->IsKeyDown(DIK_RIGHT))
	{
		forwardAction->KeyRightAction(simon);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		forwardAction->KeyLeftAction(simon);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		forwardAction->KeyDownAction(simon);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		forwardAction->KeyUpAction(simon);
	}
	else
	{
		forwardAction->NotPressKeyAction(simon);
	}






}






void PlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	Simon* simon = ((PlayScene*)scence)->GetSimon();
	MorningStar* ms = ((PlayScene*)scence)->GetMorningStar();

	if (!forwardAction->canAccessKeyDownAction(simon)) return;

	switch (KeyCode)
	{
	case DIK_A:
		if (game->IsKeyDown(DIK_UP))
			simon->UsingSubWeapon();
		else 
			simon->Whopping();
		break;
	case DIK_S:
		simon->UsingSubWeapon();
		break;

	case DIK_SPACE:
	{
		//Điều kiện để simon nhảy lên
		if (simon->GetState()==JUMPING || simon->GetState() == WHOPPING_SITTING || simon->GetState() == WHOPPING_STANDING || simon->GetState() == SITTING)
			return;
		simon->SetState(JUMPING);
	}
	break;


	//////CHEAT KEYS
	case DIK_B:
		PlayerManager::GetInstance()->SetSubweaponID(BOOMERANG_HUD);
		break;
	case DIK_W:
		PlayerManager::GetInstance()->SetSubweaponID(STOP_WATCH_HUD);
		break;
	case DIK_D:
		PlayerManager::GetInstance()->SetSubweaponID(DAGGER_HUD);
		break;
	case DIK_X:
		PlayerManager::GetInstance()->SetSubweaponID(AXE_HUD);
		break;
	case DIK_H:
		PlayerManager::GetInstance()->SetSubweaponID(HOLY_WATER_HUD);
		break;
	case DIK_C:
		PlayerManager::GetInstance()->SetSubweaponCounter(20);
		break;
	case DIK_P:
		PlayerManager::GetInstance()->SetSimonHP(HP_SIZE);
		break;
	case DIK_1:
		PlayerManager::GetInstance()->SetMSLevel(MS_LEVEL1);
		break;
	case DIK_2:
		PlayerManager::GetInstance()->SetMSLevel(MS_LEVEL2);
		break;
	case DIK_3:
		PlayerManager::GetInstance()->SetMSLevel(MS_LEVEL3);
		break;
	default:
		break;
	}

}
















