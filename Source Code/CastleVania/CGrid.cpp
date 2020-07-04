#include "CGrid.h"
#include "CCamera.h"


#include "Brick.h"
#include "Torch.h"
#include "Portal.h"
#include "SubWeapon.h"
#include "SubWeaponManager.h"
#include "PlayerManager.h"
#include "VampireBat.h"
#include "Bone.h"
#include "EnemyRevival.h"
#include <string>

using namespace std;

CGrid* CGrid::_instance = NULL;

CCamera* camera = CCamera::GetInstance();


//Lấy thể hiện duy nhất của CGrid
CGrid* CGrid::GetInstance()
{
	if (_instance == NULL) _instance = new CGrid();
	return _instance;
}




//Khởi tạo Grid dựa vào chiều dài và chiều cao của map
void CGrid::InitGrid(int width, int height)
{

	this->mapWidth = width;
	this->mapHeight = height;

	//Lấy ra những cell nằm trong khoảng từ cột bắt đầu đến cột kết thúc để chọn những object trong các cell đó
	//Cột bắt đầu 
	cols = mapWidth / DEFAULT_CELL_WIDTH;

	//Cột kết thúc 
	//ceil làm tròn giá trị VD: 2.3 -> 3
	rows = mapHeight / DEFAULT_CELL_HEIGHT;


	//Set lại kích thước của vector cell
	//kích thước bằng số cột x số dòng
	listCells.resize(rows);

	for (int i = 0; i < rows; i++)
		listCells[i].resize(cols);


	//Tạo mới tất cả các cell của grid
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			listCells[i][j] = new CCell();
}



//Thêm một CObjectCell vào Cell đã biết trước
void CGrid::AddToUnKnownCell(CObjectCell* objectCell)
{
	//Tính vị trí của cell nằm ở đâu trong Grid
	//Tức là CObjectCell này nằm ở dòng mấy, cột mấy trong danh sách vector 2 chiều cells
	int row = (int)(objectCell->GetY() / DEFAULT_CELL_HEIGHT);	//Vị trí dòng
	int col = (int)(objectCell->GetX() / DEFAULT_CELL_WIDTH);	//Vị trí cột

	//Thêm objectCell vào trong cell tương ứng
	listCells[row][col]->AddObjectCell(objectCell);

}



//Thêm một CObjectCell vào Cell chưa biết
void CGrid::AddToKnownCell(CObjectCell* objectCell, int col, int row)
{
	//Thêm objectCell vào trong cell tương ứng
	listCells[row][col]->AddObjectCell(objectCell);
}



//Lấy các object và gán vào các list phù hợp
//listObjectCells: ObjectCell trong vùng camera đang xét
//listObjects: danh sách GameObject trong vùng camera đang xét
//listStaticObjects: danh sách GameObject đang đứng yên trong vùng camera đang xét
//listStaticObjects: danh sách GameObject đang di chuyển trong vùng camera đang xét
void CGrid::GetListObject(vector<CObjectCell*> &listObjectCells, vector<LPGAMEOBJECT> &listObjects) {
	//Làm sạch list object để Cập nhật lại những danh sách này
	listObjectCells.clear();
	listObjects.clear();

	//Lấy các ObjectCells dựa vào vị trí của Camera
	listObjectCells = GetObjectCell();


	//Duyệt hết các ObjectCell (GameObject) đã lấy ra
	for (UINT i = 0; i < listObjectCells.size(); i++)
	{
		//Lấy GameObject từ ObjecCell ra
		LPGAMEOBJECT obj = listObjectCells[i]->GetObj();

		//Đưa nó vào listObjects
		listObjects.push_back(obj);

	}
}





