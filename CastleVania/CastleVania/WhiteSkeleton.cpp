#include "WhiteSkeleton.h"
#include "SubWeaponManager.h"
#include "CObjectCell.h"
#include "PlayerManager.h"
#include "EnemyRevival.h"
#include "CRandomManager.h"

WhiteSkeleton::WhiteSkeleton(float x, float y, int Type, bool isActive, int State, int allowJumpLeft, int allowJumpRight) : Enemy(x, y, Type, isActive, State)
{

	HP = WHITE_SKELETON_HP;
	ATK = WHITE_SKELETON_ATK;

	deltaVX = WHITE_SKELETON_DELTA_VX;

	this->allowJumpLeft = allowJumpLeft;
	this->allowJumpRight = allowJumpRight;
}



void WhiteSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Nếu WhiteSkeleton bị đánh bại và thời gian render hiệu ứng biến mất kết thúc
	if (state == ENEMY_DESTROYED && animation_set->at(state)->isFinishAnimation(DISAPPEAR_TIME) == true) {

		//WhiteSkeleton biến mất
		setActive(false);

		//Cho phép hồi sinh WhiteSkeleton
		isNeedRevival = true;
	}

	//Gọi hàm Update của GameObject
	CGameObject::Update(dt, listObjects);


	//Lưu objects có thể va chạm với WhiteSkeleton
	vector<LPGAMEOBJECT> coObjects;

	//Lấy những object có thể va chạm với WhiteSkeleton
	GetCollisionObject(listObjects, coObjects);


	//Simple falldown
	vy += WHITE_SKELETON_GRAVITY * dt;



	//Lấy các objects có khả năng va chạm với WS
	GetCollisionObject(listObjects, coObjects);




	//Kiểm tra va chạm của WhiteSkeleton
	//Danh sách va chạm có thể có
	vector<LPCOLLISIONEVENT> coEvents;

	//Danh sách thực sự va chạm
	vector<LPCOLLISIONEVENT> coEventsResult;


	//Làm sạch để sử dụng
	coEvents.clear();


	//Tính toán các va chạm tiềm năng với danh sách các đối tượng có thể va chạm
	CalcPotentialCollisions(&coObjects, coEvents);


	//Nếu WhiteSkeleton không va chạm với bất cứ Object nào cả
	if (coEvents.size() == 0)
	{
		//Không dùng StopWatch
		if (!SubWeaponManager::GetInstance()->isUseStopWatch) {
			//Update tọa độ của WhiteSkeleton, cộng dồn tọa độ cũ với quãng đường đi được sau thời gian dt
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		//Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj)) {


				//Va chạm cạnh trái của Brick thì cho phép WS bật ngược lại
				if (e->nx == COLLISION_DIRECTION_RIGHT) {
					
					//Vừa nhảy được 1 lần
					if (jumpState == WHITE_SKELETON_MIDDLE_JUMP) {
						vx = -WHITE_SKELETON_JUMPING_FORWARD_SPEED;
						vy = -WHITE_SKELETON_JUMPING_UP_SPEED;

						//Đổi trạng thái nhảy
						jumpState = WHITE_SKELETON_FINAL_JUMP;
					}

				}

				//Khi chạm đất sẽ xảy ra 2 TH
				//TH1: đang ở jumpState =1 thì phải cho nó bật về vị trí cũ
				//TH2: không phải jumpState = 1 thì luôn cho jumpState=0 để sẵn sàng bắn xương hoặc bật nhảy
				if (e->ny == COLLISION_DIRECTION_BOTTOM) {

					//Vừa nhảy được 1 lần
					if (jumpState == WHITE_SKELETON_MIDDLE_JUMP) {
						vx = WHITE_SKELETON_JUMPING_BACK_SPEED;
						vy = -WHITE_SKELETON_JUMPING_UP_SPEED;
						jumpState = WHITE_SKELETON_FINAL_JUMP; //Set cho khác 1 để sau khi xong update cho lần này thì nó sẽ nhảy xuống khối lệnh dưới
					}
					//Khi đứng trên mặt đất
					else {
						isStanding = true;
						jumpState = WHITE_SKELETON_NO_JUMP;
					}
	
				}



			}

		}
	}



	//Set hướng của WhiteSkeleton luôn quay về phía simon
	float simonX, simonY;

	simon->GetPosition(simonX, simonY);

	if (x > simonX) {
		orientation = OR_MINUS;
	}
	else {
		orientation = OR_PLUS;
	}


	//Đang đứng 
	if (isStanding  && state != ENEMY_DESTROYED) {

		//Biến thiên vận tốc theo OX
		vx += deltaVX;

		//Biến thiên vận tốc theo trục X, làm skeleton di chuyển qua lại 1 chỗ (khoảng cách rất nhỏ)
		//Khi đạt vận tốc đối đa, tức là quãng đường di chuyễn cực đại thì ta đổi chiều vx => WhiteSkeleton di chuyển ngược lại
		//Nhưng ta không thay đổi orientation => làm WhiteSkeleton tiến lùi liên tục
		if (vx > WHITE_SKELETON_MAX_VX || vx < -WHITE_SKELETON_MAX_VX) {
			vx = 0;
			deltaVX = -deltaVX;
		}



		//Đếm lùi đến thời gian tán công (nhảy tới hoặc bắn xương)
		if (timeStartAttack == -1) {
			timeStartAttack = GetTickCount();
		}

	}



	//Cho phép WhiteSkeleton nhảy hoặc bắn xương
	//WS chưa nhảy và thời gian timeBeforeJump đã hơn cần thiết
	if (jumpState == WHITE_SKELETON_NO_JUMP && 
		timeStartAttack != -1 &&
		GetTickCount() - timeStartAttack > WHITE_SKELETON_NEXT_JUMP_TIME &&
		state != ENEMY_DESTROYED) {
		

		//Xử lí trường hợp WS chỉ nên nhảy về 1 hướng
		//Nếu nhảy hướng ngược lại thì sẽ bị rơi xuống hố
		int allowToJump = false;

		if (orientation == OR_MINUS && allowJumpLeft==1 || orientation == OR_PLUS && allowJumpRight==1) allowToJump = true;


		//Random xem bắn xương hay nhảy tới phía trước
		int type = CRandomManager::GetInstance()->Random(LIST_RANDOM_WHITESKELETON_STATE);

		//Tạo Bone
		if (type == RD_WHITESKELETON_CREATEBONE) {
			isCreateBone = true;

			//Vẫn giữ nguyên trạng thái nhảy ban đầu
			jumpState = WHITE_SKELETON_NO_JUMP;
		}
		//Nhảy đến trước
		else if(type == RD_WHITESKELETON_JUMPING && allowToJump) {

			//Không còn chạm đất
			isStanding = false;

			//Nhảy lên
			vy = -WHITE_SKELETON_JUMPING_UP_SPEED;

			//Tùy vào hướng WS mà vx dương hay âm
			if (orientation == OR_PLUS) {
				vx = WHITE_SKELETON_JUMPING_FORWARD_SPEED;
			}
			else {
				vx = -WHITE_SKELETON_JUMPING_FORWARD_SPEED;
			}

			//Đổi trạng thái nhảy
			jumpState = WHITE_SKELETON_MIDDLE_JUMP;
		}


		//Cho phép lưu lại thời gian
		timeStartAttack = -1;


	}


	//Cho phép bắn xương
	if (isCreateBone && state != ENEMY_DESTROYED) {
		////Tạo vũ khí cho WhiteSkeleton
		bone = new Bone();

		//Vị trí đầu của WhiteSkeleton (nơi sẽ bắn ra vũ khí)
		bone->SetPosition(x, y);

		//Hướng trùng với hướng của WhiteSkeleton
		bone->SetOrientation(orientation);

		//Loại dynamic object (kiểm tra vị trí cell của object này liên tục)
		bone->SetType(DYNAMIC_OBJECT);

		//Đưa nó vào grid
		CObjectCell* objectCell = new CObjectCell(bone, x, y);

		//Không cho phép tạo Bone nữa
		isCreateBone = false;
	}




	//Kiểm tra trạng thái bất tử của WhiteSkeleton
	UpdateImmortal(ENEMY_IMMORTAL_TIME);


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}



