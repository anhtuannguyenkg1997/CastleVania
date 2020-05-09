﻿#include "MorningStar.h"
#include "Torch.h"
#include "Solider.h"
#include "Candle.h"

//Constructor
MorningStar::MorningStar() : CGameObject()
{
	damage = MS_DAMAGE_LEVEL1;
}


//Xử lí khi có va chạm với MorningStar
void MorningStar::Update(DWORD dt, Simon* simon, vector<LPGAMEOBJECT> listObjects)
{
	// Cho phép Upgrade trạng thái của MorningStar
	if (simon->isUpgradeMorningStar == true)
	{
		//Đặt lại không cho phép Upgrade MorningStar
		simon->isUpgradeMorningStar = false;

		//Upgrade MorningStar
		UpgradeMorningStar();
	}

	// lấy vị trí của simon để set vị trí của MorningStar
	float simon_x, simon_y;

	//Lấy vị trí của Simon
	simon->GetPosition(simon_x, simon_y);


	//Set hướng của Morningstar, hướng này trùng với hướng của simon
	SetOrientation(simon->GetOrientation());

	//Set tọa độ của MorningStar. Nó sẽ phụ thuộc vào vị trí của Simon
	SetPosition(simon);


	//List object sẽ va chạm với MorningStar 
	vector<LPGAMEOBJECT> coObjects;

	// chỉ xét va chạm khi render tới sprite cuối cùng của simon (vung tay tới)
	//1. Simon đang ở trạng thái đánh
	//2. Render đến frame đánh cuối cùng cùng simon (vung tay về trước)
	if ((simon->GetState()==WHOPPING_SITTING || simon->GetState() == WHOPPING_STANDING)  &&
		simon->animation_set->at(simon->GetState())->IsRenderingLastFrame() == true
		)
	{
		//lúc này ta xét va chạm giữa morning star và các gameobject khác (có thể có). 
		//Clear để sử dụng
		coObjects.clear();


		//lấy danh sách vật có thể va chạm với MorningStar. Truyền vào coObjects
		GetCollisionObject(listObjects, coObjects);

	}


	//Xử lí va chạm
	for (UINT i = 0; i < coObjects.size(); i++)
	{
		LPGAMEOBJECT obj = coObjects.at(i);

		//Va chạm MorningStar và Torch
		if (dynamic_cast<Torch*>(obj))
		{
			Torch* e = dynamic_cast<Torch*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) 
			{
				e->SetState(TORCH_DESTROYED);
			}

		}

		//Va chạm MorningStar và Candle
		if (dynamic_cast<Candle*>(obj))
		{
			Candle* e = dynamic_cast<Candle*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->SetState(CANDLE_DESTROYED);
			}
		}

		//Va chạm MorningStar và Solider
		if (dynamic_cast<Solider*>(obj))
		{
			Solider* e = dynamic_cast<Solider*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) 
			{
				e->LoseHP(this->damage);
				Sleep(100); //Dừng lại đúng bằng thời gian render frame MorningStar (vung roi)
			}
		}
	}
}



//Lấy tất cả Object có khả năng va chạm với MorningStar từ listObjects
void MorningStar::GetCollisionObject(vector<LPGAMEOBJECT> listObjects, vector<LPGAMEOBJECT>& coObjects) {
	for (auto obj : listObjects)
	{
		if (dynamic_cast<Torch*>(obj) || dynamic_cast<Candle*>(obj))
			coObjects.push_back(obj);
		else if ((dynamic_cast<Solider*>(obj)) && obj->GetState() == ACTIVE)
			coObjects.push_back(obj);
	}
}




//Render Morning star. Truyền vào currentID
void MorningStar::Render(int currentFrameSimon)
{
	//Chỉ khi currentID >= 0 ta mới vẽ Morning star lên màn hình game
	// id state hiện tại cho MorningStar từ 1->3. Mà trong animation_set có index animation từ 0 trở đi
	//Nên ta phải trừ 1 mới lấy đúng animation cần thiết
	int stateRender = state - 1;
	if (currentFrameSimon >= 0)
		animation_set->at(stateRender)->RenderByID(currentFrameSimon, orientation, x, y);

	//Vẽ vùng bao Morning star
	//RenderBoundingBox();
}





