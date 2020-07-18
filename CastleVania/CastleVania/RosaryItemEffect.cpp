#include "RosaryItemEffect.h"

RosaryItemEffect* RosaryItemEffect::_instance = NULL;



RosaryItemEffect* RosaryItemEffect::GetInstance()
{
	if (_instance == NULL) _instance = new RosaryItemEffect();
	return _instance;
}