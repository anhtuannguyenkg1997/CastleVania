#include "ObjectCreater.h"
#include "PlayerManager.h"
#include "HiddenObjectManager.h"

ObjectCreater* ObjectCreater::_instance = NULL;


ObjectCreater* ObjectCreater::GetInstance()
{
	if (_instance == NULL) _instance = new ObjectCreater();
	return _instance;
}




void ObjectCreater::CreateObject(vector<string> tokens, Simon*& simon, MorningStar*& morningStar, D3DXVECTOR2 simonPositionStart, int simonState, int simonOrientation) {

	PlayerManager* playerManager = PlayerManager::GetInstance();


	//Object Game nào cũng có position, type và animation set id
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
	{
		obj = new Simon();
		simon = (Simon*)obj;
		simon->SetPosition(simonPositionStart.x, simonPositionStart.y);
		simon->SetState(simonState);
		simon->SetOrientation(simonOrientation);

		break;
	}
	case OBJECT_TYPE_MORNINGSTAR:
	{
		obj = new MorningStar();
		morningStar = (MorningStar*)obj;
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float right = atof(tokens[4].c_str());
		float bottom = atof(tokens[5].c_str());
		int sceneTarget = atoi(tokens[6].c_str());
		int colCell = atoi(tokens[7].c_str());
		int rowCell = atoi(tokens[8].c_str());

		obj = new Portal(x, y, right, bottom, sceneTarget);
		portal = (Portal*)obj;
		portal->SetType(STATIC_OBJECT);
		portal->setActive(true);
		objectCell = new CObjectCell(portal, x, y, colCell, rowCell);

		break;
	}
	case OBJECT_TYPE_VAMPIREBAT:
	{
		int idItem = atof(tokens[4].c_str());
		int objectType = atof(tokens[5].c_str());
		int colCell = atof(tokens[6].c_str());
		int rowCell = atof(tokens[7].c_str());

		float simonPointX = atof(tokens[8].c_str());
		float simonPointY = atof(tokens[9].c_str());

		obj = new VampireBat(x, y, objectType, true, ENEMY_INACTIVE);
		vampireBat = (VampireBat*)obj;
		vampireBat->SetIDItem(idItem);
		vampireBat->SetSimonPointX(simonPointX);
		vampireBat->SetSimonPointY(simonPointY);
		objectCell = new CObjectCell(vampireBat, x, y, colCell, rowCell);

		break;
	}
	case OBJECT_TYPE_GHOST:
	{
		int idItem = atof(tokens[4].c_str());
		int objectType = atof(tokens[5].c_str());
		int colCell = atof(tokens[6].c_str());
		int rowCell = atof(tokens[7].c_str());
		int orientation = atoi(tokens[8].c_str());

		obj = new Ghost(x, y, objectType, true, ENEMY_INACTIVE);
		ghost = (Ghost*)obj;
		ghost->setActive(true);
		ghost->SetIDItem(idItem);
		ghost->SetOrientation(orientation);
		objectCell = new CObjectCell(ghost, x, y, colCell, rowCell);

		break;
	}
	case OBJECT_TYPE_BLACKKNIGHT:
	{
		int idItem = atof(tokens[4].c_str());
		int objectType = atof(tokens[5].c_str());
		int colCell = atof(tokens[6].c_str());
		int rowCell = atof(tokens[7].c_str());

		obj = new BlackKnight(x,y, objectType, true, ENEMY_WORKING);
		blackKnight = (BlackKnight*)obj;
		blackKnight->SetIDItem(idItem);
		objectCell = new CObjectCell(blackKnight, x, y, colCell, rowCell);
		break;

	}
	case OBJECT_TYPE_WHITE_SKELETON:
	{
		int idItem = atof(tokens[4].c_str());
		int objectType = atof(tokens[5].c_str());
		int colCell = atof(tokens[6].c_str());
		int rowCell = atof(tokens[7].c_str());
		int orientation = atof(tokens[8].c_str());
		int allowJumpLeft = atof(tokens[9].c_str());
		int allowJumpRight = atof(tokens[10].c_str());

		obj = new WhiteSkeleton(x, y, objectType, true, ENEMY_WORKING, allowJumpLeft, allowJumpRight);
		whiteSkeleton = (WhiteSkeleton*)obj;

		whiteSkeleton->SetIDItem(idItem);
		whiteSkeleton->SetOrientation(orientation);

		objectCell = new CObjectCell(whiteSkeleton, x, y, colCell, rowCell);
		break;

	}
	case OBJECT_TYPE_STAIR:
	{
		int objectType = atof(tokens[4].c_str());
		int colCell = atof(tokens[5].c_str());
		int rowCell = atof(tokens[6].c_str());
		int stairDirection = atof(tokens[7].c_str());
		int breakPoint = atof(tokens[8].c_str());

		obj = new Stair(x, y, objectType, true, 0);
		stair = (Stair*)obj;
		stair->SetState(stairDirection);
		stair->SetBreakPoint(breakPoint);

		objectCell = new CObjectCell(stair, x, y, colCell, rowCell);

		break;
	}
	case OBJECT_TYPE_MOVINGSTAIR:
	{
		int idItem = atof(tokens[4].c_str());
		int objectType = atof(tokens[5].c_str());
		int colCell = atof(tokens[6].c_str());
		int rowCell = atof(tokens[7].c_str());

		float mLeft = atof(tokens[8].c_str());
		float mRight = atof(tokens[9].c_str());

		obj = new MovingStair(x, y, objectType, true, 0);
		movingStair = (MovingStair*)obj;
		movingStair->SetIDItem(idItem);
		movingStair->SetMovingRegionLeft(mLeft);
		movingStair->SetMovingRegionRight(mRight);

		objectCell = new CObjectCell(movingStair, x, y, colCell, rowCell);

		break;
	}
	case OBJECT_TYPE_HIDDEN_OBJECT:
	{
		int isMoving = atoi(tokens[4].c_str());

		HiddenObjectManager::GetInstance()->CreateHiddenObject(x,y,ani_set_id, isMoving);
		break;
	}
	default:
	{
		D3DXVECTOR2 positionStart = D3DXVECTOR2(x, y);

		int idItem = atof(tokens[4].c_str());
		int objectType = atof(tokens[5].c_str());
		int colCell = atof(tokens[6].c_str());
		int rowCell = atof(tokens[7].c_str());


		CreateSimpleObject(object_type, obj, positionStart, idItem, objectType, colCell, rowCell);
	}
	}


	if (object_type != OBJECT_TYPE_HIDDEN_OBJECT) {
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
	}

}


