#include "StairManager.h"

StairManager* StairManager::_instance = NULL;


//Lấy thể hiện duy nhất của StairManager
StairManager* StairManager::GetInstance()
{
	if (_instance == NULL) _instance = new StairManager();
	return _instance;
}



//Lấy toàn bộ bậc thang của màn chơi hiện tại
void StairManager::LoadListStair(vector<LPGAMEOBJECT>* listObjects) {

	//Làm sạch list
	listStair.clear();


	Stair* stair;

	//Duyệt qua tất cả các object hiện có
	for (UINT i = 0; i < listObjects->size(); i++) {
		//Nếu loại object là stair thì đưa vào list stairs
		if (dynamic_cast<Stair*>(listObjects->at(i))) {
			Stair* stair = dynamic_cast<Stair*>(listObjects->at(i));

			listStair.push_back(stair);

		}
	}

}




//Kiẻm tra và xử lý va chạm (nếu có) giữa simon và stair
//Liên tục gọi hàm này ở Playscene
void StairManager::Update(Simon* simon)
{


	//Biến chứa boudingBox của simon 
	float simonLeft, simonTop, simonRight, simonBottom;

	//Lấy vùng bao của Simon(xét va chạm giữa simon và từng bậc thang)
	simon->GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);


	//Xét chân simon va chạm với mỗi bậc thang => cần kéo boudingBox về chân simon
	//Vì break point = -1 (chân cầu thang) nằm ở dưới brick tức là dưới chân của simon nên phải hạ bottom xuống mới có thể va chạm
	//Nếu không kéo về chân sẽ dẫn đến kết quả sai. VD simon chưa ra khỏi thang nhưng rớt xuống do top của simon chạm đỉnh thang
	simonTop += 54;	
	simonBottom += 6;  




	//Duyệt tất cả bậc thang trong listStair
	for (UINT i = 0; i < listStair.size(); i++)
	{
		//Vùng bao của Một bậc thang đang xét
		float stairLeft, stairTop, stairRight, stairBottom;	


		//Không xét va chạm với bậc thang thông thường (breakpoint = 0)
		if (listStair[i]->GetBreakPoint() == STAIR_NORMAL) continue;


		//Lấy vùng bao của Một Stair break point đang xét
		listStair[i]->GetBoundingBox(stairLeft, stairTop, stairRight, stairBottom);


		//Kiểm tra va chạm giữa Simon và một stair bất kì
		if (simon->BroadPhaseTestAABB(simonLeft, simonTop, simonRight, simonBottom, stairLeft, stairTop, stairRight, stairBottom))
		{
			//Nếu có va chạm

			//Lưu lại bậc thang va chạm với Simon => xử lí dời simon về breakpoint vị trí breakpoint thang
			//Sẽ sử dụng trong 1 trong hai hàm SimonWalkingDownStair hoặc SimonWalkingUpStair
			stairCollided = listStair[i];



			//Lấy hướng của bậc thang mà va chạm với Simon
			if (stairCollided->GetState() == STAIR_LEFT_RIGHT_ANI)		//Bậc thang dẫn từ trái qua phải, tương ứng với animation_set 0 của stair
				stairOrientation = STAIR_ORI_LEFT_TO_RIGHT;
			else if(stairCollided->GetState() == STAIR_RIGHT_LEFT_ANI)  //Bậc thang dẫn từ phải qua trái, tương ứng với animation_set 1 của stair
				stairOrientation = STAIR_ORI_RIGHT_TO_LEFT;


			//Nếu va chạm với break point có nhiệm vụ đảo chiều simon trên thang
			if (stairCollided->GetBreakPoint() == STAIR_DOUBLE && !isTouchingBreakDoubleStair) {
				//Không cho va chạm nữa
				isTouchingBreakDoubleStair = true; 

				//Simon có thể di chuyển lên trên cầu thang 
				ableWalkingUp = true;

				//Simon không thể di chuyển xuống cầu thang 
				ableWalkingDown = false;

				//Lên thang
				SimonStandingOnStair(simon);

				//Xuống thang
				SimonWalkingUpStair(simon);

				//Quay lại vòng lặp 
				continue;
			}


			//Va chạm lần đầu tiên
			//Lúc này simon còn đang ở dưới đất (isStandOnStair sai)
			if (!isStandOnStair) {


				//Nếu simon va chạm với chân cầu thang
				if (stairCollided->GetBreakPoint() == STAIR_BOTTOM) {

					//Simon có thể di chuyển lên trên cầu thang 
					ableWalkingUp = true;

					//Simon không thể di chuyển xuống cầu thang 
					ableWalkingDown = false;

				}
				//Nếu simon va chạm với đỉnh cầu thang
				else if (stairCollided->GetBreakPoint() == STAIR_TOP) {
					
					//Simon có thể di xuống cầu thang 
					ableWalkingDown = true;

					//Simon không thể di chuyển lên trên cầu thang 
					ableWalkingUp = false;
				}


			}
			//Nếu không
			//Lúc này Simon đang đứng trên thang
			else { 
				
				//Lấy vị trí của simon
				float x, y;
				simon->GetPosition(x, y);


				//Khi đứng trên thang thì Simon có thể di chuyển lên xuống cầu thang
				ableWalkingUp = true;
				ableWalkingDown = true;


				//Nếu simon đang đi lên mà va chạm với đỉnh cầu thang => xử lí simon ra khỏi thang ở đỉnh thang
				if (simon->GetState() == WALKING_UP_STAIR && stairCollided->GetBreakPoint() == STAIR_TOP) {

					//Nâng simon lên trên một đoạn đúng bằng chiều cao của thang
					//Nếu không simon sẽ bị rơi xuống vì isStandOnStair = false mà chưa kịp vượt quá nấc thang cuối cùng (breakpoint top)
					y -= 5;

					//Tắt hết cờ kiểm soát simon lên bậc thang
					ableWalkingUp = false;
					isStandOnStair = false;
					ableWalkingDown = false;
				}

				//Nếu simon đang đi xuống mà va chạm với chân cầu thang => xử lí simon ra khỏi thang ở chân thang
				if (simon->GetState() == WALKING_DOWN_STAIR && stairCollided->GetBreakPoint() == STAIR_BOTTOM) {
					
					//Simon không bị rơi khi đến bậc thang cuối cùng
					//Nếu không simon sẽ bị rơi xuống vì simon bottom nằm dưới gạch=> cần dịch tọa độ Y của simon lên
					y -= 5;


					//Tắt hết cờ kiểm soát simon xuống bậc thang
					isStandOnStair = false;
					ableWalkingDown = false;
					ableWalkingUp = false;
				}

				//Set lại vị trí của simon nếu có chạm vào break point thang
				simon->SetPosition(x, y);

			}


		}

	}


}






