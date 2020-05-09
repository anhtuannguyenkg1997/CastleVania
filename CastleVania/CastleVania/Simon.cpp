#include <fstream>
#include <string>

#include "SceneManager.h"

#include "Simon.h"
#include "Brick.h"
#include "Portal.h"
#include "Solider.h"


//Constructor
Simon::Simon() : CGameObject()
{
	score = 0;
	doubleShotOrTrippleShot = -1;
	subweaponCounter = 15;
	live = 3;
	subWeapon = -1;
	HP = 16;
}


//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với simon gán vào coObjects
void Simon::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects) {
	for (auto obj : listObjects)
	{
		if (dynamic_cast<Brick*>(obj) || dynamic_cast<Portal*>(obj) || dynamic_cast<Item*>(obj))
			coObjects.push_back(obj);
		else if (dynamic_cast<Solider*>(obj) && obj->GetState() == ACTIVE)
			coObjects.push_back(obj);
	}
}




//Xử lí va chạm giữa Simon với listObjects
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> listObjects)
{
	vector<LPGAMEOBJECT> coObjects;

	//Lấy object có khả năng va chạm với simon từ listObject (chỉ lấy trong vùng camera)
	GetCollisionObject(listObjects, coObjects);


	//Gọi hàm Update của GameObject
	CGameObject::Update(dt);

	// Simple fall down => không thì simon sẽ nhảy và không rơi
	vy += SIMON_GRAVITY * dt;



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


		//Chỉ xử lí những va chạm gần nhất với đối tượng
		//Truyền vào coEvents (danh sách va chạm tiềm năng)
		//Lấy ra coEventsResult va chạm sau khi lọc
		//Lấy ra Thời gian va chạm nhỏ nhất trên trục X,Y
		//Lấy ra Hướng va chạm
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);


		//Cập nhật vị trí của Simon khi có va chạm, Cần đẩy ra một khoảng nhất định, nếu không sẽ xuyên qua vật va chạm
		x += min_tx * dx + nx * 1.0f;
		y += min_ty * dy + ny * 0.1f;


		//Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			//Nếu là Brick (gạch)

			if (dynamic_cast<Brick*>(e->obj))
			{

				if (e->ny != 0)
				{

					//Chạm cạnh trên viên gạch
					if (e->ny == DIR_BOTTOM)
					{
						vy = 0;
						standing = true;
						falling = false;
					}
					//Nếu không chạm cạnh trên viên gạch (chạm trái, chạm phải, chạm dưới)
					else
						y += dy;
				}

			}

			if (dynamic_cast<Solider*>(e->obj)) {

				//Chạm vào Solider sẽ mất 1HP
				HP--;

				SetState(PUSHED);
			}

			//Nếu là Portal (chuyển cảnh)
			if (dynamic_cast<Portal*>(e->obj))
			{
				Portal* p = dynamic_cast<Portal*>(e->obj);

				//DebugOut(L"Switch to Scene %d", p->GetSceneId());

				SceneManager::GetInstance()->SwitchScene(p->GetSceneId());
			}

			if (dynamic_cast<Item*>(e->obj)) {
				Item* i = dynamic_cast<Item*>(e->obj);
				touchItem(i);

			}
		}

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

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

	return true;
}


void Simon::Render()
{
	//Render Simon dựa vào state của nó
	if (state == PUSHED) {
		int alpha = rand() % 255 + 0; //Nhấp nháy Simon khi chạm vào Enemy
		animation_set->at(state)->Render(DEPEND_ON_CAMERA, orientation, x, y, alpha);
	}
	else {
		//Render bình thường
		animation_set->at(state)->Render(DEPEND_ON_CAMERA, orientation, x, y, 255);
	}

	//RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STANDING:
		vx = 0;
		break;
	case WALKING:
		if (orientation == OR_PLUS) vx = SIMON_WALKING_SPEED;	//Đang hướng về chiều dương
		else vx = -SIMON_WALKING_SPEED;			//Đang hướng về chiều âm
		break;
	case JUMPING:
		standing = false;
		vy = -SIMON_JUMPING_SPEED;
		break;
	case SITTING:
		vx = 0;
		vy = 0;
		break;
	case WHOPPING_SITTING:
		animation_set->at(state)->Reset();
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	case WHOPPING_STANDING:
		animation_set->at(state)->Reset();
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	case UPGRADE_MS:
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	case PUSHED:
		vy = -SIMON_PUSHED_Y;
		if (orientation == OR_PLUS) vx = -SIMON_PUSHED_X;
		else vx = SIMON_PUSHED_X;
		animation_set->at(state)->Reset();
		animation_set->at(state)->startCountTimeAnimation(GetTickCount());
		break;
	default:
		break;
	}
}


