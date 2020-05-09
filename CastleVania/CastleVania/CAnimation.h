#pragma once


#include "CAnimationFrame.h"

/*
	Manage all frames of an animation
*/
class CAnimation
{
	DWORD beginTime;						//Thời gian bắt đầu render của 1 animation
	DWORD lastFrameTime;					//Thời gian bắt đầu render của 1 frame
	int defaultTime;						//Thời gian mặc định của frame
	int currentFrame;						//index Frame hiện tại
	vector<LPANIMATION_FRAME> frames;		//Danh sách các frame của 1 animation

public:
	//Constructor
	//Thời gian mâc định của Frame là 0,1 giây
	CAnimation(int defaultTime = 100);


	//Thời gian bắt đầu render 1 animation
	void startCountTimeAnimation(DWORD t) { beginTime = t; }


	//Đã đủ thời gian dành cho animation này hay chưa????
	//Thời gian hiện tại  - thời gian bắt đầu so sánh với thời gian dành cho animation
	//return true nếu hết thời gian của animation này
	bool isFinishAnimation(DWORD timeForAnimation) { return GetTickCount() - beginTime >= timeForAnimation; }


	//Đã render đến frame cuối cùng hay chưa?????
	bool IsRenderingLastFrame() { return currentFrame == frames.size() - 1; }


	//Bắt đầu render từ frame đầu tiên
	void Reset() { currentFrame = -1; lastFrameTime=-1; }


	//Đặt current frame là 1 frame bất kì dựa vào index trong list Animation Frame
	void SetFrame(int x) { currentFrame = x; }


	//Lấy index frame hiện tại
	int GetCurrentFrame() { return currentFrame; }


	//Lấy kích thước của animation frame (số frame của animation này)
	int GetFramesSize() { return frames.size(); }


	//Thêm một frame vào Animation này dựa vào idSprite và thời gian của frame này
	void Add(string spriteID, DWORD time = 0);


	//Render animation
	void Render(int dependOnCam, int nx, float x, float y, int alpha = 255);


	// hàm dùng riêng để render MorningStar
	void RenderMorningStar(int idSimonFrame, int nx, float x, float y, int alpha = 255); 
};

typedef CAnimation* LPANIMATION;
