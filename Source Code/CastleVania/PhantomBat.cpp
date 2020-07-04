#include "PhantomBat.h"
#include "SubWeaponManager.h"
#include "CCamera.h"
#include "CAnimations.h"
#include "PlayerManager.h"
#include "StairManager.h"


PhantomBat::PhantomBat(float x, float y, int Type, bool isActive, int State) : Enemy(x, y, Type, isActive, State)
{
	ATK = PHANTOMBAT_ATK;
}


void PhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	SubWeaponManager* subWManager = SubWeaponManager::GetInstance();
	PlayerManager* playerManager = PlayerManager::GetInstance();
	CCamera* camera = CCamera::GetInstance();

	//Nếu PhantomBat bị đánh bại và thời gian render hiệu ứng biến mất kết thúc
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME_BOSS) == true)
		setActive(false);//PhantomBat biến mất


	//Simple fallup
	if (isFallingUp) {
		vy -= PHANTOMBAT_VELOCITY_FALLDOWN * dt;
	}

	//Gọi hàm Update của GameObject
	CGameObject::Update(dt, listObjects);


	//Lưu objects có thể va chạm với PhantomBat
	vector<LPGAMEOBJECT> coObjects;


	//Lấy object có khả năng va chạm với PhantomBat
	GetCollisionObject(listObjects, coObjects, simon);


	//Kiểm tra va chạm của PhantomBat
	//Danh sách va chạm có thể có
	vector<LPCOLLISIONEVENT> coEvents;

	//Danh sách thực sự va chạm
	vector<LPCOLLISIONEVENT> coEventsResult;


	//Làm sạch để sử dụng
	coEvents.clear();


	//Tính toán các va chạm tiềm năng với danh sách các đối tượng có thể va chạm
	CalcPotentialCollisions(&coObjects, coEvents);


	//Nếu PhantomBat không va chạm với bất cứ Object nào cả
	if (coEvents.size() == 0)
	{
		if (state != ENEMY_DESTROYED) {
			//Update tọa độ của PhantomBat, cộng dồn tọa độ cũ với quãng đường đi được sau thời gian dt
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


		////Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Simon*>(e->obj))
			{
				//Xử lí khi va chạm với simon
				simon->LoseHP(PHANTOMBAT_ATK);	//Trừ HP của Simon
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




	////Luôn check vị trí của Simon
	float simonX, simonY;

	simon->GetPosition(simonX, simonY);


	////Đang sleep
	if (state == ENEMY_INACTIVE) {
		//Khi simon cách Phantombat khoảng > 200px thì active 
		if (simonX - x > PHANTOMBAT_DISTANCE_ACTIVE) {
			state = ENEMY_WORKING;
		}
	}

	////Đang fly
	if (state == ENEMY_WORKING) {

		//Tạo vùng bao xung quanh màn đấu boss. Tránh cho simon di chuyển ra khỏi vùng này
		if (!isCreateLimitRegion) {

			isCreateLimitRegion = true;

			playerManager->SetIsFightingBoss(true);
		}


		//Xử lí việc chờ đến trạng thái Bay
		WaitingForNextFlying(simon);


		//Xử lí việc chờ đên trạng thái Đứng yên
		WaitingForNextIdling();


		//Cập nhật trạng thái Bất tử 
		UpdateImmortal(ENEMY_IMMORTAL_TIME);
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}


void PhantomBat::WaitingForNextFlying(Simon* simon) {

	//Chưa ghi lại thời gian trước khi tấn công
	//1. Khi Phantombat Active lần đầu (chưa bay được 1 sequence nào)
	//2. Khi Phantombat Hoàn thành 1 State Flying (đang đứng tại vị tri Target)
	if (timeWaiting == -1) {

		//Bắt đầu ghi lại thời gian trước khi bay
		timeWaiting = GetTickCount();
	}



	//Lúc này PhantomBat đang đứng yên tại vị trí Target và sẵn sàng bay (do TimeCurrent - timeWaiting > TimeRequire)
	if (timeWaiting != -1 && GetTickCount() - timeWaiting > PHANTOMBAT_TIME_WAITING && timeFlying == -1) {

		//Ghi lại thời gian bắt đầu bay
		timeFlying = GetTickCount();


		//Nếu chưa có Type Flying nào
		if (SequenceFlyingType == -1) {

			//Random Type Flying
			SequenceFlyingType = rdManager->Random(LIST_RANDOM_PHANTOMBAT_FLYING_ATTACK);
		}

		//DebugOut(L"%d \n", SequenceFlyingType);



		//Kết quả Random ở trên sẽ quyết định PhantomBat tấn công Simon theo cách nào
		if (SequenceFlyingType == PHANTOMBAT_FLYING_ATTACK_TYPE1) {

			//Nếu chưa random loại cho trạng thái bay thứ 1
			if (SFType1_ATK == -1) {
				SFType1_ATK = rdManager->Random(LIST_RANDOM_PHANTOMBAT_FLYING_ATTACK_TYPE1);
				//DebugOut(L"%d \n", SFType1_ATK);
			}

			//Nếu cho phép đánh trúng simon thì dựa vào vị trí simon mà quyết định Type Fly
			//Nếu không thì sẽ giữ nguyên kết quả Random
			if (IsHitSimon()) {
				if (GetDirectionNeedAttack(simon) == PHANTOMBAT_FLY_STATE_LEFT) 
					SFType1_ATK = PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND1;

				if (GetDirectionNeedAttack(simon) == PHANTOMBAT_FLY_STATE_RIGHT_TOP)
					SFType1_ATK = PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND3;

				if (GetDirectionNeedAttack(simon) == PHANTOMBAT_FLY_STATE_RIGHT_BOTTOM)
					SFType1_ATK = PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND2;
			}


			//Bay theo trạng thái bay thứ nhất
			FlyingSquenceType1();
		}
		else {

			//Nếu chưa random loại cho trạng thái bay thứ 2
			if (SFType2_ATK == -1) {
				SFType2_ATK = rdManager->Random(LIST_RANDOM_PHANTOMBAT_FLYING_ATTACK_TYPE2);
				//DebugOut(L"%d \n", SFType2_ATK);
			}


			//Nếu cho phép đánh trúng simon thì dựa vào vị trí simon mà quyết định Type Fly
			//Nếu không thì sẽ giữ nguyên kết quả Random
			if (IsHitSimon()) {
				if (GetDirectionNeedAttack(simon) == PHANTOMBAT_FLY_STATE_LEFT)
					SFType2_ATK = PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND1;
				else
					SFType2_ATK = PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND2;
			}


			//Bay theo trạng thái bay thứ hai
			FlyingSquenceType2();


		}
	}


}



void PhantomBat::WaitingForNextIdling() {

	//Trong thời gian bay đến mục tiêu
	if (timeFlying != -1 && GetTickCount() - timeFlying < PHANTOMBAT_TIME_FLYING) {
		//DO NOTHING
	}
	//Nếu đang ở thời gian chờ
	else {

		//Nếu tấn công về phía simon thì cho tác dụng của trọng lực hướng lên => tạo độ cong chuyển động	
		StateFlyingCurrent == PHANTOMBAT_FLY_STATE_ATTACK ? isFallingUp = true : isFallingUp = false;


		//Hết thời gian bay đến mục tiêu vì hết thời gian cho phép bay
		if (timeFlying != -1 && GetTickCount() - timeFlying > PHANTOMBAT_TIME_FLYING) {
			
			//Chuẩn bị cho chu kỳ mới
			timeFlying = -1;
			timeWaiting = -1;

		}
		//Nếu đang chờ 0.5s
		//Chuyển trạng thái có thể đến StateFlyingCurrent == 1 => bị tác dụng của trọng lực
		//Nên phải khóa vận tốc = 0 để PB không bị kéo lên
		else {
			vx = vy = 0;
		}
	}

}




void PhantomBat::FlyingSquenceType1() {

	if (SFType1_ATK == -1) return;

	//Bay xuống vị trí tấn công
	if (StateFlyingCurrent == PHANTOMBAT_FLY_STATE_BEGIN) {
		vy = (SFTYPE1_STATE_BEGIN_Y - y) / PHANTOMBAT_TIME_FLYING;
		vx = (SFTYPE1_STATE_BEGIN_X - x) / PHANTOMBAT_TIME_FLYING;
		StateFlyingCurrent = PHANTOMBAT_FLY_STATE_ATTACK;
	}
	//Tấn công Simon
	else if (StateFlyingCurrent == PHANTOMBAT_FLY_STATE_ATTACK) {

		if (SFType1_ATK == PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND1) {
			vy = (SFTYPE1_STATE_ATTACK_KIND1_Y - y) / PHANTOMBAT_TIME_FLYING;
			vx = (SFTYPE1_STATE_ATTACK_KIND1_X - x) / PHANTOMBAT_TIME_FLYING;
		}
		else if (SFType1_ATK == PHANTOMBAT_FLYING_ATTACK_TYPE1_KIND2) {
			vy = (SFTYPE1_STATE_ATTACK_KIND2_Y - y) / PHANTOMBAT_TIME_FLYING;
			vx = (SFTYPE1_STATE_ATTACK_KIND2_X - x) / PHANTOMBAT_TIME_FLYING;
		}
		else {
			vy = (SFTYPE1_STATE_ATTACK_KIND3_Y - y) / PHANTOMBAT_TIME_FLYING;
			vx = (SFTYPE1_STATE_ATTACK_KIND3_X - x) / PHANTOMBAT_TIME_FLYING;
		}

		StateFlyingCurrent = PHANTOMBAT_FLY_STATE_GOBACK;
	}
	//Bay về vị trí ban đầu
	else if (StateFlyingCurrent == PHANTOMBAT_FLY_STATE_GOBACK) {
		vy = (ORIGINAL_COORDNINATE_Y - y) / PHANTOMBAT_TIME_FLYING;
		vx = (ORIGINAL_COORDNINATE_X - x) / PHANTOMBAT_TIME_FLYING;

		//Trả lại trạng thái 0. Để khi nhảy vào Type Flying mới thì nó sẽ chạy từ đầu state=0 -> state=2
		StateFlyingCurrent = PHANTOMBAT_FLY_STATE_BEGIN;

		//Reset sau khi hoàn thành 1 sequence => có thể random lại khi PB quay về vị trí ban đầu
		SFType1_ATK = -1;
		SequenceFlyingType = -1;

	}

}


void PhantomBat::FlyingSquenceType2() {

	if (SFType2_ATK == -1) return;


	//Bay xuống vị trí tấn công
	if (StateFlyingCurrent == PHANTOMBAT_FLY_STATE_BEGIN) {
		if (SFType2_ATK == PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND1) {
			vy = (SFTYPE2_STATE_BEGIN_KIND1_Y - y) / PHANTOMBAT_TIME_FLYING;
			vx = (SFTYPE2_STATE_BEGIN_KIND1_X - x) / PHANTOMBAT_TIME_FLYING;
		}
		else {
			vy = (SFTYPE2_STATE_BEGIN_KIND2_Y - y) / PHANTOMBAT_TIME_FLYING;
			vx = (SFTYPE2_STATE_BEGIN_KIND2_X - x) / PHANTOMBAT_TIME_FLYING;
		}

		StateFlyingCurrent = PHANTOMBAT_FLY_STATE_ATTACK;
	}
	//Tấn công Simon
	else if (StateFlyingCurrent == PHANTOMBAT_FLY_STATE_ATTACK) {
		if (SFType2_ATK == PHANTOMBAT_FLYING_ATTACK_TYPE2_KIND1) {
			vy = (SFTYPE2_STATE_ATTACK_KIND1_Y - y) / PHANTOMBAT_TIME_FLYING;
			vx = (SFTYPE2_STATE_ATTACK_KIND1_X - x) / PHANTOMBAT_TIME_FLYING;
		}
		else {
			vy = (SFTYPE2_STATE_ATTACK_KIND2_Y - y) / PHANTOMBAT_TIME_FLYING;
			vx = (SFTYPE2_STATE_ATTACK_KIND2_X - x) / PHANTOMBAT_TIME_FLYING;
		}

		StateFlyingCurrent = PHANTOMBAT_FLY_STATE_GOBACK;
	}
	//Bay về vị trí ban đầu
	else if (StateFlyingCurrent == PHANTOMBAT_FLY_STATE_GOBACK) {

		vy = (ORIGINAL_COORDNINATE_Y - y) / PHANTOMBAT_TIME_FLYING;
		vx = (ORIGINAL_COORDNINATE_X - x) / PHANTOMBAT_TIME_FLYING;

		//Trả lại trạng thái 0. Để khi nhảy vào Type Flying mới thì nó sẽ chạy từ đầu state=0 -> state=2
		StateFlyingCurrent = PHANTOMBAT_FLY_STATE_BEGIN;

		//Reset sau khi hoàn thành 1 sequence => có thể random lại khi PB quay về vị trí ban đầu
		SequenceFlyingType = -1;
		SFType2_ATK = -1;
	}
}



bool PhantomBat::IsHitSimon() {

	int result = rdManager->Random(LIST_RANDOM_PHANTOMBAT_HIT_SIMON);

	if (result == PHANTOMBAT_HIT_SIMON) {
		return true;
	}

	return false;
}


//Xác định hướng đánh của PhantomBat theo simon
int PhantomBat::GetDirectionNeedAttack(Simon* simon) {

	float left, top, right, bottom;

	simon->GetBoundingBox(left, top, right, bottom);


	if (left < PB_STATE_LEFT) return PHANTOMBAT_FLY_STATE_LEFT;

	if (top < PB_STATE_TOP) return PHANTOMBAT_FLY_STATE_RIGHT_TOP;

	return PHANTOMBAT_FLY_STATE_RIGHT_BOTTOM;
}


void PhantomBat::Render()
{
	SubWeaponManager* subWManager = SubWeaponManager::GetInstance();

	if (isImmortal && state != ENEMY_DESTROYED) {
		int alpha = rand() % 255 + 0;
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y, alpha);
	}
	else {
		animation_set->at(state)->Render(INSIDE_CAMERA, subWManager->isUseStopWatch, orientation, x, y);
	}

}




void PhantomBat::SetState(int state)
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
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}




void PhantomBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + PHANTOMBAT_TRANSPARENT_SPACE;
	top = y;
	right = left + PHANTOMBAT_WIDTH;
	bottom = top + PHANTOMBAT_HEIGHT;
}



//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với Zombie gán vào coObjects
void PhantomBat::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {

	if (!simon->getIsImmortal() && !StairManager::GetInstance()->GetIsSimonStandingOnStair()) {
		coObjects.push_back(simon);
	}

}



//Tính lượng máu còn lại của PhantomBat
void PhantomBat::Attacked(int lostHP)
{
	//Lấy máu từ playerManager
	int tempHP = PlayerManager::GetInstance()->GetBossHp();

	//Mất máu
	tempHP -= lostHP;

	if (tempHP <= 0) tempHP = 0;

	//PhantomBat không còn máu
	if (tempHP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);
	}

	//Cập nhật máu trên HUD
	PlayerManager::GetInstance()->SetBossHP(tempHP);



}




