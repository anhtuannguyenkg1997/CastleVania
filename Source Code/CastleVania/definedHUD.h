#pragma once

//ID COMPONENT IN HUD
#define HUD_HP_PLAYER_ID	1
#define HUD_HP_PLAYER_LOSE_ID		2
#define HUD_HP_ENEMY_ID	3

#define HUD_FRAME_ID	4
#define HUD_SUBWEAPON_ID	5
#define HUD_ITEM_ID		6



//SUBWEAPON ON HUD
#define DAGGER_HUD			0
#define	AXE_HUD			    1
#define HOLY_WATER_HUD		2
#define BOOMERANG_HUD		3
#define	STOP_WATCH_HUD		4


#define DOUBLE_SHOT_HUD 0
#define TRIPPLE_SHOT_HUD 1



//BREAK SECTION
#define HUD_TEXTURE	1
#define HUD_SPRITE	2
#define HUD_HP	3
#define HUD_ITEM	4
#define HUD_SCORE 5


//CONST FOR HUD PRESENT
#define HP_SIZE	16

#define HUD_TEXT_SCORE_LEN	6
#define HUD_TEXT_TIME_LEN	4
#define HUD_TEXT_SUBWEAPONCOUNTER_LEN	2
#define HUD_TEXT_LIVE_LEN	2


//FONT REGION
#define HUD_LEFT	0
#define HUD_TOP	15
#define HUD_BOTTOM	80
#define HUD_RIGHT	512


//POSITION
//FRAME SHOW SUBWEAPON
#define SUBWEAPON_FRAME_X	288
#define SUBWEAPON_FRAME_Y	32


//SUBWEAPON POSITION
#define SUBWEAPON_INSIDE_X	305
#define SUBWEAPON_INSIDE_Y	38


//DOUBLE SHOT OR TRIPLE SHOT POSITION
#define ITEM_HUD_X	450
#define ITEM_HUD_Y	38


//HP SIMON POSITION
#define HP_PLAYER_POSITION_Y	31
#define HP_PLAYER_POSITION_X	105
#define HP_PLAYER_POSITION_X_SPACE	9

//HP BOSS POSITION
#define HP_BOSS_POSITION_Y	47
#define HP_BOSS_POSITION_X	106
#define HP_BOSS_POSITION_X_SPACE	9



//FINISH GAME
//SCORE DEFINITION
#define HUD_SCORE_PER_SEC 100
#define HUD_SCORE_PER_SMALLHEART 200


//TIME HUD DEFINITION
#define HUD_TIME_PLAY_SOUND_FINISH_GAME 5000
#define HUD_TIME_SUB_PER_TIMESEC 50
#define HUD_TIME_SUB_PER_SMALLHEART 300
#define HUD_TIME_SCORE_DISPLAY 500