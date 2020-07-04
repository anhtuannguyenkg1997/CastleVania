#include "CCamera.h"
#include "CTileMap.h"
#include "Simon.h"
#include "PlayerManager.h"

CCamera* CCamera::_instance = NULL;

CTileMap* tilemap = CTileMap::GetInstance();



//Set vị trí của camera
void CCamera::SetCameraPosition(D3DXVECTOR2 position)
{
	//Luôn dịch chuyển xuống so với gốc tọa độ bằng bottom HUD
	//=> dư là 1 phần trên để vẽ HUD
	float t = position.y + HUD_BOTTOM;
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
	PlayerManager* playerManager = PlayerManager::GetInstance();

	float simonPosX, simonPosY;

	//Lấy vị trí của Simon
	simon->GetPosition(simonPosX, simonPosY);


	//Nếu không đấu boss thì camera mới đi theo simon
	if (!playerManager->GetIsFightingBoss()) {


		//Giữ cho simon luôn đi hơn một nửa so với kích thước camera
		if (simonPosX > (CAMERA_WIDTH / 2) && //Simon đi hơn một nửa kích thước camera hoặc
			simonPosX + CAMERA_WIDTH / 2 <= tilemap->GetMapWidth()) //Camera chưa chạm cạnh phải của map
		{
			//Dời camera đi để simon nằm giữa camera
			D3DXVECTOR2 cameraUpdatedPosition = D3DXVECTOR2(simonPosX - CAMERA_WIDTH / 2, 0);

			//Set vị trí của camera
			SetCameraPosition(cameraUpdatedPosition);
		}
	}


}



//Kiểm tra Object có nằm trong vùng Camera không
bool CCamera::checkObjectInCamera(LPGAMEOBJECT object)
{
	float left, top, right, bottom;
	object->GetBoundingBox(left, top, right, bottom);


	//Nếu simon rơi xuống vùng dưới do rớt xuống hố (bottom) thì mới xét nó ra ngoài camera => simon dead
	if (dynamic_cast<Simon*>(object)) {
		return (top + HUD_BOTTOM) < position.y + CAMERA_HEIGHT;
	}
	//Nếu không phải là simon thì xét bình thường dựa vào 4 vùng (left,top,right,bottom) của camera
	else {
		return right > position.x && left < position.x + CAMERA_WIDTH
			&& (bottom + HUD_BOTTOM) >= position.y && (top + HUD_BOTTOM) < position.y + CAMERA_HEIGHT;
	}


}



//Kiểm tra Object có nằm trong vùng Camera không
bool CCamera::checkObjectInCameraRevival(Enemy* enemy)
{
	float left, top, right, bottom;
	enemy->GetBoundingBoxStart(left, top, right, bottom);


	return right > position.x && left < position.x + CAMERA_WIDTH
		&& (bottom + HUD_BOTTOM) >= position.y && (top + HUD_BOTTOM) < position.y + CAMERA_HEIGHT;



}