void ObjectCreater::CreateSimpleObject(int object_type, CGameObject*& obj, D3DXVECTOR2 positionStart, int idItem, int objectType, int colCell, int rowCell) {

	PlayerManager* playerManager = PlayerManager::GetInstance();

	switch (object_type)
	{
	case OBJECT_TYPE_HUNCH_BACK:
		obj = new HunchBack(positionStart.x, positionStart.y, objectType, true, ENEMY_WORKING);
		hunchBack = (HunchBack*)obj;
		hunchBack->SetIDItem(idItem);
		objectCell = new CObjectCell(hunchBack, positionStart.x, positionStart.y, colCell, rowCell);
		break;
	case OBJECT_TYPE_RAVEN:
		obj = new Raven(positionStart.x, positionStart.y, objectType, true, ENEMY_INACTIVE);
		raven = (Raven*)obj;
		raven->SetOrientation(OR_MINUS);
		raven->SetIDItem(idItem);
		objectCell = new CObjectCell(raven, positionStart.x, positionStart.y, colCell, rowCell);
		break;
	case OBJECT_TYPE_PHANTOM_BAT:
		obj = new PhantomBat(positionStart.x, positionStart.y, objectType, true, ENEMY_INACTIVE);
		phantomBat = (PhantomBat*)obj;
		phantomBat->SetIDItem(idItem);
		objectCell = new CObjectCell(phantomBat, positionStart.x, positionStart.y, colCell, rowCell);
		break;
	case OBJECT_TYPE_ZOMBIE:
		obj = new Zombie(positionStart.x, positionStart.y, objectType, true, ENEMY_WORKING);
		zombie = (Zombie*)obj;
		zombie->SetOrientation(OR_MINUS);
		zombie->SetIDItem(idItem);
		objectCell = new CObjectCell(zombie, positionStart.x, positionStart.y, colCell, rowCell);
		break;
	case OBJECT_TYPE_TORCH:
		obj = new Torch(positionStart.x, positionStart.y, objectType, true, STATIC_OBJECT_ACTIVE);
		torch = (Torch*)obj;
		torch->SetIDItem(idItem);
		objectCell = new CObjectCell(torch, positionStart.x, positionStart.y, colCell, rowCell);
		break;
	case OBJECT_TYPE_CANDLE:
		obj = new Candle(positionStart.x, positionStart.y, objectType, true, STATIC_OBJECT_ACTIVE);
		candle = (Candle*)obj;
		candle->SetIDItem(idItem);
		objectCell = new CObjectCell(candle, positionStart.x, positionStart.y, colCell, rowCell);
		break;
	case OBJECT_TYPE_MOVINGSTAIR:
		obj = new MovingStair(positionStart.x, positionStart.y, objectType, true, 0);
		movingStair = (MovingStair*)obj;
		objectCell = new CObjectCell(brick, positionStart.x, positionStart.y, colCell, rowCell);
		break;
	case OBJECT_TYPE_BRICK:
		obj = new Brick(positionStart.x, positionStart.y, objectType, true, STATIC_OBJECT_ACTIVE);
		brick = (Brick*)obj;
		brick->SetIDItem(idItem);

		if (objectType == BOSS_BRICK) {
			brick->setActive(false);
		}
		else {
			brick->setActive(true);
		}
		objectCell = new CObjectCell(brick, positionStart.x, positionStart.y, colCell, rowCell);
		break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
}