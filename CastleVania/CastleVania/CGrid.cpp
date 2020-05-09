#include "CGrid.h"
#include "CCamera.h"


#include "Brick.h"
#include "Torch.h"
#include "Portal.h"


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


	//DebugOut(L"col: %d, row: %d \n", cols, rows);
}



//Thêm một CObjectCell vào Grid
void CGrid::AddObjectCell(CObjectCell* objectCell)
{
	//Tính vị trí của cell nằm ở đâu trong Grid
	//Tức là CObjectCell này nằm ở dòng mấy, cột mấy trong danh sách vector 2 chiều cells
	int row = (int)(objectCell->GetY() / DEFAULT_CELL_HEIGHT);	//Vị trí dòng
	int col = (int)(objectCell->GetX() / DEFAULT_CELL_WIDTH);	//Vị trí cột

	//Thêm objectCell vào trong cell tương ứng
	listCells[row][col]->AddObjectCell(objectCell);

}



//Lấy các object và gán vào các list phù hợp
//listObjectCells: ObjectCell trong vùng camera đang xét
//listObjects: danh sách GameObject trong vùng camera đang xét
//listStaticObjects: danh sách GameObject đang đứng yên trong vùng camera đang xét
//listStaticObjects: danh sách GameObject đang di chuyển trong vùng camera đang xét
void CGrid::GetListObject(vector<CObjectCell*> &listObjectCells, vector<LPGAMEOBJECT> &listObjects, vector<LPGAMEOBJECT> &listObjectsRender) {
	//Làm sạch list object để Cập nhật lại những danh sách này
	listObjectCells.clear();
	listObjects.clear();
	listObjectsRender.clear();



	//Lấy các ObjectCells dựa vào vị trí của Camera
	listObjectCells = GetObjectCell();


	//Duyệt hết các ObjectCell (GameObject) đã lấy ra
	for (UINT i = 0; i < listObjectCells.size(); i++)
	{
		//Lấy GameObject từ ObjecCell ra
		LPGAMEOBJECT obj = listObjectCells[i]->GetObj();

		//Đưa nó vào listObjects
		listObjects.push_back(obj);

		//Kiểm tra xem ta sẽ đưa GameObject này vào đâu????
		//Nếu là gạch thì k cần đưa vào list nào cả. vì ta k cần render hay update mà chỉ cần xét va chạm cho nó=> quay lại vòng lặp
		if (dynamic_cast<Brick*>(obj))
			continue;
		//Push vào danh sách vật thể render
		else
			listObjectsRender.push_back(obj);

	}
}





////Dựa vào vị trí của Camera, lấy ra những object nằm trong vùng camera đó
////Đưa vào listObjectCell
vector<CObjectCell*> CGrid::GetObjectCell()
{
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

			//Duyệt trong cell này. lấy ra những object còn đang hoạt động
			//Đưa nó vào listObjectCell
			for (int k = 0; k < listCells[i][j]->GetObjectList().size(); k++) {
				if (listCells[i][j]->GetObjectList().at(k)->GetObj()->isActive()) {
					listObjectCells.push_back(listCells[i][j]->GetObjectList().at(k));
				}
			}
		}
	}

	return listObjectCells;
}



//Cập nhật ObjectCell trong Grid.
//Bỏ qua ObjectCell nào đang ngoài vùng camera
void CGrid::Update(vector<CObjectCell*> listObjectCells) {

	//Duyệt qa tất cả các objectCell 
	for (int i = 0; i < listObjectCells.size(); i++)
	{
		//Lấy từng object ra
		LPGAMEOBJECT obj = listObjectCells[i]->GetObj();

		//Nếu object đó đã bị hủy (VD cây đuốc bị đánh và biến mất)
		if (!obj->isActive())
			//Bỏ qua và không cập nhật Cell 
			//Tức là lúc này quay lên trên vòng lặp for
			continue;

		//Nếu object đang còn 
		float newPos_x, newPos_y;

		//Ta lấy vị trí của object đó (sau khi update)
		obj->GetPosition(newPos_x, newPos_y);


		//Cập nhật cell cho objectCell này nếu nó di chuyển đến cell khác
		//Nếu nó ra khỏi vùng camera thì không cần update object này
		listObjectCells[i]->Update(newPos_x, newPos_y);


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
}



//Làm sạch grid
void CGrid::Clear() {

	//Duyệt tất cả các Cell trong list Cell 
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{

			//Xóa từng cell
			//DebugOut(L"Size cell[%d][%d]=%d \n", i, j, listCells[i][j]->GetSize());
			listCells[i][j]->Clear();
		}


	//Xóa list cell
	listCells.clear();
}




CGrid::~CGrid()
{
}










