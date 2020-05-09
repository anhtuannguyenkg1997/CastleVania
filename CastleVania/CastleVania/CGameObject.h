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
public:
	float x;		// Tọa độ 
	float y;

	float dx;		// Quảng đường
	float dy;

	float vx;		// Vận tốc
	float vy;

	int orientation; // Hướng của object (quay về chiều dương hay chiều âm)

	int state;		// Trạng thái của Object


	//Danh sách các Animation của GameObject này
	LPANIMATION_SET animation_set;

	DWORD dt;		//Thời gian

	bool active;	//Kích hoạt để hiển thị lên màn hình

	int itemStored;		// Loại Item mà object này mang (nếu không có là -1)

	bool isCreatedItem = false;		//Kiểm tra object này có tạo item chưa??? 


	CGameObject();

	//SETTER
	// Set vị trí (trong thế giới game)
	void SetPosition(float x, float y) { this->x = x; this->y = y; }

	//Set tốc độ (trên cả trục X và Y)
	void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }

	//Set trạng thái ()
	void SetState(int state) { this->state = state; }	

	//Set Hướng (OR_MINUS: quay về trục âm, OR_PLUS: quay về trục dương)
	void SetOrientation(int orientation) { this->orientation = orientation; }	

	//Set ID Item mà object này chứa đựng
	void SetIDItem(int itemStored) { this->itemStored = itemStored; }

	//Set active kích hoạt 
	void Active(bool active) { this->active = active; }	

	//Set trạng thái đã tạo item chưa????
	void setCreatedItem(bool x) { this->isCreatedItem = x; }	



	//GETTER
	//Lấy vị trí (trong thế giới game)
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }	

	//Lấy tốc độ (trên cả trục X và Y)
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }	

	//Lấy trạng thái
	int GetState() { return this->state; }	

	//Lấy hướng (OR_MINUS: quay về trục âm, OR_PLUS: quay về trục dương)
	int GetOrientation() { return this->orientation; }	

	//Lấy trạng thái object này đã tạo item chưa???

	//Lấy trạng thái kích hoạt
	bool isActive() { return this->active; }	




	//Xét va chạm
	void SweptAABB(
		float ml, float mt, float mr, float mb,
		float dx, float dy,
		float sl, float st,
		float sr, float sb,
		float& t, float& nx, float& ny);


	// Xét va chạm của 2 Object tĩnh
	bool BroadPhaseTestAABB(
		float left_a, float top_a, float right_a, float bottom_a,
		float left_b, float top_b, float right_b, float bottom_b
	);

	//Xử lí va chạm với 2 Object đang di chuyển
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);


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
		float& ny);

	//Thêm AnimationSet cho Object này
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }



	//Lấy vùng bao quanh Object (xét va chạm)
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;


	//Update object theo thời gian dt. Xét va chạm với listObject
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject = NULL);


	//Vẽ lên màn hình
	virtual void Render() = 0;

	//Vẽ vùng bao object
	void RenderBoundingBox();
};

