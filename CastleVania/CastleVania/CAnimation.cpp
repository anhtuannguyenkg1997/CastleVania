#include "CAnimation.h"
#include "define.h"
#include "CDebug.h"
#include "PlayerManager.h"

//Constructor
CAnimation::CAnimation(int defaultTime)
{
	this->defaultTime = defaultTime;
	lastFrameTime = -1;
	currentFrame = -1;
}



//Thêm một frame vào animation này bằng cách lấy ID của sprite của frame (đã lưu trong unorder_map sprites)
void CAnimation::Add(string spriteID, DWORD time)
{
	//Nếu chưa set thời gian cho frame này => lấy thời gian mặc định
	if (time == 0) time = this->defaultTime;

	//Lấy ra sprite dành cho frame này => truyền id Sprite vào
	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteID);

	//Tạo một animation_frame mới
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, time);

	//Đấy nó vào list các frame 
	//Push_back => thêm vào cuối vector
	frames.push_back(frame);
}




void CAnimation::Render(int accordingCam, bool isStop, int nx, float x, float y, int alpha)
{
	//Lấy thời gian hiện tại
	DWORD now = GetTickCount();


	//Bắt đầu render
	if (currentFrame == -1)
	{
		//Đặt frame render là frame đầu
		currentFrame = 0;

		//Thời gian bắt đầu render
		lastFrameTime = now;
	}
	else
	{
		//Lấy thời gian render của frame này (thời gian render yêu cầu)
		DWORD t = frames[currentFrame]->GetTime();

		//now - lastFrameTime = thời gian render thực tế của frame này
		//Nếu thời gian thực tế >= thời gian yêu cầu
		//=> bị quá thời gian dành cho animation
		//==> cần phải chuyển frame để kịp time cho animation


		if (now - lastFrameTime >= t) {

			//Nếu không bị đứng yên

				//Chuyển đến frame kế tiếp
			if (!isStop) {
				currentFrame++;
				//Đặt lại thời gian bắt đầu render
			}


			lastFrameTime = now; 

			//Nếu đã render đến frame cuối
			if (currentFrame >= frames.size())
			{
				//Đặt lại curFrame là frame đầu
				currentFrame = 0;
			}
		}
		//Nếu thời gian thực tế < thời gian yêu cầu
		//Thì không tiến hành chuyển frame
	}

	//Render frame hiện tại
	frames[currentFrame]->GetSprite()->Draw(accordingCam, nx, x, y, alpha);
}


///Hàm vẽ đặc biệt dùng để render Morning star
//CurrentID là ID của Frame Simon truyền vào (lúc đánh: có 3 frame)
//nx là hướng của MorningStar: 1 là từ trái qua phải, -1 là từ phải qua trái
void CAnimation::RenderMorningStar(int simonFrameID, int nx, float x, float y, int alpha)
{
	//Nếu MS đang ở Level, lúc này Animation có 12 Sprite (Mỗi 1 trạng thái roi có 4 Sprite)
	//1 trạng thái chiếm 4 Sprite liên tiếp nhau (0 -> 15), tạo ra hình ảnh lấp lánh cùa MS
	//Chọn ra 1 Sprite bất kì trong 4 Sprite để render
	if (PlayerManager::GetInstance()->GetMSLevel() == MS_LEVEL3) {
		
		//Render trạng thái 1 (Sprite 0 -> Sprite 3)
		//Tương tự với 2 trạng thái còn lại, ta có công thức sau để random ra 1 sprite bất kì
		int Min = simonFrameID*4;
		int Max = Min + 3;
		int r = Min + rand() % (Max + 1 - Min);

		//Vẽ từng Frame Morning star theo từng frame của Simon
		frames[r]->GetSprite()->Draw(INSIDE_CAMERA, nx, x, y, alpha);

	}
	//Level 1 và 2 chỉ có 3 Sprite
	else {
		//Vẽ từng Frame Morning star theo từng frame của Simon VD: frameSimon HIT = 0 thì frameMorningStar = 0 
		frames[simonFrameID]->GetSprite()->Draw(INSIDE_CAMERA, nx, x, y, alpha);
	}



}