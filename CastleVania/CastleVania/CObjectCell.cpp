#include "CObjectCell.h"
#include "CGrid.h"


CGrid* grid = CGrid::GetInstance();


//Thêm mới một object cell vào Grid
CObjectCell::CObjectCell(LPGAMEOBJECT obj, float x, float y)
{
	this->obj = obj;
	this->x = x;
	this->y = y;

	grid->AddObjectCell(this);
}



//Cập nhật lại vị trí của ObjectCell trong cells
//Di chuyển objectCell sang cell mới nếu tọa độ mới của objectCell ở vùng cell khác
void CObjectCell::Update(float x, float y)
{
	grid->UpdateObjectCell(this, x, y);
}