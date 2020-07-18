#include <iostream>
#include <fstream>
#include <cstring>
#include "SoundManager.h"
#include "CUtils.h"
#include <atlstr.h>

SoundManager* SoundManager::__Instance = NULL;
Sound* sound;

SoundManager* SoundManager::GetInstance()
{
	if (__Instance == NULL)
		__Instance = new SoundManager();
	return __Instance;
}



//Load Sound track (nhạc nền)
void SoundManager::_ParseSection_SOUND_TRACK(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; //skip invalid line


	int id = atoi(tokens[0].c_str());	//id Scene


	CString Source = tokens[1].c_str();	//Sound file Path 

	LPTSTR path = Source.GetBuffer(Source.GetLength());


	sound = new Sound(path, true);

	_ArrSound[id] = sound;


}


//Load sound effect (nhạc hiệu ứng VD như tiếng quất roi (whopping)
void SoundManager::_ParseSection_SOUND_EFFECT(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; //skip invalid line


	int id = atoi(tokens[0].c_str());	//id Scene


	CString Source = tokens[1].c_str();	//Sound file Path 

	LPTSTR path = Source.GetBuffer(Source.GetLength());


	int isLoop = atoi(tokens[2].c_str());	//is loop this sound???


	//Có lặp lại hay không?? 1-có
	sound = new Sound(path, isLoop == 1 ? true : false );

	_ArrSound[id] = sound;

}





//Dừng tất cả các sound đang phát
void SoundManager::StopAll()
{
	for (auto& x : _ArrSound)
	{
		if (x.second->isPlaying())
			x.second->Stop();
	}
}


void SoundManager::LoadResourceSound(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SOUND_TRACK]") { section = GAME_SOUND_TRACK; continue; }
		if (line == "[SOUND_EFFECT]") { section = GAME_SOUND_EFFECT; continue; }


		switch (section)
		{
		case GAME_SOUND_TRACK: _ParseSection_SOUND_TRACK(line); break;
		case GAME_SOUND_EFFECT: _ParseSection_SOUND_EFFECT(line); break;
		}
	}
	f.close();



}



void SoundManager::Play(int id) {

	//Nếu cho phép playsound
	if(playSound)
		_ArrSound.at(id)->Play();
}



bool SoundManager::isPlayingSound(int id) {

	return _ArrSound.at(id)->isPlaying();

}


void SoundManager::Stop(int id) {

	_ArrSound.at(id)->Stop();

}
