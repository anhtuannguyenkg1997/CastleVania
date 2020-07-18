#include "InvisibilityItemEffect.h"


InvisibilityItemEffect* InvisibilityItemEffect::_instance = NULL;


InvisibilityItemEffect* InvisibilityItemEffect::GetInstance()
{
	if (_instance == NULL) _instance = new InvisibilityItemEffect();
	return _instance;
}