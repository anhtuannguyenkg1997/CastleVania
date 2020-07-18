#include "CCell.h"



//Thêm một objectCell vào danh sách objectCell của một Cell
void  CCell::AddObjectCell(CObjectCell* objectCell) {
	listObjectCells.push_back(objectCell);

}



//Xóa một objectCell ra khỏi danh sách objectCell của một Cell
void CCell::DeleteObjectCell(CObjectCell* objectCell) {

	//https://stackoverflow.com/questions/26567687/how-to-erase-vector-element-by-pointer
	//Nếu ta biết rằng chỉ có một phần tử cần xóa
	//ta có thể sử dụng find để dừng ngay khi tìm thấy giá trị

	auto it = std::find(listObjectCells.begin(), listObjectCells.end(), objectCell);
	if (it != listObjectCells.end()) { listObjectCells.erase(it); }


}



//Xóa tất cả các ObjectCell trong Cell
//Đưa listCell về 0 element
void CCell::Clear() {
	//https://stackoverflow.com/questions/991335/how-to-erase-delete-pointers-to-objects-stored-in-a-vector
	
	int size = listObjectCells.size();

	//Nếu listObjectCells không có element nào thì ta return luôn
	if (size == 0) return;

	//Duyệt từ đầu đến cuối vector listObjectCells
	//Gán element đầu tiên cho biến it
	for (auto it = listObjectCells.begin(); it != listObjectCells.end(); ) {

		//Nếu it khác NULL
		if ((*it)!=NULL) {

			//Tiến hành xóa it trong listObjectCells
			delete* it;
			it = listObjectCells.erase(it);
		}	//Nếu không duyệt đến element tiếp theo
		else {
			++it;
		}
	}


	//Làm sạch listObjectCells
	listObjectCells.clear();

	//DebugOut(L"Cell cleared, cell size: %d \n", listObjectCells.size());
}