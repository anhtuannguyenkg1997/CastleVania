#include "CCamera.h"
#include "CTileMap.h"

CCamera* CCamera::_instance = NULL;

CTileMap* tilemap = CTileMap::GetInstance();



//Set vị trí của camera
void CCamera::SetCameraPosition(D3DXVECTOR2 position)
{
	//Luôn dịch chuyển xuống so với gốc tọa độ 80px
	float t = position.y + 80;
	this->position = D3DXVECTOR2(position.x, t);
}


//Lấy thể hiện duy nhất của camera
CCamera* CCamera::GetInstance()
{
	if (_instance == NULL) _instance = new CCamera();
	return _instance;
}



//Set camera luôn chạy theo simon
void CCamera::Update(Simon* simon) {
	//Giữ cho simon luôn đi hơn một nửa so với kích thước camera
	if (simon->x > (CAMERA_WIDTH / 2) && //Simon đi hơn một nửa kích thước camera hoặc
		simon->x + CAMERA_WIDTH / 2 <= tilemap->GetMapWidth()) //Camera chưa chạm cạnh phải của map
	{
		//Dời camera đi để simon nằm giữa camera
		D3DXVECTOR2 cameraUpdatedPosition = D3DXVECTOR2(simon->x - CAMERA_WIDTH / 2, 0);

		//Set vị trí của camera
		SetCameraPosition(cameraUpdatedPosition);
	}



}



//Kiểm tra Object có nằm trong vùng Camera không
bool CCamera::checkObjectInCamera(LPGAMEOBJECT object)
{

	float obj_x, obj_y;
	object->GetPosition(obj_x, obj_y);

	return obj_x >= position.x && obj_x < position.x + CAMERA_WIDTH
		&& (obj_y+80) >= position.y && (obj_y+80) < position.y + CAMERA_HEIGHT;
}