//Khi Simon đi xuống dưới cầu thang
void StairManager::SimonWalkingDownStair(Simon *simon)
{
	//Nếu simon đã va chạm với breakPoint Stair (đỉnh thang) Mà chưa hề ở trên thang
	//Dời Simon về bậc thang đầu tiên để đảm bảo simon đi lên không bị lệch với vị trí của thang
	if (!isStandOnStair)
	{
		float stairPosX, stairPosY, simonPosX, simonPosY;


		//Lấy tọa độ X của bậc thang va chạm với simon
		stairPosX = stairCollided->GetX();
		stairPosY = stairCollided->GetY();

		//Lấy tọa độ của Simon
		simon->GetPosition(simonPosX, simonPosY);

		simonPosY = stairPosY - SIMON_HEIGHT + STAIR_HEIGHT ;

		//Nếu cầu thang dẫn từ trái qua phải
		if (stairOrientation == STAIR_ORI_LEFT_TO_RIGHT) {
			//Dời simon về vị trí của bậc thang va chạm, Nhưng vì có phần thừa của simon nên ta phải trừ đi phần thừa đó
			simonPosX = stairPosX - SIMON_REDUNDANT_HAND;
		}
		//Nếu cầu thang dẫn từ phải qua trái
		else if (stairOrientation == STAIR_ORI_RIGHT_TO_LEFT) {
			//Dời simon về vị trí của bậc thang va chạm, Nhưng vì có phần thừa của simon nên ta phải trừ đi phần thừa đó
			//Dịch sẵn 16px do có set vx nên ta phải trừ đi
			simonPosX = stairPosX - SIMON_REDUNDANT_ELBOW;		
		}


		//Set tọa đõ của Simon để đứng lên thang. Giảm Y để simon nhích xuống dưới 
		simon->SetPosition(simonPosX, simonPosY + 0.1);

		//Simon luôn ngược hướng với cầu thang
		simon->SetOrientation(-stairOrientation);

		//Set trạng thái đang ở trên thang
		isStandOnStair = true;

	}
	else //Simon đang ở trên cầu thang
	{
		//Hướng của simon luôn ngược hướng cầu thang
		simon->SetOrientation(-stairOrientation);

		//Set trạng thái simon đi xuống thang
		simon->SetState(WALKING_DOWN_STAIR);
	}

}





