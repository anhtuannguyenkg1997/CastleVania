#pragma once
#include "CGame.h"
#include <mmsystem.h>
#include "DSutil.h"


class Sound
{
	//Từng sound riêng lẽ
	CSound* wave;

	//Instance list sound 
	CSoundManager* dsound;

	//Có lặp lại khi kết thúc sound hay không
	bool isLoop = false; 

public:
	//khởi tạo một sound
	Sound(LPTSTR filename, bool isLoop);

	//Phát sound
	void Play();

	//dừng sound
	void Stop();

	//kiểm tra sound có đang được phát hay không
	bool isPlaying();


};

