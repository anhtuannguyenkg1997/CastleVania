#pragma once
#include "CGame.h"
#include <mmsystem.h>
#include "DSutil.h"


class Sound
{
	CSound* wave;
	CSoundManager* dsound;

	bool isLoop = false; //Có lặp lại khi kết thúc sound hay không

public:
	Sound(LPTSTR filename, bool isLoop);

	void Play();
	void Stop();

	bool isPlaying();


};