////Dựa vào vị trí của Camera, lấy ra những object nằm trong vùng camera đó
////Đưa vào listObjectCell
vector<CObjectCell*> CGrid::GetObjectCell()
{
	PlayerManager* playerManager = PlayerManager::GetInstance();


	//danh sách ObjectCell cần trả về
	vector<CObjectCell*> listObjectCells;

	//Lấy ra những cell nằm trong khoảng từ cột bắt đầu đến cột kết thúc để chọn những object trong các cell đó
	//Cột bắt đầu 
	int fromCol = (int)(camera->GetCameraPositon().x / DEFAULT_CELL_WIDTH);

	//Cột kết thúc 
	//ceil làm tròn giá trị VD: 2.3 -> 3
	int toCol = ceil((camera->GetCameraPositon().x + CAMERA_WIDTH) / DEFAULT_CELL_WIDTH);


	//Duyệt qua những cell nằm trong đoạn cols [start_col, end_col]
	//Những đoạn này ta cần lấy object ra vì nó nằm trong vùng camera
	//Ngoài những cell này thì ta không cần quan tâm đến những object nằm trong cell đó
	for (int i = 0; i < rows; i++)
	{
		for (int j = fromCol; j < toCol; j++)
		{
			//Lấy cell ra
			CCell* cell = listCells[i][j];

			//Duyệt trong cell này. lấy ra những object còn đang hoạt động & nằm trong camera
			//Đưa nó vào listObjectCell
			for (int k = 0; k < listCells[i][j]->GetObjectList().size(); k++) {

				LPGAMEOBJECT obj = listCells[i][j]->GetObjectList().at(k)->GetObj();

				//Nếu object cần hồi sinh
				if (obj->isNeedRevival) {
					
					//Thêm nó vào listRevival
					EnemyRevival::GetInstance()->AddToListRevival(listCells[i][j]->GetObjectList().at(k));
					
					//tắt cờ cho phép hồi sinh 
					listCells[i][j]->GetObjectList().at(k)->GetObj()->isNeedRevival = false;
					continue;
				}
					

				//Đưa các brick vào list trừ brick màn đấu boss
				if (dynamic_cast<Brick*>(obj) && obj->GetType() == BOSS_BRICK &&  playerManager->GetIsFightingBoss()) {
					listObjectCells.push_back(listCells[i][j]->GetObjectList().at(k));
					continue;
				}
				//Đối với subweapon vì ta cần phải update lại số lượng subweapon còn lại => chạy vào hàm update của subweapon manager 
				//Vì thế subweapon này phải chạy vào hàm update => không cần check nó nằm trong camera hay không ở đây
				//Vì nếu ở đây mà k nằm trong camera thì nó sẽ không thể chạy vào hàm update của subweapon => không tính đc vũ khí phụ còn lại => không đánh tiếp được subweapon
				if (dynamic_cast<SubWeapon*>(obj)&& obj->isActive()) {
					listObjectCells.push_back(listCells[i][j]->GetObjectList().at(k));
					continue;
				}
				if(camera->checkObjectInCamera(obj) && obj->isActive()){
					listObjectCells.push_back(listCells[i][j]->GetObjectList().at(k));
				}
			}
		}
	}

	return listObjectCells;
}



//Cập nhật ObjectCell trong Grid.
//Bỏ qua ObjectCell nào đang ngoài vùng camera
void CGrid::Update(vector<CObjectCell*> &listObjectCells) {

	SubWeaponManager* subweaponManager = SubWeaponManager::GetInstance();


	
	
	EnemyRevival::GetInstance()->Update();



	//Duyệt qa tất cả các objectCell 
	for (int i = 0; i < listObjectCells.size(); i++)
	{
		//Lấy từng object ra
		LPGAMEOBJECT obj = listObjectCells[i]->GetObj();


		//Nếu không là loại dynamic object thì không cần phải update objectcell này
		if (obj->GetType() != DYNAMIC_OBJECT) {
			continue;
		}

		//Khi Subweapon đi ra khỏi vùng camera thì Active(false)
		//Chỉ áp dụng cho Subweapon & vampireBat vì nếu các object khác
		//VD như Torch, nếu không Active nó thì nó sẽ mất
		if (!camera->checkObjectInCamera(listObjectCells[i]->GetObj())) {
			
			if (dynamic_cast<SubWeapon*>(listObjectCells[i]->GetObj())) {
				//Weapon này sẽ biến mất
				listObjectCells[i]->GetObj()->setActive(false);
				continue;
			}


			//Object di chuyển ra khỏi map => cần xóa
			if (dynamic_cast<VampireBat*>(listObjectCells[i]->GetObj())) {
				listObjectCells[i]->GetObj()->setActive(false);
				continue;
			}

		}


		//Nếu object đang còn 
		float newPos_x, newPos_y;

		//Ta lấy vị trí của object đó (sau khi update)
		if (obj == NULL) continue;
	
		
		obj->GetPosition(newPos_x, newPos_y);


		//Cập nhật cell cho objectCell này nếu nó di chuyển đến cell khác
		//Nếu nó ra khỏi vùng camera thì không cần update object này
		listObjectCells[i]->Update(newPos_x, newPos_y);

		//DebugOut(L">>>Update ObjectCell %f %f \n", newPos_x, newPos_y);
	}
}





