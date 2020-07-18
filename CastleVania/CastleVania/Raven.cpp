#include "Raven.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "StairManager.h"

Raven::Raven(float x, float y, int Type, bool isActive, int State) : Enemy(x, y, Type, isActive, State)
{
	HP = RAVEN_HP;
}


void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Nếu Raven bị đánh bại và thời gian render hiệu ứng biến mất kết thúc
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) {
		
		//Raven biến mất
		setActive(false);
		
		//Cho phép hồi sinh Raven
		isNeedRevival = true;
	}

	//Gọi hàm Update của GameObject
	CGameObject::Update(dt, listObjects);


	//Luôn check vị trí của Simon
	float simonX, simonY;

	//Lấy vị trí của Simon
	simon->GetPosition(simonX, simonY);


	//Đang đứng yên
	if (state == ENEMY_INACTIVE) {
		//Khi hướng raven quay về chiều âm và simon cách raven một đoạn nhỏ hơn 185px thì raven tấn công về phía simon
		if (orientation == OR_MINUS && x - simonX < RAVEN_ACTIVE_RANGE) {
			SetState(ENEMY_WORKING);
		}


	}

	//Bắt đầu tấn công simon
	if (state == ENEMY_WORKING) {

		//Cho phép ghi lại thời gian trước khi tấn công. Thời gian này giúp ta xác định khi nào đến lần tấn công kế tiếp
		if (timeBeforeAttack==-1) {

			//Ghi lại thời gian trước khi tấn công
			timeBeforeAttack = GetTickCount();

		}


		//Để Raven raven tấn công cần có các điều kiện sau
		//1. Không phải ghi lại thời gian trước khi tấn công (vì đã ghi rồi , khối if bên trên). 
		//2. Lúc này GetTickCount() - timeBeforeAttack đang chạy và phát hiện > thời gian giữa 1 lần tấn công => hết thời gian nghỉ
		//3. Chưa ghi lại thời gian bắt đầu tấn công => Ghi lại để tính thời gian tấn công
		if (timeBeforeAttack!=-1 &&
			GetTickCount() - timeBeforeAttack > RAVEN_TIME_NEXT_ATTACK && 
			timeStartAttack == -1) {
			
			//Đã ghi thời gian bắt đầu tấn công . Để tính toán thời gian tấn công của Raven

			//Ghi thời gian bắt đầu tấn công
			timeStartAttack = GetTickCount();
			timeBeforeAttack = -1;
		}



		//Lúc này raven đang tấn công
		//1. Đã ghi lại thời gian bắt đầu tấn công
		//2. Thời gian hiện tại - thời gian bắt đầu tấn công < thời gian tấn công
		if (timeStartAttack !=-1 && GetTickCount() - timeStartAttack < RAVEN_TIME_ATTACK && state != ENEMY_DESTROYED) {


			//TÍNH VX (làm cho raven bay trái qua phải hoặc ngược lại), tùy vào vị trí của nó và simon
			//Nếu X của raven > X simon tức là raven đang ở bên phải simon => Raven phải hướng về bên trái và Vx âm
			if (simonX < x) {

				orientation = OR_MINUS;

				vx = -RAVEN_FLYING_SPEED_X;

			}
			//Nếu X của raven < X simon tức là raven đang ở bên trái simon => Raven phải hướng về bên phải và Vx dương
			else {
				orientation = OR_PLUS;

				vx = RAVEN_FLYING_SPEED_X;
			}



			//TÍNH VY (làm cho raven bay trên xuống hoặc dưới lên), tùy vào vị trí của nó và simon
			//Nếu Raven cách simon 1 đoạn 30px theo trục Y thì chuyển sang trạng thái tấn công theo chiều ngang => vy = 0
			if (abs(y - simonY) < RAVEN_FLYING_VERTICAL_RANGE) {
				vy = 0;
			}
			//Nếu y của Raven < y của Simon thì lúc này raven đang ở trên simon => phải tấn công xuống dưới => vy dương
			else if (y < simonY) {
				vy = RAVEN_FLYING_SPEED_Y;
			}
			//Nếu y của Raven > y của Simon thì lúc này raven đang ở dưới simon => phải tấn công lên trên => vy âm
			else {
				vy = -RAVEN_FLYING_SPEED_Y;
			}


		}
		//Lúc này raven đã quá thời gian được tấn công
		else if (timeStartAttack !=-1 && GetTickCount() - timeStartAttack > RAVEN_TIME_ATTACK) {
			timeBeforeAttack = GetTickCount();		//Ghi thời gian trước khi tấn công
			timeStartAttack = -1;
		}
		//Nếu đang ở trạng thái chờ tấn công
		else {
			//Cho raven đứng yên
			vx = vy = 0;
		}


		//Xét va chạm với Raven
		//Lưu objects có thể va chạm với Raven
		vector<LPGAMEOBJECT> coObjects;


		GetCollisionObject(listObjects, coObjects, simon);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;


		//Làm sạch để sử dụng
		coEvents.clear();


		//Tính toán các va chạm tiềm năng với danh sách các đối tượng có thể va chạm
		CalcPotentialCollisions(&coObjects, coEvents);


		//Nếu Raven không va chạm với bất cứ Object nào cả
		if (coEvents.size() == 0)
		{
			//Không dùng StopWatch
			if (!SubWeaponManager::GetInstance()->isUseStopWatch) {
				//Cập nhật vị trí của Raven
				x += dx;
				y += dy;
			}

		}
		else //Nếu có va chạm 
		{

			float min_tx, min_ty;	//Thời gian va chạm nhỏ nhất trên trục X,Y
			float nx = 0, ny;		//Hướng va chạm trên trục X,Y
			float rdx = 0;
			float rdy = 0;



			//Chỉ xử lí những va chạm gần nhất với đối tượng
			//Truyền vào coEvents (danh sách va chạm tiềm năng)
			//Lấy ra coEventsResult va chạm sau khi lọc
			//Lấy ra Thời gian va chạm nhỏ nhất trên trục X,Y
			//Lấy ra Hướng va chạm
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


			// block every object first!
			x += min_tx * dx + nx * 0.1f;
			y += min_ty * dy + ny * 0.1f;


			//Duyệt qua danh sách va chạm gần nhất
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				//Lấy từng phần tử trong danh sách
				LPCOLLISIONEVENT e = coEventsResult[i];

				


				if (dynamic_cast<Simon*>(e->obj))
				{
					simon->Attacked(RAVEN_ATK);
				}


			}

		}



		//Kiểm tra trạng thái bất tử của Raven
		UpdateImmortal(ENEMY_IMMORTAL_TIME);
	}


}