//Kiểm tra va chạm giữa MorningStar và các Object khác
bool MorningStar::CheckCollision(float otherLeft, float otherTop, float otherRight, float otherBottom)
{
	//Biết lưu vị trí của MorningStar
	float msLeft,msTop,msRight,msBottom;

	//Lấy vị trí của MorningStar
	GetBoundingBox(msLeft, msTop, msRight, msBottom);


	//Xét va chạm giữa MS với Object khác. 2 object tĩnh
	return CGameObject::BroadPhaseTestAABB(msLeft, msTop, msRight, msBottom, otherLeft, otherTop, otherRight, otherBottom);
}


//Lấy vùng bao của MorningStar
//Dùng xét va chạm và RenderBoudningBox
void MorningStar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//Dời vùng bao MS xuống tầm tay của Simon
	//Vì nếu lấy top nó sẽ cao hơn MS
	top = y+12;

	bottom = top + MORNINGSTAR_HEIGHT;



	//Nếu MorningStar đang hướng về bên phải
	if (orientation == OR_PLUS) {
		//Tọa độ trái bằng tọa độ X của MorningStar + thêm khoảng cách đến roi trong Texture
		left = x + 74;
	}
	else { //Nếu MorningStar đang hướng về bên trái
		//Tọa độ trái bằng chính tọa độ X của MorningStar
		left = x;

	}

	//Nếu MorningStar đang ở level1 hoặc level2
	if (state != MS_LEVEL3) {
		//Tọa độ phải bằng tọa độ trái + chiều dài của MorningStar
		right = left + MORNINGSTAR_LEVEL1_2_WIDTH;
	}
	else {
		//Tọa độ phải bằng tọa độ trái + chiều dài của MorningStar
		right = left + MORNINGSTAR_LEVEL3_WIDTH;
	}
}


void MorningStar::SetPosition(Simon* simon) {
	float m_x, m_y; //Tọa độ của morningStar
	simon->GetPosition(m_x, m_y);

	//Set vị trí của MorningStar (khi Simon vung roi)
	if (state != MS_LEVEL3) { //LEVEL1, LEVEL2
		//Nếu MorningStar đang hướng về bên phải
		if (orientation == OR_PLUS) {
			//Trừ một đoạn 15px vẽ thêm vào simon
			m_x -= 15;
		}
		else {
			//Bằng kích thước của texture morningStar (level1, level2)
			//Trừ đi một đoạn bằng của tổng kích thước vẽ thêm (15px) và kích thước của simon width (60px) 
			m_x -= 41;
		}
	}
	else { //LEVEL 3
		//Nếu MorningStar đang hướng về bên phải
		if (orientation == OR_PLUS) {
			//Trừ một đoạn 15px vẽ thêm vào simon
			m_x -= 15;
		}
		else {
			//Bằng kích thước của texture morningStar (level3)
			//Trừ đi một đoạn bằng của tổng kích thước vẽ thêm (15px) và kích thước của simon width (60px) 
			m_x -= 77;
		}
	}


	//Simon đang ngồi
	if (simon->GetState() == SITTING || simon->GetState() == WHOPPING_SITTING)
		m_y += 15; //Dịch chuyển Morningstar xuống gần tầm tay của Simon khi ngồi


	//Set vị trí của MorningStar
	x = m_x;
	y = m_y;
}



//Upgrade level của MorningStar
void MorningStar::UpgradeMorningStar()
{
	//Level 1 => Level 2 => Level 3
	switch (state)
	{
	case MS_LEVEL1: {
		SetState(MS_LEVEL2); 
		this->damage = MS_DAMAGE_LEVEL2;
		break;
	}
	case MS_LEVEL2: {
		SetState(MS_LEVEL3); 
		this->damage = MS_DAMAGE_LEVEL3;
		break;
	}
	default:
		break;
	}

}