#include "StopWatchSubWeapon.h"


StopWatchSubWeapon* StopWatchSubWeapon::_instance = NULL;


StopWatchSubWeapon* StopWatchSubWeapon::GetInstance()
{
	if (_instance == NULL) _instance = new StopWatchSubWeapon();
	return _instance;
}