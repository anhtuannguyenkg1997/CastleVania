#include "CGameObject.h"

CGameObject::CGameObject()
{
	state = 0;
	x = y = 0;
	vx = vy = 0;
	orientation = OR_PLUS;					
	active = true;				
	idItem = -1;

}


CGameObject::CGameObject(float x, float y, int Type, bool isActive, int State)
{
	state = State;
	this->x = x;
	this->y = y;

	positionStart.x = x;
	positionStart.y = y;

	vx = vy = 0;
	orientation = OR_PLUS;	
	active = isActive;
	idItem = -1;
	type = Type;
}


//Vẽ vùng bao của GameObject
void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);

	//Xác định vùng bao ở đâu
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;


	//Vẽ đè lên object với độ trong suốt alpha =100
	CGame::GetInstance()->Draw(INSIDE_CAMERA, OR_MINUS, l, t, bbox, 0, 0, rect.right, rect.bottom, 100);
}





//Xét va chạm với hai đối tượng đang di chuyển
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		//Vật đang đứng yên
	float ml, mt, mr, mb;		//Vật đang di chuyển
	float t;					//Thời gian va chạm
	float nx, ny;				//Kết quả va chạm


	//Lấy vùng bao của vật đang đứng yên
	coO->GetBoundingBox(sl, st, sr, sb);

	//Xử lý với đối tượng đang xét đứng yên
	float svx, svy;				//Tốc độ của đối tượng mà ta giả sử đang đứng yên (trên trục x,y)
	coO->GetSpeed(svx, svy);	//Lấy tốc độ của đối tượng đứng yên


	//Tính toán quảng đường đi được của đối tượng đang đứng yên
	float sdx = svx * dt;
	float sdy = svy * dt;

	//Tính toán quảng đường đi được của đối tượng đang chuyển động so với vật đang đứng yên
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	//Lấy bounding box của vật đang di chuyển
	GetBoundingBox(ml, mt, mr, mb);


	//Gọi hàm kiểm tra va chạm
	CGame::GetInstance()->SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	//Tạo một sự kiện va chạm mới
	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, coO);
	return e;
}


//Kiểm tra 2 object đã va chạm với nhau từ trước hay chưa?????
bool CGameObject::BroadPhaseTestAABB(float objectLeft, float objectTop, float objectRight, float objectBottom, float otherLeft, float otherTop, float otherRight, float otherBottom) {
	return CGame::GetInstance()->BroadPhaseTestAABB(objectLeft, objectTop, objectRight, objectBottom, otherLeft, otherTop, otherRight, otherBottom);
}


//Tính toán các va chạm tiềm năng với danh sách các đối tượng có thể va chạm
//coObjects: danh sách các đối tượng có thể va chạm
//coEvents: danh sách các va chạm tiềm năng
void CGameObject::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	//Duyệt hết danh sách các object truyền vào
	for (UINT i = 0; i < coObjects->size(); i++)
	{

		//Kiểm tra va chạm cho từng object trong list
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));


		//Có va chạm
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);  //Thêm vào danh sách các vật va chạm tiềm năng
		else
			delete e;	//Xóa khỏi danh sách các vật có thể va chạm
	}

	//Sắp xếp các phần tử trong phạm vi [first, last] theo thứ tự tăng dần.
	//Hàm nhị phân chấp nhận hai phần tử trong phạm vi làm đối số và trả về giá trị có thể chuyển đổi thành bool
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}





//Lọc lại các va chạm
//Chỉ xử lí những va chạm gần nhất với đối tượng
void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents, //Danh sách va chạm tiềm năng
	vector<LPCOLLISIONEVENT>& coEventsResult, //Kết quả danh sách va chạm sau khi lọc
	float& min_tx, float& min_ty, //Thời gian va chạm nhỏ nhất trên trục X,Y
	float& nx, float& ny, //Hướng va chạm
	float& rdx, float& rdy
)
{
	//Khởi tạo giá trị min t ban đầu
	min_tx = 1.0f;
	min_ty = 1.0f;
	nx = 0.0f;
	ny = 0.0f;

	//Biến lưu vị trí phần tử va chạm gần nhất trên trục X và Y
	int min_ix = -1;
	int min_iy = -1;


	//Làm sạch danh sách để sử dụng
	coEventsResult.clear();


	//Duyệt qua danh sách các va chạm tiềm năng
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];	//Lấy va chạm thứ i trong danh sách

		//Tìm min t trên trục x để lấy đối tượng va chạm trên trục X
		if (c->t < min_tx && c->nx != 0) {			// (thời gian va chạm), (nx != 0 có va chạm theo trục x)
			min_tx = c->t;	//Lấy thời gian va chạm min trên trục x
			nx = c->nx;		//Lấy hướng va chạm trên trục x
			min_ix = i;		//Lấy phần tử va chạm gần nhất trong danh sách
			rdx = c->dx;
		}

		//Tìm min t trên trục y để lấy đối tượng va chạm trên trục Y
		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t;	//Lấy thời gian va chạm min trên trục y
			ny = c->ny;		//Lấy hướng va chạm trên trục y
			min_iy = i;		//Lấy phần tử va chạm gần nhất trong danh sách
			rdy = c->dy;
		}
	}

	//Thêm vào danh sách kết quả va chạm sau khi lọc
	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}



//Nếu đang trong trạng thái bất tử thì kiểm tra
//Nếu vượt quá thời gian bất tử thì set lại biến isImmortal = false
void CGameObject::UpdateImmortal(int timeImmortal){ 
	if (isImmortal) {
		if (GetTickCount() - beginTimeImmortal >= timeImmortal) {
			isImmortal = false;
		}
	}
}


void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> &coObject)
{
	//Gán thời gian dt
	this->dt = dt;

	//Cập nhật quảng đường đi được của Object này = vận tốc * thời gian
	dx = vx * dt;	
	dy = vy * dt;
}