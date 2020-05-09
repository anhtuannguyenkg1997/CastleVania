#include "CPlayScenceKeyHandler.h"



void CPlayScenceKeyHandler::KeyState(BYTE* state)
{

	CGame* game = CGame::GetInstance();
	Simon* simon = ((PlayScene*)scence)->GetSimon();

	//Chờ render animation
	if (!simon->isFinishRenderAnimation())
		return;

	// nếu simon đang nhảy và chưa chạm đất
	if ((simon->GetState() == JUMPING || simon->GetState() == STANDING)
		&& simon->standing == false)
		return;


	//Nếu simon đi qua phải
	if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetOrientation(OR_PLUS);
		simon->SetState(WALKING);
	}
	//Nếu Simon đi qua trái
	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetOrientation(OR_MINUS);
		simon->SetState(WALKING);
	}
	//Nếu Simon ngồi
	else if (game->IsKeyDown(DIK_DOWN))
	{
		//Nếu simon đang rơi thì không cho ngồi
		if (simon->standing == false || simon->falling == true)
		{
			simon->SetState(STANDING);
			return;
		}

		simon->SetState(SITTING);
	}
	//Nếu không nhấn phím
	else
	{
		simon->SetState(STANDING);
	}

}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	Simon* simon = ((PlayScene*)scence)->GetSimon();

	//Kết thúc render animation simon thì mới cho phép nhấn phím
	if (!simon->isFinishRenderAnimation())
		return;

	switch (KeyCode)
	{
	case DIK_A:
		Simon_Hit();
		break;
	case DIK_SPACE:
	{
		//Điều kiện để simon nhảy lên
		if (simon->standing == false || simon->GetState() == WHOPPING_SITTING || simon->GetState() == WHOPPING_STANDING || simon->GetState() == SITTING)
			return;
		simon->SetState(JUMPING);
	}
	break;
	default:
		break;
	}
}



void CPlayScenceKeyHandler::Simon_Hit()
{
	Simon* simon = ((PlayScene*)scence)->GetSimon();


	//Nếu đang đánh thì không cho đánh tiếp
	if (simon->GetState() == WHOPPING_SITTING || simon->GetState() == WHOPPING_STANDING)
		return;

	//Nếu đang rơi thì không cho đánh
	if (simon->falling == true)
		return;

	//Đứng và đánh
	if (simon->GetState() == STANDING || simon->GetState() == JUMPING)
	{
		simon->SetState(WHOPPING_STANDING);
	}

	//Ngồi và đánh
	else if (simon->GetState() == SITTING)
	{
		simon->SetState(WHOPPING_SITTING);
	}
}


