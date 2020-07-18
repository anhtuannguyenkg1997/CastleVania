#include "VampireBat.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "EnemyRevival.h"

VampireBat::VampireBat(float x, float y, int Type, bool isActive, int State) : Enemy(x, y, Type, isActive, State)
{
	deltaVY = VAMPIRE_BAT_DELTA_VY;

	HP = VAMPIREBAT_HP;
	ATK = VAMPIREBAT_ATK;


}



void VampireBat::Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon)
{
	//Nếu dơi bị đánh bại và thời gian render hiệu ứng biến mất kết thúc
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) {
		setActive(false);//Con dơi biến mất
	}

	//Nếu con dơi đang ở trạng thái đứng yên
	if (state == ENEMY_INACTIVE) {

		//Không cho Bat rơi
		vx = 0;
		vy = 0;


		//Luôn check vị trí của Simon
		float simonX, simonY;

		simon->GetPosition(simonX, simonY);


		//Đến vị trí này thì dơi bắt đầu bay
		if (simonX < simonPositionX && simonY < simonPositionY)
			SetState(ENEMY_WORKING);

	}
	//Nếu Bat bay
	else if (state == ENEMY_WORKING) {

		//Không dùng StopWatch
		if (!SubWeaponManager::GetInstance()->isUseStopWatch) {
			Enemy::Update(dt, coObject);

			//Tăng vy => dơi rơi xuống
			vy += deltaVY;


			//Đến khoảng Vy cực đại => không rơi nữa
			if (vy >= VAMPIRE_BAT_MAX_VY) {
				deltaVY = 0;
				vy = 0;
			}

			//Cập nhật tọa độ của VB
			x += dx;
			y += dy;
		}


		UpdateImmortal(ENEMY_IMMORTAL_TIME);

	}


}




void VampireBat::Render()
{
	SubWeaponManager* subWManager = SubWeaponManager::GetInstance();

	if (isImmortal) {
		int alpha = rand() % 255 + 0;
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y, alpha);
	}
	else {
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y);
	}

}




void VampireBat::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ENEMY_INACTIVE:
		vx = vy = 0;
		break;
	case ENEMY_WORKING:
		if (orientation > 0)
			vx = VAMPIRE_BAT_FLYING_SPEED;
		else
			vx = -VAMPIRE_BAT_FLYING_SPEED;
		vy = 0;
		break;
	case ENEMY_DESTROYED:
		vx = 0;
		vy = 0;
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}




void VampireBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + VAMPIRE_BAT_WIDTH;
	bottom = top + VAMPIRE_BAT_HEIGHT;
}





//Tính lượng máu còn lại của VampireBat
void VampireBat::Attacked(int HPLose)
{
	Enemy::Attacked(HPLose);

	PlayerManager* playerManager = PlayerManager::GetInstance();

	//VampireBat không còn máu
	if (HP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);

		//Set điểm cho người chơi khi tiêu diệt blackKnight
		int score = playerManager->GetScore();
		score += VAMPIREBAT_EXP;
		playerManager->SetScore(score);
	}
}