#include "BlackKnight.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "EnemyRevival.h"
#include "StairManager.h"


BlackKnight::BlackKnight(float x, float y, int Type, bool isActive, int State) : Enemy(x, y, Type, isActive, State)
{
	HP = BLACKKNIGHT_HP;
	ATK = BLACKKNIGHT_ATK;

	vx = BLACKKNIGHT_WALKING_SPEED;
}





void BlackKnight::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{

	//Nếu BlackKnight bị tiêu diệt
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) {

		//BlackKnight biến mất
		this->active = false;

		//Cho phép hồi sinh BlackKnight
		isNeedRevival = true;
	}


	//Kiểm tra có dùng Stopwatch hay không???
	if (SubWeaponManager::GetInstance()->isUseStopWatch) return;


	//Gọi hàm Update của GameObject
	CGameObject::Update(dt, listObjects);


	//Lưu objects có thể va chạm với Simon
	vector<LPGAMEOBJECT> coObjects;


	//Lấy object có khả năng va chạm với Zombie
	GetCollisionObject(listObjects, coObjects);


	//Simple falldown
	vy += BLACKKNIGHT_GRAVITY * dt;



	//Kiểm tra va chạm của BlackKnight
	//Danh sách va chạm có thể có
	vector<LPCOLLISIONEVENT> coEvents;

	//Danh sách thực sự va chạm
	vector<LPCOLLISIONEVENT> coEventsResult;


	//Làm sạch để sử dụng
	coEvents.clear();


	//Tính toán các va chạm tiềm năng với danh sách các đối tượng có thể va chạm
	CalcPotentialCollisions(&coObjects, coEvents);


	//Nếu Simon không va chạm với bất cứ Object nào cả
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

		// block every object first!
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;


		////Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Chạm gạch Enemy thì đổi chiều
			if (dynamic_cast<Brick*>(e->obj) && e->obj->GetType() == ENEMY_BRICK)
			{
				vx *= -1;
				orientation *= -1;
			}
		}

	}


	//Lấy vị trí của simon
	float simonX, simonY, simonVX, simonVY;
	simon->GetPosition(simonX, simonY);
	simon->GetSpeed(simonVX, simonVY);


	//Khi simon di chuyễn vào vùng active của BlackKnight
	if (abs(x - simonX) < BN_REGION_CO_X && abs(y - simonY) < BN_REGION_CO_Y && state != ENEMY_DESTROYED) {
		//1, simon đang di chuyển về chiều âm
		//2, vận tốc của BlackKnight âm
		//3, tọa độ X của simon > BlackKnight
		if (simonVX <= 0 && vx < 0 && simonX > x) {

			//BN di chuyển về chiều dương
			vx = BLACKKNIGHT_WALKING_SPEED;
			orientation = OR_PLUS;
		}


		//1, simon đang di chuyển về chiều dương
		//2, vận tốc của BlackKnight dương
		//3, tọa độ X của simon < BlackKnight
		if (simonVX >= 0 && vx > 0 && simonX < x) {

			//BN di chuyển về chiều âm
			vx = -BLACKKNIGHT_WALKING_SPEED;
			orientation = OR_MINUS;
		}

	}


	//Cập nhật trạng thái bất tử của BlackKnight
	UpdateImmortal(ENEMY_IMMORTAL_TIME);



	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}



//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với Zombie gán vào coObjects
void BlackKnight::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects) {

	//BlackKnight va chạm với Brick
	for (auto obj : listObjects)
	{
		if (dynamic_cast<Brick*>(obj))
			coObjects.push_back(obj);
	}


}



//Vẽ Solider
void BlackKnight::Render()
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



void BlackKnight::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case ENEMY_WORKING:
		break;
	case ENEMY_DESTROYED:
		vx = 0;
		animation_set->at(state)->startCountTimeAnimation(GetTickCount()); //Set thời gian hiệu ứng phá hủy
		break;
	default:
		break;
	}
}



//Lấy vùng bao của Solider
void BlackKnight::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BLACKKNIGHT_WIDTH;
	bottom = top + BLACKKNIGHT_HEIGHT;
}


void BlackKnight::GetBoundingBoxStart(float& left, float& top, float& right, float& bottom)
{
	left = positionStart.x;
	top = positionStart.y;
	right = left + BLACKKNIGHT_WIDTH;
	bottom = top + BLACKKNIGHT_HEIGHT;
}


//Tính lượng máu còn lại của BlackKnight
void BlackKnight::Attacked(int lostHP)
{
	Enemy::Attacked(lostHP);
	PlayerManager* playerManager = PlayerManager::GetInstance();

	//BlackKnight không còn máu
	if (HP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);

		//Set điểm cho người chơi khi tiêu diệt blackKnight
		int score = playerManager->GetScore();
		score += BLACKKNIGHT_EXP;
		playerManager->SetScore(score);
	}

}