//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với simon gán vào coObjects
void WhiteSkeleton::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects) {


	for (auto obj : listObjects)
	{
		if (dynamic_cast<Brick*>(obj))
			coObjects.push_back(obj);
	}


}



void WhiteSkeleton::Render()
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




void WhiteSkeleton::SetState(int state)
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

void WhiteSkeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + WHITE_SKELETON_WIDTH;
	bottom = top + WHITE_SKELETON_HEIGHT;

}

void WhiteSkeleton::GetBoundingBoxStart(float& left, float& top, float& right, float& bottom)
{
	left = positionStart.x;
	top = positionStart.y;
	right = left + WHITE_SKELETON_WIDTH;
	bottom = top + WHITE_SKELETON_HEIGHT;
}


//Tính lượng máu còn lại của WhiteSkeleton
void WhiteSkeleton::Attacked(int lostHP)
{
	Enemy::Attacked(lostHP);

	PlayerManager* playerManager = PlayerManager::GetInstance();

	//WhiteSkeleton không còn máu
	if (HP == 0) {
		//Trạng thái render hiệu ứng biến mất
		SetState(ENEMY_DESTROYED);

		//Set điểm cho người chơi khi tiêu diệt WhiteSkeleton
		int score = playerManager->GetScore();
		score += VAMPIREBAT_EXP;
		playerManager->SetScore(score);
	}
}