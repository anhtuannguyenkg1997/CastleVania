#include "Ghost.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "EnemyRevival.h"

Ghost::Ghost(float x, float y, int Type, bool isActive, int State) : Enemy(x, y, Type, isActive, State)
{
	deltaVY = GHOST_FLYING_DELTAVX;

	HP = GHOST_HP;
	ATK = GHOST_ATK;


}


void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>& coObject, Simon* simon)
{
	//Nếu Ghost bị đánh bại và thời gian render hiệu ứng biến mất kết thúc
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) {

		//Ghost biến mất
		setActive(false);

		//Cho phép hồi sinh Ghost
		isNeedRevival = true;
	}


	//Nếu ghost chưa xuất hiện
	if (state == ENEMY_INACTIVE) {

		//Không cho ghost rơi
		vx = vy = 0;


		//Luôn check vị trí của Simon
		float simonX, simonY;

		simon->GetPosition(simonX, simonY);


		//Khi hướng ghost quay về chiều âm và simon cách ghost 100px về chiều âm thì ghost xuất hiện
		if (orientation == OR_MINUS && x - simonX > GHOST_SPACE_TO_ACTIVE) {
			SetState(ENEMY_WORKING);
		}


		//Khi hướng ghost quay về chiều dương và simon cách ghost 100px về chiều âm thì ghost xuất hiện
		if (orientation == OR_PLUS && simonX - x > GHOST_SPACE_TO_ACTIVE) {
			SetState(ENEMY_WORKING);
		}

	}
	//Nếu Ghost đang bay
	else if (state == ENEMY_WORKING) {

		Enemy::Update(dt, coObject);

		//Biến thiên vận tốc theo thời gian
		vy += deltaVY;


		//Đến khoảng Vy cực đại  thì đổi chiều => ghost tâng lên xuống theo trục Y tạo chuyển động sóng
		if (vy > GHOST_FLYING_MAXVY || vy < -GHOST_FLYING_MAXVY) {
			deltaVY = -deltaVY;
		}


		//Lấy vận tốc của simon
		float vxSimon, vySimon;
		simon->GetSpeed(vxSimon, vySimon);


		if (state != ENEMY_DESTROYED) {

			//Ghost di chuyển theo simon theo chiều dương
			if (vxSimon <= 0 && vx < 0 && simon->GetX() > x) {
				vx = GHOST_FLYING_SPEED;
				orientation = OR_PLUS;
			}

			//Ghost di chuyển theo simon theo chiều âm
			if (vxSimon >= 0 && vx > 0 && simon->GetX() < x) {
				vx = -GHOST_FLYING_SPEED;
				orientation = OR_MINUS;
			}
		}



		//Không dùng StopWatch
		if (!SubWeaponManager::GetInstance()->isUseStopWatch) {
			////Update tọa độ của BN, cộng dồn tọa độ cũ với quãng đường đi được sau thời gian dt
			x += dx;
			y += dy;
		}



		//Kiểm tra trạng thái bất tử
		UpdateImmortal(ENEMY_IMMORTAL_TIME);

	}


}




void Ghost::Render()
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




void Ghost::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ENEMY_INACTIVE:
		vx = vy = 0;
		break;
	case ENEMY_WORKING:
		if (orientation == OR_MINUS)
			vx = -GHOST_FLYING_SPEED;
		else
			vx = GHOST_FLYING_SPEED;
		break;
	case ENEMY_DESTROYED:
		vx = vy = 0;
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + GHOST_WIDTH;
	bottom = top + GHOST_HEIGHT;
}



void Ghost::GetBoundingBoxStart(float& left, float& top, float& right, float& bottom)
{
	left = positionStart.x;
	top = positionStart.y;
	right = left + GHOST_WIDTH;
	bottom = top + GHOST_HEIGHT;
}



//Tính lượng máu còn lại của Ghost
void Ghost::Attacked(int lostHP)
{
	Enemy::Attacked(lostHP);
	PlayerManager* playerManager = PlayerManager::GetInstance();

	//Ghost không còn máu
	if (HP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);

		//Set điểm cho người chơi khi tiêu diệt Ghost
		int score = playerManager->GetScore();
		score += GHOST_EXP;
		playerManager->SetScore(score);
	}
}