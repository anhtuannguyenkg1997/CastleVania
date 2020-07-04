#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <algorithm>

#include "CDebug.h"
#include "CTextures.h"
#include "CSprites.h"
#include "CAnimation.h"
#include "CAnimations.h"

#include "define.h"

using namespace std;


class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;



//Struct mô tả một va chạm
struct CCollisionEvent
{
	LPGAMEOBJECT obj;	//Thời gian va chạm
	float t, nx, ny;	//Kết quả của sự va chạm
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->obj = obj;
	}

	//Hàm để so sánh sự kiện hai vật va chạm dựa theo thời gian t
	//Chọn ra thời gian nhỏ hơn để lấy ra va chạm gần nhất với object
	//Trả về giá trị bool
	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};



class CGameObject
{
protected:
	float x;		// Tọa độ 
	float y;

	float dx;		// Quảng đường
	float dy;

	float vx;		// Vận tốc
	float vy;

	int orientation; // Hướng của object (quay về chiều dương hay chiều âm)

	int state;		// Trạng thái của Object


	DWORD dt;		//Thời gian

	bool active;	//=true để hiển thị lên màn hình

	int idItem;		// Loại Item mà object này mang (nếu không có là -1)

	int type;	//Static or Dynamic

	DWORD beginTimeImmortal; //Thời gian bắt đầu trạng thái bất tử

	bool isImmortal = false; //Có phải đang trong trạng thái bất tử không

	bool isDisappearByWeapon = false; //Object biến mất có phải là do Weapon hay không?? hay do khuất tầm camera (1 số object)

	D3DXVECTOR2 positionStart; //Vị trí ban đầu mà GameObject này xuất hiện từ đầu (dùng để hồi sinh gameObject)


public:

	bool isNeedRevival = false;

	//Danh sách các Animation của GameObject này
	LPANIMATION_SET animation_set;

	CGameObject();
	CGameObject(float x, float y,int Type, bool isActive,int State);

	////ATTRIBUTES FUNCTION


	//SETTER
	// Set vị trí (trong thế giới game)
	void SetPosition(float x, float y) { this->x = x; this->y = y; }

	//Set tốc độ (trên cả trục X và Y)
	void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }

	void SetVY(float vy) { this->vy = vy; }

	void SetVX(float vx) { this->vx = vx; }

	//Set trạng thái ()
	void SetState(int state) { this->state = state; }

	//Set Hướng (OR_MINUS: quay về trục âm, OR_PLUS: quay về trục dương)
	void SetOrientation(int orientation) { this->orientation = orientation; }

	//Set ID Item mà object này chứa đựng
	void SetIDItem(int id) { this->idItem = id; }

	//Set active kích hoạt 
	void setActive(bool active) { this->active = active; }

	//Xác định gameObject này biến mất do vấn đề gì (ra ngoài viewport hay bị tiêu diệt bởi weapon)
	void setIsDisappearByWeapn(bool value) { this->isDisappearByWeapon = value; }

	//Set loại object 
	void SetType(int type) { this->type = type; }

	//Set vị trí ban đầu của GameObject
	void SetPositionStart(D3DXVECTOR2 positionStart) { this->positionStart = positionStart; }

	//Thiết lập trạng thái bất tử 
	void SetImmortal() { this->beginTimeImmortal = GetTickCount(); isImmortal = true; }




	//GETTER
	//Lấy vị trí (trong thế giới game)
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }

	float GetX() { return this->x; }
	float GetY() { return this->y; }

	//Lấy tốc độ (trên cả trục X và Y)
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }	

	//Lấy trạng thái
	int GetState() { return this->state; }	

	//Lấy hướng (OR_MINUS: quay về trục âm, OR_PLUS: quay về trục dương)
	int GetOrientation() { return this->orientation; }	

	//Lấy trạng thái kích hoạt
	bool isActive() { return this->active; }	

	//Lấy ID item mà object này đang chứa
	int GetIDItem() { return this->idItem; }

	//Lấy loại object này
	int GetType() { return this->type; }

	//Lấy vị trí ban đầu của GameObject
	D3DXVECTOR2 GetPositionStart() { return this->positionStart; }

	//Lấy trạng thái bất tử
	bool getIsImmortal() { return this->isImmortal; }

	//Xác định gameObject này biến mất do vấn đề gì (ra ngoài viewport hay bị tiêu diệt bởi weapon)
	bool getIsDisappearByWeapon() { return this->isDisappearByWeapon; }




	////MAIN FUNCTION



	
	//Kiểm tra xem còn được bất tử hay không?
	void UpdateImmortal(int timeImmortal);


	//Kiểm tra va chạm với 2 Object đang di chuyển
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);

	bool BroadPhaseTestAABB(float objectLeft, float objectTop, float objectRight, float objectBottom, float otherLeft, float otherTop, float otherRight, float otherBottom);


	//Tính toán các va chạm tiềm năng từ danh sách các đối tượng có thể va chạm
	//Tạo ra một sự kiện va chạm mới
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	
	
	//Lấy ra va chạm gần nhất với đối tượng gameObject này
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx, 
		float& rdy);



	//Thêm Animation cho Object này
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }


	//Lấy vùng bao quanh Object (xét va chạm)
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;


	//Update object theo thời gian dt. Xét va chạm với object trong vùng viewport
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &coObject);


	//Vẽ lên màn hình
	virtual void Render() = 0;

	//Vẽ vùng bao object
	void RenderBoundingBox();
};

