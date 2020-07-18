#include "AttackingEffectManager.h"

AttackingEffectManager* AttackingEffectManager::_instance = NULL;

AttackingEffectManager* AttackingEffectManager::GetInstance()
{
	if (_instance == NULL) _instance = new AttackingEffectManager();
	return _instance;
}



//Hàm này được gọi liên tục bên playscene để update listEffect
void AttackingEffectManager::Update() {

	//Nếu không có effect nào cần render thì return
	if (listEffect.size() == 0) return;


	//Duyệt qua tất cả các effect đang có
	for (int i = 0; i < listEffect.size(); i++) {


		//Nếu effect nào đã quá thời gian render
		if (listEffect[i]->isFinish()) {

			//Xóa effect đó 
			DeleteEffect(listEffect[i]);

		}

	}

}


//Lấy vị trí render của Effect trả về 1 vector2
//nx,ny là hướng va chạm của SubWeapon
//left, top, right, bottom là boundingbox của vật bị va chạm bởi SW (Enemy, Candle,..)
D3DXVECTOR2 AttackingEffectManager::GetPositionEffectSweptAABB(int nx, int ny, float left, float top, float right, float bottom) {
	
	//Vị trí cần tìm
	float x, y;

	//XÉT TRÊN TRỤC X
	//Nếu hướng va chạm là bên trái của SW
	if (nx == COLLISION_DIRECTION_LEFT) {
		//x sẽ lấy right của vật bị va chạm (va chạm từ phải qua trái)
		x = right;
	}
	//Nếu hướng va chạm là bên phải của SW
	else if (nx == COLLISION_DIRECTION_RIGHT) {
		//x sẽ lấy left của vật bị va chạm (va chạm từ trái qua phải)
		x = left;
	}
	//Nếu không
	else {
		//x sẽ nằm ở giữa vật bị va chạm
		x = left + (right - left) / 2;
	}


	//XÉT TRÊN TRỤC Y
	//Nếu hướng va chạm là trên của SW
	if (ny == COLLISION_DIRECTION_TOP) {
		//y sẽ lấy bottom của vật bị va chạm (va chạm dưới lên trên)
		y = bottom;
	}
	//Nếu hướng va chạm là dưới của SW
	else if (nx == COLLISION_DIRECTION_BOTTOM) {
		//y sẽ lấy top của vật bị va chạm (va chạm trên xuống dưới)
		y = top;
	}
	//Nếu không
	else {
		//y sẽ nằm ở giữa vật bị va chạm
		y = top + (bottom - top) / 2;
	}


	//Trả về vector tọa độ 
	D3DXVECTOR2 result = D3DXVECTOR2(x, y);

	return result;
}




//Lấy vị trí render của Effect trả về 1 vector2
//nx là hướng của MorningStar
//left, top, right, bottom là boundingbox của vật bị va chạm bởi MorningStar (Enemy, Candle,..)
D3DXVECTOR2 AttackingEffectManager::GetPositionEffectBroadPhaseTest(int nx, float leftOther, float rightOther, float topObject) {
	
	//Vị trí cần tìm
	float x, y;

	//Nếu MS đang hướng về chiều dương => va chạm với left của otherObject
	if (nx == OR_PLUS) {

		//x sẽ lấy left của vật bị va chạm (va chạm từ trái qua phải)
		x = leftOther;
	}
	//Nếu MS đang hướng về chiều âm => va chạm với right của otherObject
	else {
		//x sẽ lấy right của vật bị va chạm (va chạm từ phải qua trái)
		x = rightOther;
	}

	//Top sẽ lấy theo top của MS
	y = topObject;


	//Trả về vector tọa độ 
	D3DXVECTOR2 result = D3DXVECTOR2(x, y);

	return result;
}


void AttackingEffectManager::Render() {
	
	//Duyệt qua list effect
	for (int i = 0; i < listEffect.size(); i++) {

		//Render từng effect
		listEffect[i]->Render();

	}
}



//Xóa một effect
void AttackingEffectManager::DeleteEffect(AttackingEffect* effect) {

	//https://stackoverflow.com/questions/26567687/how-to-erase-vector-element-by-pointer
	//Nếu ta biết rằng chỉ có một phần tử cần xóa
	//ta có thể sử dụng find để dừng ngay khi tìm thấy giá trị

	auto it = std::find(listEffect.begin(), listEffect.end(), effect);
	if (it != listEffect.end()) { listEffect.erase(it); }

}
