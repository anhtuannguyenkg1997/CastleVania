#include "Sound.h"



Sound::Sound(LPTSTR filename, bool isLoop)
{
	dsound = new CSoundManager();
	dsound->Initialize(CGame::GetInstance()->GetWindowHandle(), DSSCL_PRIORITY);
	dsound->SetPrimaryBufferFormat(2, 22050, 16);

	HRESULT result = dsound->Create(&wave, filename);

	this->isLoop = isLoop;
}




//Kiểm tra sound đang được phát hay không???
bool Sound::isPlaying()
{
	return wave->IsSoundPlaying();
}


void Sound::Play()
{
	wave->Play(0, isLoop, 1L);
}



void Sound::Stop()
{
	wave->Stop();
}