//Lấy vùng thừa để đảm bảo khi simon vung roi, phần cánh tay thừa ở phía sau không đẩy simon ra phía trước làm giật tọa độ của simon trong 1 frame ảnh
//Lấy vùng bao của Simon
//X,Y chính là tọa đô cũa simon. Bao gồm những phần thừa cắt từ sprite , chỉ quan tâm đên left và right do
//Xử lí phần cánh tay thừa mà simon vung ra khi đánh roi
//Nên khi lấy vùng bao, ta phải trừ đi phần thừa đó đế lấy chính xác bouding của simon
void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Nếu simon đang quay sang phái 
	if (orientation == OR_PLUS) {
		left = x + 17;	//cạnh trái đến cánh tay của simon (phần cong) cộng đế dời vào sát
		right = left + SIMON_WIDTH - 29; //cạnh phải đến cánh tay của simon (phần thẳng) trừ đế dời vào sát
	}
	else { //Nếu simon đang quay sang trái 
		left = x + 15;	//cạnh trái đến cánh tay của simon (phần thẳng) cộng đế dời vào sát
		right = left + SIMON_WIDTH - 31; //cạnh phải đến cánh tay của simon (phần cong) trừ đế dời vào sát
	}


	//Simon ở trạng thái bình thường (BBOX mặc định bằng chiều cao của Simon 60px)
	if (state == STANDING || state == WHOPPING_STANDING || state == WALKING || state == UPGRADE_MS || state == PUSHED) {
		top = y;
		bottom = top + SIMON_HEIGHT;
	}
	else if (state == SITTING || state == WHOPPING_SITTING || state == JUMPING) { //Simon ở trạng thái đặc biệt (BBOX cần giảm xuống của Simon 47px- kích thước của simon khi ngồi)
		top = y + 14;
		bottom = top + SIMON_SIT_HEIGHT;
	}

}



//Xử lí khi Simon chạm vào Item
void Simon::touchItem(Item* item)
{
	//item biến mất
	item->active = false;

	int idItem = item->GetState();

	switch (idItem)
	{
	case ITEM_STOPWATCH:
		subWeapon = STOP_WATCH_HUD;
		break;
	case ITEM_DAGGER:
		subWeapon = DAGGER_HUD;
		break;
	case ITEM_AXE:
		subWeapon = AXE_HUD;
		break;
	case ITEM_HOLYWATER:
		subWeapon = HOLY_WATER_HUD;
		break;
	case ITEM_BOOMERANG:
		subWeapon = BOOMERANG_HUD;
		break;
	case ITEM_SMALLHEART:
		subweaponCounter += 1;		//Thêm 1 đơn vị vũ khí phụ
		break;
	case ITEM_BIGHEART:
		subweaponCounter += 5;		//Thêm 5 đơn vị vũ khí phụ
		break;
	case ITEM_ROSARY:
		break;
	case ITEM_INVISIBILITYPOTION:
		break;
	case ITEM_MORNINGSTAR:
		SetState(UPGRADE_MS); // Hiệu ứng đổi màu khi upgrade MorningStar
		 //Không cho Simon di chuyển
		vx = 0;
		vy = 0;
		//Bật cờ xác nhận cho upgrade MS. Để class MS xử lí phần upgrade này
		isUpgradeMorningStar = true;
		break;
	case ITEM_MONEYBAG_RED:
		score += 100;	//Thêm 100 điểm
		break;
	case ITEM_MONEYBAG_PURPLE:
		score += 400;	//Thêm 400 điểm
		break;
	case ITEM_MONEYBAG_WHITE:
		score += 700;	//Thêm 700 điểm
		break;
	case ITEM_MONEYBAG:
		score += 1000;	//Thêm 1000 điểm
		break;
	case ITEM_DOUBLESHOT:
		doubleShotOrTrippleShot = 0; //Phần tử đầu tiên trong itemList bên Player để render
		isGotDoubleShotItem = true;
		break;
	case ITEM_TRIPLESHOT:
		doubleShotOrTrippleShot = 1; //Phần tử thứ hai trong itemList bên Player để render
		isGotTripleShotItem = true;
		break;
	case ITEM_PORKCHOP:
		//Tăng HP cho simon thêm 3 đơn vị
		HP += 3;
		if (HP > HP_SIZE)
			HP = HP_SIZE;
		break;
	default:
		break;
	}
}


void Simon::LoseHP(int x)
{
	HP -= x;

	if (HP <= 0)
		HP = 0;
}

void Simon::AddHP(int x)
{
	HP += x;

	if (HP >= 16)
		HP = 16;
}