#pragma once

#include "CStandardized.h"
#include "definedHUD.h"


void StandardizedText(string& score, string& remainTime, string& subWeaponCounter, string& live) {

	while (score.length() < HUD_TEXT_SCORE_LEN) score = "0" + score;

	while (remainTime.length() < HUD_TEXT_TIME_LEN) remainTime = "0" + remainTime;

	while (subWeaponCounter.length() < HUD_TEXT_SUBWEAPONCOUNTER_LEN) subWeaponCounter = "0" + subWeaponCounter;

	while (live.length() < HUD_TEXT_LIVE_LEN) live = "0" + live;
}