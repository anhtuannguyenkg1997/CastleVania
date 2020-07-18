#include "AttackingEffect.h"
#include "CDebug.h"
#include "AttackingEffectManager.h"


AttackingEffect::AttackingEffect(D3DXVECTOR2 position) {
	this->position = position;

	//Set Animation
	//Lấy Animation set trong danh sách AnimationSet
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	//Dựa vào ID 
	LPANIMATION_SET ani_set = animation_sets->Get(OBJECT_TYPE_ATTACKING_EFFECT);

	//Set animationSet cho Object này
	animation_set = ani_set;


	//Khi tạo object đồng thời bắt đầu đếm thời gian bắt đầu effect
	timeStartEffect = GetTickCount();

	//Đưa nó vào list Effect để quản lý
	AttackingEffectManager::GetInstance()->AddListEffect(this);
}



void AttackingEffect::Render() {
	animation_set->at(ACTIVE_EFFECT)->Render(INSIDE_CAMERA, false, OR_PLUS, position.x, position.y);
}



