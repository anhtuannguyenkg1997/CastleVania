#pragma once

#include "CGameObject.h"
#include "Simon.h"

class Portal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
public:
	Portal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
};