//Simon đi lên cầu thang
void StairManager::SimonWalkingUpStair(Simon* simon)
{
	//Nếu simon đã va chạm với breakPoint Stair
	//Mà chưa hề ở trên thang
	//Dời Simon về bậc thang đầu tiên để đảm bảo simon đi lên không bị lệch với vị trí của thang
	if (!isStandOnStair)
	{
		float stairPosX, stairPosY, simonPosX, simonPosY;


		//Lấy vị trí của bậc thang va chạm với simon
		stairPosX = stairCollided->GetX();
		stairPosY = stairCollided->GetY();

		//Lấy vị trí của Simon
		simon->GetPosition(simonPosX, simonPosY);

		//Dời vị trí Y của simon lên theo bậc thang va chạm
		simonPosY = stairPosY - SIMON_HEIGHT;

		//Dời simon về vị trí bước lên bậc thang đầu tiên
		//Nếu cầu thang dẫn từ trái qua phải
		if (stairOrientation == STAIR_ORI_LEFT_TO_RIGHT) {
			simonPosX = stairPosX - SIMON_REDUNDANT_ELBOW;	
		}
		else //Nếu cầu thang dẫn từ phải qua trái
		{
			simonPosX = stairPosX - SIMON_REDUNDANT_HAND;
		}



		//Set lại vị trí của Simon
		simon->SetPosition(simonPosX, simonPosY);


		//Hướng của simon luôn cùng hướng cầu thang
		simon->SetOrientation(stairOrientation);


		//Bật trạng thái simon đang đứng trên thang
		isStandOnStair = true;


	}
	else  //Simon đang ở trên cầu thang
	{
		//Hướng của simon luôn ngược hướng cầu thang
		simon->SetOrientation(stairOrientation);

		//Set trạng thái simon đi lên thang
		simon->SetState(WALKING_UP_STAIR);
	}


}




//Xử lí khi simon đứng trên thang
void StairManager::SimonStandingOnStair(Simon* simon)
{

	//Các trạng thái mà ta cho phép simon đứng trên thang
	if (simon->GetState() == WALKING_UP_STAIR || simon->GetState() == WALKING_DOWN_STAIR
		|| simon->GetState() == WALKING_UP_STAIR_WHOPPING || simon->GetState() == WALKING_DOWN_STAIR_WHOPPING)
	{

		//đang đi lên trên thang và đánh
		if (simon->GetState() == WALKING_UP_STAIR_WHOPPING)
		{
			simon->SetState(WALKING_UP_STAIR);
			simon->animation_set->at(WALKING_UP_STAIR)->startCountTimeAnimation(0);
		}
		//đang đi xuống thang và đánh
		else if (simon->GetState() == WALKING_DOWN_STAIR_WHOPPING)
		{
			simon->SetState(WALKING_DOWN_STAIR);
			simon->animation_set->at(WALKING_DOWN_STAIR)->startCountTimeAnimation(0);
		}


		//Đang ở trên thang thì Simon có vx=vy=0
		simon->SetSpeed(0.0, 0.0);

		//Reset lại animation của các animation STAIR_UP,STAIR_DOWN. 
		//Nếu không thì nó sẽ render tiếp các animation dẫn đến simon chuyển frame ảnh liên tục
		simon->animation_set->at(WALKING_UP_STAIR)->Reset();
		simon->animation_set->at(WALKING_DOWN_STAIR)->Reset();

	}

}