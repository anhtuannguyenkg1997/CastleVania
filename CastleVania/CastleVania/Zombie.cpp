#include "Zombie.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "StairManager.h"
#include "InvisibilityItemEffect.h"

Zombie::Zombie(float x, float y, int Type, bool isActive, int State) : Enemy(x, y, Type, isActive, State)
{
	HP = ZOMBIE_HP;

	vx = -ZOMBIE_WALKING_SPEED;
}




void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Nếu Zombie bị đánh bại và thời gian render hiệu ứng biến mất kết thúc
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) {

		//Zombie biến mất
		setActive(false);

		//Cho phép hồi sinh Zombie
		isNeedRevival = true;

	}


	//Gọi hàm Update của GameObject
	CGameObject::Update(dt, listObjects);


	//Lưu objects có thể va chạm với Zombie
	vector<LPGAMEOBJECT> coObjects;


	//Lấy object có khả năng va chạm với Zombie
	GetCollisionObject(listObjects, coObjects, simon);


	//Simple falldown
	vy += ZOMBIE_GRAVITY * dt;


	//Kiểm tra va chạm của Zombie
	//Danh sách va chạm có thể có
	vector<LPCOLLISIONEVENT> coEvents;

	//Danh sách thực sự va chạm
	vector<LPCOLLISIONEVENT> coEventsResult;


	//Làm sạch để sử dụng
	coEvents.clear();


	//Tính toán các va chạm tiềm năng với danh sách các đối tượng có thể va chạm
	CalcPotentialCollisions(&coObjects, coEvents);


	//Nếu Zombie không va chạm với bất cứ Object nào cả
	if (coEvents.size() == 0)
	{
		//Không dùng StopWatch
		if (!SubWeaponManager::GetInstance()->isUseStopWatch) {
			//Update tọa độ của Zombie, cộng dồn tọa độ cũ với quãng đường đi được sau thời gian dt
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
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;


		////Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Simon*>(e->obj))
			{
				
				simon->Attacked(ZOMBIE_ATK);	//Trừ HP của Solider
				//simon->SetState(PUSHED);


				////Nếu HP của simon < 0 thì simon chuyển về trạng thái dead
				//if (PlayerManager::GetInstance()->GetSimonHP() <= 0) {
				//	simon->SetState(DEAD);
				//}
				//else {//Nếu không
				//	//Set trạng thái bất tử	
				//	simon->SetImmortal();
				//}
			}
		}

	}




	//Luôn check vị trí của Simon
	float simonX, simonY;

	simon->GetPosition(simonX, simonY);

	//Kiểm tra để khi Zombie chết sẽ không di chuyển được
	if (state != ENEMY_DESTROYED) {

		//Set hướng và vận tốc của Zombie luôn về phía Simon
		if (simonX > x) {
			orientation = OR_PLUS;
			vx = ZOMBIE_WALKING_SPEED;
		}
		else if (simonX < x) {
			orientation = OR_MINUS;
			vx = -ZOMBIE_WALKING_SPEED;
		}
		else vx = 0;
	}





	//Cập nhật trạng thái bất tử của Zombie
	UpdateImmortal(ENEMY_IMMORTAL_TIME);



	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}



//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với Zombie gán vào coObjects
void Zombie::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {

	//Simon không ở trạng thái bất tử, không ở trên thang và không vô hình thì xét va chạm với Zombie
	if (!simon->getIsImmortal() && 
		!StairManager::GetInstance()->GetIsSimonStandingOnStair() && 
		!InvisibilityItemEffect::GetInstance()->isInvisibility) {
		coObjects.push_back(simon);
	}

	//Zombie va chạm với Brick
	for (auto obj : listObjects)
	{
		if (dynamic_cast<Brick*>(obj))
			coObjects.push_back(obj);
	}


}



void Zombie::Render()
{
	SubWeaponManager* subWManager = SubWeaponManager::GetInstance();

	//Đang trong trạng thái bất tử thì nhấp nháy
	if (isImmortal) {
		int alpha = rand() % 255 + 0;
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y, alpha);
	}

	//Nếu không thì render bình thường
	else {
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y);
	}

	//RenderBoundingBox();
}




void Zombie::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ENEMY_WORKING:
		vx = -ZOMBIE_WALKING_SPEED;
		break;
	case ENEMY_DESTROYED:
		vx = vy = 0;
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ZOMBIE_WIDTH;
	bottom = top + ZOMBIE_HEIGHT;
}

void Zombie::GetBoundingBoxStart(float& left, float& top, float& right, float& bottom)
{
	left = positionStart.x;
	top = positionStart.y;
	right = left + ZOMBIE_WIDTH;
	bottom = top + ZOMBIE_HEIGHT;


}

//Tính lượng máu còn lại của Zombie
void Zombie::Attacked(int lostHP)
{
	Enemy::Attacked(lostHP);

	PlayerManager* playerManager = PlayerManager::GetInstance();

	//Zombie không còn máu
	if (HP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);

		//Set điểm cho người chơi khi tiêu diệt Zombie
		int score = playerManager->GetScore();
		score += ZOMBIE_EXP;
		playerManager->SetScore(score);
	}
}