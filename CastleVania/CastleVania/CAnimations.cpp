#include "CAnimations.h"
#include "CDebug.h"
#include "CUtils.h"

CAnimationSets* CAnimationSets::__instance = NULL;

CAnimations* CAnimations::__instance = NULL;


//Lấy thể hiện duy nhất của CAnimations
CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}


//Thêm một animation mới vào danh sách animations
void CAnimations::Add(string id, LPANIMATION ani)
{
	animations[id] = ani;
}


//Lấy animation từ danh sách animations dựa vào ID của nó
LPANIMATION CAnimations::Get(string id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %s\n", ToWSTR(id).c_str());
	return ani;
}


//Làm sạch danh sách animations
void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}












CAnimationSets::CAnimationSets()
{

}


//Lấy thể hiện duy nhất của CAnimationSet
CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}



//Lấy danh sách animation 
LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}

void CAnimationSets::Clear()
{
	for (auto x : animation_sets)
	{
		LPANIMATION_SET ani = x.second;
		delete ani;
	}


	animation_sets.clear();

	//DebugOut(L"[CLeared] Size AnimationSet: %d \n", animation_sets.size());
}