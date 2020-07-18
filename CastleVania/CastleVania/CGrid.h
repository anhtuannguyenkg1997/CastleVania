#pragma once

#include <vector>
#include "CGameObject.h"

#include "CCell.h"



class CGrid
{
	//Thể hiện duy nhất của Grid
	static CGrid* _instance;

	//Chiều rộng của map
	int mapWidth;

	//Chiều cao của map
	int mapHeight;


	//Số cột của grid
	int cols;
	int rows;


	//Danh sách các Cell trong Grid. 
	//Mảng 2 chiều với kích thước mỗi Cell bằng 1/2 chiều dài và 1/2 chiều rộng
	//Kích thước của listCells phụ thuộc vào cols và rows
	vector<vector<CCell*>> listCells;


	

public:

	

	//CGrid(int map_width, int map_height);
	void InitGrid(int map_width, int map_height);
	~CGrid();

	//Lấy thể hiện duy nhất của CGrid
	static CGrid* GetInstance();

	//Thêm một ObjectCell vào Cell
	void AddToUnKnownCell(CObjectCell* objectCell);
	void AddToKnownCell(CObjectCell* objectCell, int col, int row);



	//Lấy các Object gán vào từng list phù hợp
	void GetListObject(vector<CObjectCell*> &listObjectCells, vector<LPGAMEOBJECT> &listObjects);

	//Cập nhật một ObjectCell dựa vào tọa độ của GameObject và tọa độ Camera
	void UpdateObjectCell(CObjectCell* unit, float x, float y);


	//Cập nhật Grid
	void Update(vector<CObjectCell*> &listObjectCells);

	// lấy tất cả các ObjectCell* nằm trong vùng Camera để Update và Render
	vector<CObjectCell*> GetObjectCell(); 


	void ShowSizeCells();

	//Xóa tất cả cell trong Grid
	void Clear();




};