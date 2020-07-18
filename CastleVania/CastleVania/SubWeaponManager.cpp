#include "SubWeaponManager.h"
#include "CObjectCell.h"
#include "PlayerManager.h"

#include "DaggerSubWeapon.h"
#include "AxeSubWeapon.h"
#include "BoomerangSubWeapon.h"
#include "HolyWaterSubWeapon.h"
#include "StopWatchSubWeapon.h"
#include "SoundManager.h"

SubWeaponManager* SubWeaponManager::_instance = NULL;

SubWeaponManager* SubWeaponManager::GetInstance()
{
	if (_instance == NULL) _instance = new SubWeaponManager();
	return _instance;
}




void SubWeaponManager::Update(Simon* simon, DWORD dt) {

	if (simon == NULL) return;

	PlayerManager* manager = PlayerManager::GetInstance();


	//Xử lí khi đang ăn Item StopWatch StopWatch
	if (manager->GetSubweaponID() == STOP_WATCH_HUD) {


		//Kiểm tra StopWatch còn tác dụng hay không
		CheckStopWatchOverTime();


		//Chưa cho phép tạo StopWatch
		if (!createSubweapon) return;


		//Bắt đầu đếm thời gian dùng StopWatch
		StopWatchSubWeapon::GetInstance()->StartStopWatch(GetTickCount());

		//Tính sổ subweaponCounter còn lại
		int remainSubweapon = manager->GetSubweaponCounter() - 5;
		manager->SetSubweaponCounter(remainSubweapon);


		//Trong thời gian dùng StopWatch sẽ phát âm thanh
		SoundManager::GetInstance()->Play(SOUND_SW_STOP_WATCH);


		//Xác nhận đang dùng StopWatch
		isUseStopWatch = true;

		//Không cho tạo Subweapon nữa
		createSubweapon = false;


	}
	else {


		//Kiểm tra double shot or triple shot để biết số lượng tối đa subweapon được sử dụng
		switch (manager->GetDoubleShotOrTrippleShot())
		{
		case DOUBLE_SHOT_HUD:
			maxSubWeaponCreate = NUM_SW_DOUBLESHOT;
			break;
		case TRIPPLE_SHOT_HUD:
			maxSubWeaponCreate = NUM_SW_TRIPLESHOT;
			break;
		default:
			maxSubWeaponCreate = NUM_SW_NORMAL;
			break;
		}

		//Xử lí việc tạo ra nhiều subweapon cùng lúc
		//Lấy thời gian hiện tại
		DWORD now = GetTickCount();

		//Thời gian bắt đầu tạo ra subweapon (chạy lần đầu-initial)
		if (startTimeCreateSubweapon == -1)
			//Ghi lại thời gian tạo subweapon
			startTimeCreateSubweapon = GetTickCount();



		//Đủ thời gian cho phép tạo subweapon tiếp theo
		if (now - startTimeCreateSubweapon > WEAPON_CREATED_TIME) {
			isCreateNextSubWeapon = true; //Cho phép tạo subweapon
		}
		else {//Nếu không thì không cho phép tạo subweapon
			isCreateNextSubWeapon = false; //Không cho phép tạo subweapon
		}


		//Cho phép tạo Subweapon Dagger, Axe, Boomerang, HolyWater
		if (createSubweapon) {

			//Bắt đầu đếm thời gian từ khi tạo subweapon này
			startTimeCreateSubweapon = GetTickCount();

			SubWeapon* subweapon = nullptr;

			int aniSetSubWeapon = -1;

			//Tùy vào subweapon ID là gì thì ta tạo ra subweapon tương ứng
			//Đồng thời phát âm thanh tương ứng với SW đó
			switch (manager->GetSubweaponID())
			{
			case DAGGER_HUD:
				SoundManager::GetInstance()->Play(SOUND_SW_DAGGER);
				subweapon = new DaggerSubWeapon(simon);
				aniSetSubWeapon = WEAPONS_DAGGER;
				break;
			case AXE_HUD:
				SoundManager::GetInstance()->Play(SOUND_SW_AXE);
				subweapon = new AxeSubWeapon(simon);
				aniSetSubWeapon = WEAPONS_AXE;
				break;
			case BOOMERANG_HUD:
				SoundManager::GetInstance()->Play(SOUND_SW_BOOMERANG);
				subweapon = new BoomerangSubWeapon(simon);
				aniSetSubWeapon = WEAPONS_BOOMERANG;
				break;
			case HOLY_WATER_HUD:
				SoundManager::GetInstance()->Play(SOUND_SW_HOLYWATER);
				subweapon = new HolyWaterSubWeapon(simon);
				aniSetSubWeapon = WEAPONS_HOLY_WATER;
				break;
			default:
				break;
			}

			//Set Animation
			//Lấy Animation set trong danh sách AnimationSet
			CAnimationSets* animation_sets = CAnimationSets::GetInstance();

			//Dựa vào ID 
			LPANIMATION_SET ani_set = animation_sets->Get(aniSetSubWeapon);

			//Set animationSet cho Object này
			subweapon->SetAnimationSet(ani_set);


			//Vị trí của SW bắt đầu xuất hiện
			float x, y;

			//Lấy vị trí của SW
			subweapon->GetPosition(x, y);


			//Tạo một objectCell mới để add vào Grid
			CObjectCell* objectCell = new CObjectCell(subweapon, x, y);


			//Tăng số lượng SW hiện có lên 1 đơn vị
			numSubWeaponCreated++;


			//Tính số lượng subweaponCounter (quả tim nhỏ) còn lại
			int remainSubweapon = manager->GetSubweaponCounter() - 1;
			manager->SetSubweaponCounter(remainSubweapon);


			//Không cho tạo Subweapon nữa
			createSubweapon = false;
		}



	}


	



}





