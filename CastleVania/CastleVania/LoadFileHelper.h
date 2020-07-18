#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "CUtils.h"
#include "CTextures.h"
#include "CSprites.h"
#include "CAnimations.h"
#include "CAnimation.h"
#include "CGameObject.h"


using namespace std;





//LOAD DATA FROM FILE
void _ParseSection_TEXTURES(string line);
void _ParseSection_SPRITES(string line);
void _ParseSection_ANIMATIONS(string line);
void _ParseSection_ANIMATION_SETS(string line);



