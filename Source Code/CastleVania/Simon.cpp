#include <fstream>
#include <string>

#include "SceneManager.h"

#include "Simon.h"
#include "Brick.h"
#include "Portal.h"
#include "BlackKnight.h"
#include "SubWeaponManager.h"
#include "InvisibilityItemEffect.h"
#include "PlayerManager.h"
#include "CCamera.h"

#include "VampireBat.h"
#include "Ghost.h"
#include "HunchBack.h"
#include "WhiteSkeleton.h"
#include "Raven.h"
#include "Zombie.h"
#include "PhantomBat.h"
#include "Stair.h"
#include "StairManager.h"
#include "MovingStair.h"
#include "HiddenObjectManager.h"
#include "SoundManager.h"

//Constructor
Simon::Simon() : CGameObject()
{

}


//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với simon gán vào coObjects
void Simon::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects) {
	StairManager* manager = StairManager::GetInstance();
	InvisibilityItemEffect* invisiEffect = InvisibilityItemEffect::GetInstance();


	for (auto obj : listObjects)
	{
		//Simon đang trên thang thì không va chạm gạch
		if (!manager->GetIsSimonStandingOnStair()) {
			if (dynamic_cast<Brick*>(obj)) {
				Brick* brick = dynamic_cast<Brick*>(obj);
				if (brick->GetType()!=5)
					coObjects.push_back(obj);
			}
		}

		//Đang bất tử, dùng invisibilty hoặc đang trên thang thì không xét va chạm với Enemy
		if (!isImmortal && !manager->GetIsSimonStandingOnStair() && !invisiEffect->isInvisibility) {
			if (dynamic_cast<Enemy*>(obj) && 
				!dynamic_cast<Raven*>(obj)&& 
				!dynamic_cast<HunchBack*>(obj) && 
				!dynamic_cast<PhantomBat*>(obj) && 
				!dynamic_cast<Zombie*>(obj) 
				&& obj->GetState() == ENEMY_WORKING)
				coObjects.push_back(obj);
		}

		if (dynamic_cast<Portal*>(obj) || dynamic_cast<MovingStair*>(obj))
			coObjects.push_back(obj);
	}
}




//Xử lí va chạm giữa Simon với listObjects
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects)
{
	StairManager* stairManager = StairManager::GetInstance();
	InvisibilityItemEffect* invisiEffect = InvisibilityItemEffect::GetInstance();
	CCamera* camera = CCamera::GetInstance();
	PlayerManager* playerManager = PlayerManager::GetInstance();


	//Kiểm tra simon có rơi xuống hố hay không?
	if (!camera->checkObjectInCamera(this)) {
		SetState(DEAD);
		return;
	}


	//Kiểm tra trạng thái bất tử của simon
	UpdateImmortal(SIMON_IMMORTAL_TIME);

	//Kiểm tra có đang trong trạng thái invisibility
	if (invisiEffect->isInvisibility && invisiEffect->IsInvisibility()) {
		invisiEffect->isInvisibility = false;
		SetImmortal();
		SoundManager::GetInstance()->Play(SOUND_INVISIBLE_END);
	}



	//Lưu objects có thể va chạm với Simon
	vector<LPGAMEOBJECT> coObjects;

	//Lấy object có thể va chạm với simon từ listObject (chỉ lấy trong vùng camera)
	GetCollisionObject(listObjects, coObjects);


	//Gọi hàm Update của GameObject
	CGameObject::Update(dt, listObjects);


	//Không đứng trên bậc thang thì simon mới bị tác động bởi trọng lực
	if (!stairManager->GetIsSimonStandingOnStair())
	{
		//Simple falldown
		vy += SIMON_GRAVITY * dt;
	}


	//Kiểm tra va chạm của Simon
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Xét chung cho brick và moving stair
			if (dynamic_cast<Brick*>(e->obj) && e->obj->GetIDItem() == 1 && e->ny == COLLISION_DIRECTION_BOTTOM)
			{
				//Chạm cạnh trên viên gạch
				HiddenObjectManager::GetInstance()->isTouchedBrick = true;

			}

			//Nếu là Portal (chuyển cảnh)
			if (dynamic_cast<Portal*>(e->obj))
			{
				Portal* p = dynamic_cast<Portal*>(e->obj);

				//DebugOut(L"Switch to Scene %d", p->GetSceneId());
				string temp = SceneManager::GetInstance()->GetIDShow();
				SceneManager::GetInstance()->SetBeforeScene(temp);
				SceneManager::GetInstance()->SwitchScene(p->GetSceneId());

			}


			//Xử lý va chạm với Enemy
			if (dynamic_cast<Enemy*>(e->obj)) {

				Enemy* enemy = dynamic_cast<Enemy*>(e->obj); 

				LoseHP(enemy->GetATK());

				//Nếu hết HP
				if (playerManager->GetSimonHP() <= 0) {
					
					//Trạng thái Dead
					SetState(DEAD);

					//Nếu đang đấu boss mà thua
					if (dynamic_cast<PhantomBat*>(e->obj)) {

						//Tắt trạng thái đấu boss
						PlayerManager::GetInstance()->SetIsFightingBoss(false);
					}

				}
				else {
					SetImmortal();

					SetState(PUSHED);
				}
			}


		}





	}



	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

}