//Cập nhật lại ObjectCell dựa vào vị trí camera và vị trí của object đó
//Nếu object ra khỏi camera thì không cần cập nhật
//Nếu object di chuyển qua cell khác thì xóa nó trong cell cũ và thêm vào cell mới
void CGrid::UpdateObjectCell(CObjectCell* objectCell, float x, float y)
{
	//Lấy vùng camera trên Grid
	//Cột bắt đầu 
	int fromCol = (int)(camera->GetCameraPositon().x / DEFAULT_CELL_WIDTH);

	//Cột kết thúc 
	//ceil làm tròn giá trị VD: 2.3 -> 3
	int toCol = (ceil((camera->GetCameraPositon().x + CAMERA_WIDTH) / DEFAULT_CELL_WIDTH)) - 1;


	// lấy chỉ số cell cũ
	int staleRow = (int)(objectCell->GetY() / DEFAULT_CELL_HEIGHT);
	int staleCol = (int)(objectCell->GetX() / DEFAULT_CELL_WIDTH);

	// lấy chỉ số cell mới
	int currentRow = (int)(y / DEFAULT_CELL_HEIGHT);
	int currentCol = (int)(x / DEFAULT_CELL_WIDTH);


	// nếu object ra khỏi vùng camera -> không cần cập nhật
	if (!(currentCol <= toCol || currentCol >= fromCol))
		return;


	// cập nhật toạ độ mới
	objectCell->SetX(x);
	objectCell->SetY(y);

	// object vẫn nằm trong cell cũ thì không cần chuyển nó sang cell khác
	if (staleCol == currentCol && staleRow == currentRow)
		return;



	//Trường hợp object di chuyễn qua đến cell khác
	// thêm vào cell mới
	listCells[currentRow][currentCol]->AddObjectCell(objectCell);


	// Xóa khỏi cell cũ
	listCells[staleRow][staleCol]->DeleteObjectCell(objectCell);

	//DebugOut(L"Them vao cell moi \n");
}



//Làm sạch grid
void CGrid::Clear() {

	//Duyệt tất cả các Cell trong list Cell 
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{

			//Xóa từng cell
			listCells[i][j]->Clear();
		}



	//Xóa list cell
	listCells.clear();

	//Xóa listRevival
	EnemyRevival::GetInstance()->Clear();
	

}



void CGrid::ShowSizeCells() {
	//Lấy vùng camera trên Grid
	//Cột bắt đầu 
	int fromCol = (int)(camera->GetCameraPositon().x / DEFAULT_CELL_WIDTH);

	//Cột kết thúc 
	//ceil làm tròn giá trị VD: 2.3 -> 3
	int toCol = (ceil((camera->GetCameraPositon().x + CAMERA_WIDTH) / DEFAULT_CELL_WIDTH)) - 1;


	for (int i = 0; i < rows; i++) {
		for (int j = fromCol; j <= toCol; j++) {
			DebugOut(L"Cell[%d][%d]=%d || ",i,j, listCells[i][j]->GetSize());
		}
	}
	DebugOut(L"\n");
}







CGrid::~CGrid()
{
}










