#include "CObjectCell.h"
#include "CGrid.h"


CGrid* grid = CGrid::GetInstance();


//Thêm mới một object cell vào Grid
///Object này chưa biết vị trí Cell
CObjectCell::CObjectCell(LPGAMEOBJECT obj, float x, float y)
{
	this->obj = obj;
	this->x = x;
	this->y = y;

	grid->AddToUnKnownCell(this);
}


//Thêm mới một object cell vào Grid
///Object này đã biết vị trí Cell
CObjectCell::CObjectCell(LPGAMEOBJECT obj, float x, float y, int colCell, int rowCell)
{
	this->obj = obj;
	this->x = x;
	this->y = y;

	grid->AddToKnownCell(this, colCell, rowCell);
}



//Cập nhật lại vị trí của ObjectCell trong cells
//Di chuyển objectCell sang cell mới nếu tọa độ mới của objectCell ở vùng cell khác
void CObjectCell::Update(float x, float y)
{
	grid->UpdateObjectCell(this, x, y);
}