void Simon::Whopping() {

	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();

	subweaponManager->isHitBySubweapon = false;

	//Đứng và đánh
	if (GetState() == STANDING || GetState() == JUMPING)
	{
		SetState(WHOPPING_STANDING);
	}
	//Đứng trên thang hướng về chiều dương và đánh
	else if (GetState() == WALKING_UP_STAIR)
	{
		SetState(WALKING_UP_STAIR_WHOPPING);
	}
	//Đứng trên thang hướng về chiều âm và đánh
	else if (GetState() == WALKING_DOWN_STAIR)
	{
		SetState(WALKING_DOWN_STAIR_WHOPPING);
	}
	//Ngồi và đánh
	else if (GetState() == SITTING)
	{
		SetState(WHOPPING_SITTING);
	}



}



void Simon::UsingSubWeapon() {
	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();
	PlayerManager* manager = PlayerManager::GetInstance();


	//Subweapon đặc biệt, chỉ có thể dùng 1 lần ở 1 thời điểm
	if (manager->GetSubweaponID() == STOP_WATCH_HUD)
	{
		//Chưa cho phép tạo ra thêm subweapon
		if (!subweaponManager->isAvailableCreateStopWatch()) return;

		//Lúc này đang đánh bằng subweapon
		subweaponManager->createSubweapon = true;
		subweaponManager->isHitBySubweapon = true;
	}
	else {
		//Chưa cho phép tạo ra thêm subweapon
		if (!subweaponManager->isAvailableCreateOtherSubWeapon()) return;


		//Lúc này đã được tạo Subweapon
		subweaponManager->createSubweapon = true;
		subweaponManager->isHitBySubweapon = true;

		if (GetState() == STANDING) SetState(WHOPPING_STANDING);
		if (GetState() == SITTING) SetState(WHOPPING_SITTING);
		if (GetState() == JUMPING) SetState(WHOPPING_STANDING);
		if (GetState() == WALKING_UP_STAIR) SetState(WALKING_UP_STAIR_WHOPPING);
		if (GetState() == WALKING_DOWN_STAIR) SetState(WALKING_DOWN_STAIR_WHOPPING);
	}



}





