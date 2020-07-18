#pragma once
#include "CObjectCell.h"


class CCell
{
	//Lưu trữ các danh sách objectCell của Cell này
	vector<CObjectCell*> listObjectCells;

public:
	//Constructor
	CCell() {}


	//Lấy danh sách objectCell của Cell này
	vector<CObjectCell*> GetObjectList() { return listObjectCells; }


	//Thêm một CObjectCell vào danh sách objectCell của cell này
	void AddObjectCell(CObjectCell* objectCell);


	//Xóa một CObjectCell khỏi danh sách CObjectCell của Cell này
	void DeleteObjectCell(CObjectCell* objectCell);


	//Lấy số lượng ObjectCell trong cell này
	int GetSize() { return listObjectCells.size(); }


	//Làm sạch cell
	void Clear();
};

