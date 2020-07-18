#include "LoadFileHelper.h"


void _ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) {
		return; // skip invalid lines
	}

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	string R = (tokens[2].c_str());
	string G = (tokens[3].c_str());
	string B = (tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), NULL);

	//DebugOut(L"Texture --> %d\n", texID);
}

void _ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	string ID = (tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);

	//DebugOut(L"Sprite --> %s\n", ToWSTR(ID).c_str());
}

void _ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	string ani_id = (tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		string sprite_id = (tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
		//DebugOut(L"--> %s\n", ToWSTR(sprite_id).c_str());
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
	//DebugOut(L"Animation --> %s\n", ToWSTR(ani_id).c_str());
}

void _ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		string ani_id = (tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);

		s->push_back(ani);

		//DebugOut(L"Animation Set--> %s\n", ToWSTR(ani_id).c_str());
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);

}








