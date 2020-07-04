#include "MovingStair.h"

MovingStair::MovingStair(float x, float y, int Type, bool isActive, int State) : CGameObject(x, y, Type, isActive, State)
{
	vx = MOVINGSTAIR_SPEED;
	vy = 0.0f;
}


void MovingStair::Update(DWORD dt, vector<LPGAMEOBJECT>& listObjects, Simon* simon)
{
	//Gọi đến update của CGameObject
	CGameObject::Update(dt, listObjects);

	//Cập nhật vị trí solider theo trục X vì movingstair chỉ đi theo chiều ngang
	x += dx;



	//Nếu chạm đến vùng giới hạn trái hoặc phải thì ta đổi chiều vận tốc => di chuyển ngược lại
	if (x > movingRegionRight) {
		vx = -vx;
	}
	else if (x < movingRegionLeft) {
		vx = -vx;
	}


	//Lưu objects có thể va chạm với Simon
	vector<LPGAMEOBJECT> coObjects;


	GetCollisionObject(listObjects, coObjects, simon);



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
	if (coEvents.size() != 0)
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


		//Duyệt qua danh sách va chạm gần nhất
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//Lấy từng phần tử trong danh sách
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Simon*>(e->obj))
			{
				float simonX, simonY;
				simon->GetPosition(simonX, simonY);

				simonX += dx*2;

				simon->SetPosition(simonX, simonY);
			}

		}
	}




}



//Lấy từ danh sách listObject. tìm ra các vật có thể va chạm với HunchBack gán vào coObjects
void MovingStair::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects, Simon* simon) {

	coObjects.push_back(simon);

}

void MovingStair::Render()
{
	animation_set->at(state)->Render(INSIDE_CAMERA, false, NOT_FLIPPING, x, y);
}



void MovingStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + MOVINGSTAIR_WIDTH;
	b = t + MOVINGSTAIR_HEIGHT;
}


