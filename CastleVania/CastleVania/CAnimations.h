#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include <vector>

#include "CSprites.h"
#include "CAnimation.h"

//Quản lý toàn bộ animations trong game
class CAnimations
{
	//Thể hiện duy nhất của CAnimations
	static CAnimations * __instance;


	//Danh sách animation dùng trong game
	//id để phân biệt các animation với nhau và nó là duy nhất
	unordered_map<string, LPANIMATION> animations;

public:

	//Lấy thể hiện duy nhất của CAnimation
	static CAnimations* GetInstance();


	//Lấy Animation bằng cách truyền vào ID tương ứng
	LPANIMATION Get(string id);


	//Thêm một animation mới vào danh sách animations
	void Add(string id, LPANIMATION ani);


	//Làm sạch toàn bộ danh sách animations
	void Clear();


};







//Danh sách animation của toàn bộ game
typedef vector<LPANIMATION> CAnimationSet;


//Con trỏ trỏ đến danh sách này
typedef CAnimationSet* LPANIMATION_SET;


//Quản lý các animation dành riêng cho mỗi object
class CAnimationSets
{
	//Thể hiện duy nhất của CAnimationSets
	static CAnimationSets* __instance;


	//Danh sách animation của set này
	//Dựa vào id của animation, ta sẽ lấy được danh sách animation nằm trong set này 
	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();

	//Lấy thể hiện duy nhất của CAnimationSet
	static CAnimationSets* GetInstance();


	//Lấy ra danh sách animation tương ứng với id của nó trong set này
	//Hay 
	LPANIMATION_SET Get(unsigned int id);


	//Thêm một danh sách animation vào trong set này,
	//id là định danh duy nhất giúp lấy danh sách animation ra
	void Add(int id, LPANIMATION_SET ani);



	//Làm sạch danh sách animation_sets
	void Clear();


};