//Kiểm tra đã render xong những animation đặc biệt chưa????
//Những animation này cần có thời gian để render. Nếu k xét thì nó sẽ diễn ra cực kì nhanh.
//Ví dụ hành động vung roi của simon.
//Những hành động này cần phải có 1 thời gian nhất định.Nếu FinishAnimation thì ta mới cho phép các sự kiện từ bàn phím
bool Simon::isFinishRenderAnimation() {
	if (state == UPGRADE_MS && animation_set->at(UPGRADE_MS)->isFinishAnimation(UPGRADING_TIME) == false)
		return false;

	if (state == WHOPPING_STANDING && animation_set->at(WHOPPING_STANDING)->isFinishAnimation(WHOPPING_TIME) == false)
		return false;

	if (state == WHOPPING_SITTING && animation_set->at(WHOPPING_SITTING)->isFinishAnimation(WHOPPING_TIME) == false)
		return false;

	if (state == PUSHED && animation_set->at(PUSHED)->isFinishAnimation(PUSHING_TIME) == false)
		return false;

	if (state == WALKING_UP_STAIR && animation_set->at(WALKING_UP_STAIR)->isFinishAnimation(WALKING_ON_STAIR_TIME) == false)
		return false;

	if (state == WALKING_DOWN_STAIR && animation_set->at(WALKING_DOWN_STAIR)->isFinishAnimation(WALKING_ON_STAIR_TIME) == false)
		return false;

	if (state == WALKING_UP_STAIR_WHOPPING && animation_set->at(WALKING_UP_STAIR_WHOPPING)->isFinishAnimation(WHOPPING_TIME) == false)
		return false;

	if (state == WALKING_DOWN_STAIR_WHOPPING && animation_set->at(WALKING_DOWN_STAIR_WHOPPING)->isFinishAnimation(WHOPPING_TIME) == false)
		return false;

	return true;
}


void Simon::Render()
{

	if (isImmortal) { //Trạng thái bất tử
		int alpha = rand() % 255 + 0;
		animation_set->at(state)->Render(INSIDE_CAMERA, false, orientation, x, y, alpha);
	}
	else if (InvisibilityItemEffect::GetInstance()->isInvisibility) { //Trạng thái vô hình 
		animation_set->at(state)->Render(INSIDE_CAMERA, false, orientation, x, y, 0);
	}
	else {//Nếu không Render bình thường	
		animation_set->at(state)->Render(INSIDE_CAMERA, false, orientation, x, y, 255);
	}

}



void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	StairManager* stairManager = StairManager::GetInstance();

	switch (state)
	{
	case STANDING:
		//Simon đứng im
		vx = 0;
		break;
	case WALKING:
		if (orientation == OR_PLUS)
			vx = SIMON_WALKING_SPEED;	//Đang hướng về chiều dương
		else
			vx = -SIMON_WALKING_SPEED;			//Đang hướng về chiều âm
		break;
	case JUMPING:
		stairManager->SetIsSimonStandingOnStair(false);	//Nếu không simon đứng trên thang và nhảy thì sẽ k rơi do vy = 0
		//standing = false; //Trạng thái simon không còn va chạm với brick
		vy = -SIMON_JUMPING_SPEED; //Vận tốc âm để simon bay lên
		animation_set->at(state)->Reset(); //Bắt đàu render lại từ frame đầu
		animation_set->at(state)->startCountTimeAnimation(GetTickCount()); //Bắt đầu tính thời gian chờ cho animation này
		break;
	case SITTING:
		//Simon đứng im
		vx = 0;
		break;
	case WHOPPING_STANDING:
	case WHOPPING_SITTING:
		animation_set->at(state)->Reset(); //Bắt đàu render lại từ frame đầu
		animation_set->at(state)->startCountTimeAnimation(GetTickCount()); //Bắt đầu tính thời gian chờ cho animation này
		break;
	case UPGRADE_MS:
		animation_set->at(state)->startCountTimeAnimation(GetTickCount()); //Bắt đầu tính thời gian chờ cho animation này
		break;
	case PUSHED:
		SoundManager::GetInstance()->Play(SOUND_SIMON_PUSHED);
		//Simon hướng về chiều dương
		if (orientation == OR_PLUS)
			vx = -SIMON_PUSHED_SPEED; //Cần đẩy simon về chiều âm
		else //Simon hướng về chiều âm
			vx = SIMON_PUSHED_SPEED; //Cần đẩy simon về chiều dương
		animation_set->at(state)->Reset(); //Bắt đàu render lại từ frame đầu
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());//Bắt đầu tính thời gian chờ cho animation này
		break;
	case DEAD:
		vx = 0;
		timeDied = GetTickCount(); //Đếm thời gian từ lúc simon chết (render simon chết và chờ 1 TG để load lại màn chơi)
		break;
	case WALKING_UP_STAIR:
		if (orientation == OR_PLUS) //Hướng về chiều dương
			vx = SIMON_VX_ON_STAIR;	//Vận tốc dương
		else						//Hướng về chiều âm
			vx = -SIMON_VX_ON_STAIR;	//Vận tốc âm

		vy = -SIMON_VY_ON_STAIR;	//Đi lên nên vận tốc trên trục Y phải âm
		animation_set->at(state)->Reset();	//Bắt đàu render lại từ frame đầu
		animation_set->at(state)->startCountTimeAnimation(GetTickCount()); //Bắt đầu tính thời gian chờ cho animation này
		break;
	case WALKING_DOWN_STAIR:
		if (orientation == OR_PLUS)		//Hướng về chiều dương
			vx = SIMON_VX_ON_STAIR;	//Vận tốc dương
		else							//Hướng về chiều âm
			vx = -SIMON_VX_ON_STAIR;	//Vận tốc âm

		vy = SIMON_VY_ON_STAIR;		//Đi xuống nên vận tốc trên trục Y phải dương
		animation_set->at(state)->Reset();//Bắt đàu render lại từ frame đầu
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());//Bắt đầu tính thời gian chờ cho animation này
		break;
	case WALKING_UP_STAIR_WHOPPING:
	case WALKING_DOWN_STAIR_WHOPPING:
		animation_set->at(state)->Reset();//Render lại từ frame đầu tiên
		animation_set->at(state)->startCountTimeAnimation(GetTickCount()); //Bắt đầu tính thời gian chờ cho animation này
		break;
	default:
		break;
	}
}