//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với HunchBack gán vào coObjects
void Raven::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {


	if (!simon->getIsImmortal() && !StairManager::GetInstance()->GetIsSimonStandingOnStair()) {
		coObjects.push_back(simon);
	}


}


void Raven::Render()
{
	SubWeaponManager* subWManager = SubWeaponManager::GetInstance();

	//Đang trong trạng thái bất tử => nhấp nháy
	if (isImmortal) {
		int alpha = rand() % 255 + 0;
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y, alpha);
	}
	//Nếu không render bình thường
	else {
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y);
	}

}


void Raven::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ENEMY_INACTIVE:
		vx = vy = 0;
		break;
	case ENEMY_WORKING:
		break;
	case ENEMY_DESTROYED:
		vx = vy = 0;
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());	//Ghi thời gian bắt đầu render hiệu ứng biến mất
		break;
	default:
		break;
	}
}

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + RAVEN_WIDTH;
	bottom = top + RAVEN_HEIGHT;
}

void Raven::GetBoundingBoxStart(float& left, float& top, float& right, float& bottom)
{
	left = positionStart.x;
	top = positionStart.y;
	right = left + RAVEN_WIDTH;
	bottom = top + RAVEN_HEIGHT;
}


//Tính lượng máu còn lại của Raven
void Raven::Attacked(int lostHP)
{
	Enemy::Attacked(lostHP);

	PlayerManager* playerManager = PlayerManager::GetInstance();

	//Raven không còn máu
	if (HP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);

		//Set điểm cho người chơi khi tiêu diệt Raven
		int score = playerManager->GetScore();
		score += RAVEN_EXP;
		playerManager->SetScore(score);
	}
}