//Kiểm tra xem có được phép dùng StopWatch hay không
//(sẽ chạy trong class Simon. Hàm UsingSubweapon để check mỗi khi player đánh vũ khí phụ)
bool SubWeaponManager::isAvailableCreateStopWatch() {

	//Nếu chưa ăn subweapon item nào thì k được sử dụng
	if (PlayerManager::GetInstance()->GetSubweaponID() == -1) return false;


	//Nếu số subweaponCounter (quả tim nhỏ) < 5 thì không cho dùng
	if (PlayerManager::GetInstance()->GetSubweaponCounter() < SWC_FOR_STOPWATCH_SUBWEAPON)
		return false;

	//Nếu đang dùng StopWatch thì không cho dùng nữa
	if (!StopWatchSubWeapon::GetInstance()->IsStopWatchTimeUp())
		return false;


	//Được phép dùng StopWatch
	return true;
}





//Kiểm tra xem có được phép dùng Dagger, Axe, Boomerang, HolyWater hay không
//(sẽ chạy trong class Simon. Hàm UsingSubweapon để check mỗi khi player đánh vũ khí phụ)
bool SubWeaponManager::isAvailableCreateOtherSubWeapon() {

	//Nếu chưa ăn subweapon item nào thì k được sử dụng
	if (PlayerManager::GetInstance()->GetSubweaponID() == -1) return false;

	//Nếu số subweaponCounter (quả tim nhỏ) < 1 thì không cho dùng
	if (PlayerManager::GetInstance()->GetSubweaponCounter() < SWC_FOR_OTHER_SUBWEAPON) return false;


	//Số lượng subweapon hiện có >= giới hạn cho phép thì không cho dùng
	if (!(numSubWeaponCreated < maxSubWeaponCreate)) return false;


	//Nếu chưa đủ thời gian tạo subweapon kế tiếp thì không cho dùng
	if (!isCreateNextSubWeapon) return false;


	//Được phép dùng Dagger, Axe, Boomerang, HolyWater
	return true;
}





//Kiểm tra xem đã hết thời gian dùng stopWatch chưa
void SubWeaponManager::CheckStopWatchOverTime()
{
	//Nếu hết thời gian dùng StopWatch
	if (StopWatchSubWeapon::GetInstance()->IsStopWatchTimeUp()) {
		
		//Tắt cờ dùng stopWatch
		isUseStopWatch = false;

		//Nếu còn phát âm thanh StopWatch
		if (SoundManager::GetInstance()->isPlayingSound(SOUND_SW_STOP_WATCH)) {

			//Dừng phát âm thanh StopWatch
			SoundManager::GetInstance()->Stop(SOUND_SW_STOP_WATCH);
		}
	}


}
