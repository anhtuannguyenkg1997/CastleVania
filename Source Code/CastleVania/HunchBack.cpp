#include "HunchBack.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "EnemyRevival.h"
#include "StairManager.h"

HunchBack::HunchBack(float x, float y, int Type, bool isActive, int State) : Enemy(x, y, Type, isActive, State)
{

	HP = HUNCH_BACK_HP;

}




void HunchBack::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	SubWeaponManager* subWManager = SubWeaponManager::GetInstance();



	//Nếu HunchBack bị tiêu diệt và thời gian render hiệu ứng biến mất kết thúc
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) {
		
		//HunchBack biến mất
		setActive(false);

		//Cho phép hồi sinh HunchBack
		isNeedRevival = true;

	}
		

	//Kiểm tra có dùng Stopwatch hay không???
	if (subWManager->isUseStopWatch) return;


	//Gọi hàm Update của GameObject
	CGameObject::Update(dt, listObjects);


	//Lưu objects có thể va chạm với Simon
	vector<LPGAMEOBJECT> coObjects;


	//Lấy object có khả năng va chạm với Zombie
	GetCollisionObject(listObjects, coObjects, simon);


	//Simple falldown
	vy += HUNCH_BACK_GRAVITY * dt;



	//Kiểm tra va chạm của HunchBack
	//Danh sách va chạm có thể có
	vector<LPCOLLISIONEVENT> coEvents;

	//Danh sách thực sự va chạm
	vector<LPCOLLISIONEVENT> coEventsResult;


	//Làm sạch để sử dụng
	coEvents.clear();


	//Tính toán các va chạm tiềm năng với danh sách các đối tượng có thể va chạm
	CalcPotentialCollisions(&coObjects, coEvents);



	//Nếu HunchBack không va chạm với bất cứ Object nào cả
	if (coEvents.size() == 0)
	{
		//Update tọa độ của Simon, cộng dồn tọa độ cũ với quãng đường đi được sau thời gian dt
		x += dx;
		y += dy;

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


		////Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Va chạm dưới với Brick thì HunchBack đứng yên
			if (dynamic_cast<Brick*>(e->obj))
			{
				if (e->ny != 0 && e->ny == COLLISION_DIRECTION_BOTTOM) {
					vx = vy = 0;
				}
				
			}

			if (dynamic_cast<Simon*>(e->obj))
			{
				//Xử lí khi va chạm với simon
				simon->LoseHP(HUNCH_BACK_ATK);	//Trừ HP của Simon
				simon->SetState(PUSHED);		//Đẩy simon ra


				//Nếu HP của simon < 0 thì simon chuyển về trạng thái dead
				if (PlayerManager::GetInstance()->GetSimonHP() <= 0) {
					simon->SetState(DEAD);
				}
				else {//Nếu không
					//Set trạng thái bất tử	
					simon->SetImmortal();
				}
			}
		}

	}

	//Luôn check vị trí của Simon
	float simonX, simonY;

	simon->GetPosition(simonX, simonY);

	//Set hướng của HB luôn quay về phía Simon
	if (simonX > x) {
		orientation = OR_PLUS;
	}
	else if (simonX < x) {
		orientation = OR_MINUS;
	}

	//Kích hoạt HunchBack
	if (!isActive) {
		//Khi simon cách HunchBack 153px bất kể chiều nào thì hunch back được kích hoạt
		if ((orientation == OR_PLUS && simonX - x < HUNCH_BACK_SPACE_TO_ACTIVE)||
			(orientation == OR_MINUS && x - simonX < HUNCH_BACK_SPACE_TO_ACTIVE)) 
		{
			isActive = true;
		}
	}


	//Nếu HunchBack đã kích hoạt và chưa nhảy
	if (timeJumped==-1 && isActive && state != ENEMY_DESTROYED) {

		////Khi hướng hunchback quay về chiều dương
		if (orientation == OR_PLUS) {
			vx = HUNCH_BACK_WALKING_SPEED;		//HB nhảy về chiều dương
			vy = -HUNCH_BACK_JUMPING_SPEED;
			timeJumped = GetTickCount();		//Bắt đầu ghi lại thời gian nhảy gần nhất
		}
		////Khi hướng hunchback quay về chiều âm
		else if (orientation == OR_MINUS) {
			vx = -HUNCH_BACK_WALKING_SPEED;		//HB nhảy về chiều âm
			vy = -HUNCH_BACK_JUMPING_SPEED;
			timeJumped = GetTickCount();		//Bắt đầu ghi lại thời gian nhảy gần nhất
		}

	}


	//Thời gian cách nhau của mỗi lần nhảy là 2000s
	if (isActive && timeJumped!=-1 && GetTickCount()- timeJumped> HUNCH_BACK_TIME_TO_JUMP) {	
		//Cho phép nhảy
		timeJumped = -1;	//Reset lại thời gian lần cuối nhảy
		vx = vy = 0;		//Không nhảy thì vx=vy=0
	}


	//Cập nhật trạng thái bất tử của Zombie
	UpdateImmortal(ENEMY_IMMORTAL_TIME);



	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];


}



//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với HunchBack gán vào coObjects
void HunchBack::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {


	if (!simon->getIsImmortal() && !StairManager::GetInstance()->GetIsSimonStandingOnStair()) {
		coObjects.push_back(simon);
	}


	for (auto obj : listObjects)
	{
		if (dynamic_cast<Brick*>(obj))
			coObjects.push_back(obj);
	}


}



void HunchBack::Render()
{
	SubWeaponManager* subWManager = SubWeaponManager::GetInstance();

	if (isImmortal) {
		int alpha = rand() % 255 + 0;
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y, alpha);
	}
	else {
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y);
	}

	//RenderBoundingBox();
}




void HunchBack::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ENEMY_WORKING:
		vx = vy = 0;
		break;
	case ENEMY_DESTROYED:
		vx = vy = 0;
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}

void HunchBack::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + HUNCH_BACK_WIDTH;
	bottom = top + HUNCH_BACK_HEIGHT;
}


void HunchBack::GetBoundingBoxStart(float& left, float& top, float& right, float& bottom)
{
	left = positionStart.x;
	top = positionStart.y;
	right = left + HUNCH_BACK_WIDTH;
	bottom = top + HUNCH_BACK_HEIGHT;
}


//Tính lượng máu còn lại của HunchBack
void HunchBack::Attacked(int lostHP)
{
	Enemy::Attacked(lostHP);
	PlayerManager* playerManager = PlayerManager::GetInstance();

	//HunchBack không còn máu
	if (HP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);

		//Set điểm cho người chơi khi tiêu diệt HunchBack
		int score = playerManager->GetScore();
		score += HUNCH_BACK_EXP;
		playerManager->SetScore(score);
	}
}