//Lấy vùng thừa để đảm bảo khi simon vung roi, phần cánh tay thừa ở phía sau không đẩy simon ra phía trước làm giật tọa độ của simon trong 1 frame ảnh
//X,Y chính là tọa đô cũa simon. Bao gồm những phần thừa cắt từ sprite , chỉ quan tâm đên left và right do
//Xử lí phần cánh tay thừa mà simon vung ra khi đánh roi
//Nên khi lấy vùng bao, ta phải trừ đi phần thừa đó đế lấy chính xác bouding của simon
void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + SIMON_REDUNDANT_HAND;	//cạnh trái đến cánh tay của simon cộng đế dời vào sát
	right = left + SIMON_WIDTH - SIMON_REDUNDANT_ELBOW; //cạnh phải đến khuỷu tay của simon trừ đế dời vào sát

	//Simon ở trạng thái bình thường (mặc định bằng chiều cao của Simon 60px)
	if (state == STANDING || state == WHOPPING_STANDING || state == WALKING || state == UPGRADE_MS || state == PUSHED || state == WALKING_UP_STAIR || state == WALKING_DOWN_STAIR || state == WALKING_DOWN_STAIR_WHOPPING || state == WALKING_UP_STAIR_WHOPPING) {
		top = y;
		bottom = top + SIMON_HEIGHT;
	}
	//Simon ở trạng thái NGỒI (BBOX cần giảm xuống của Simon 47px- kích thước của simon khi ngồi)
	else if (state == SITTING || state == WHOPPING_SITTING || state == JUMPING) {
		top = y + (SIMON_HEIGHT - SIMON_SIT_HEIGHT);
		bottom = top + SIMON_SIT_HEIGHT;
	}

}












void Simon::LoseHP(int x)
{
	PlayerManager* manager = PlayerManager::GetInstance();

	int temp = manager->GetSimonHP();
	temp -= x;

	if (temp <= 0) {
		manager->SetSimonHP(0);
		SetState(DEAD);
	}
	else {
		manager->SetSimonHP(temp);
	}


}

void Simon::AddHP(int x)
{
	PlayerManager* manager = PlayerManager::GetInstance();

	int temp = manager->GetSimonHP();
	temp += x;

	if (temp >= HP_SIZE) {
		manager->SetSimonHP(HP_SIZE);
	}
	else {
		manager->SetSimonHP(temp);
	}

}