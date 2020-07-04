#pragma once
#include "CGame.h"
#include <unordered_map>
#include "Sound.h"

using namespace std;

class SoundManager
{
	unordered_map<int, Sound* > _ArrSound;
	static SoundManager* __Instance;


	void _ParseSection_SOUND_EFFECT(string line);
	void _ParseSection_SOUND_TRACK(string line);
public:
	//Có phát nhạc hay không???
	bool playSound =true;

	static SoundManager* GetInstance();
	 
	//Load data sound từ file txt
	void LoadResourceSound(LPCWSTR gameFile);

	//Phát sound với id tương ứng
	void Play(int id);

	//Kiểm tra xem sound có id đang phát hay không???
	bool isPlayingSound(int id);

	//Dừng một sound
	void Stop(int id);

	//Dừng tất cả các sound
	void StopAll();
};

