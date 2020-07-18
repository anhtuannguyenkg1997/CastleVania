#include "CSprites.h"
#include "CGame.h"


CSprites* CSprites::_instance = NULL;


//Thêm một sprite mới vào danh sách sprite. 
void CSprites::Add(string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	//Tạo mới một sprite => gọi lại constructor của lớp sprite
	LPSPRITE sprite = new CSprite(id, left, top, right, bottom, tex);

	//Thêm nó vào mảng sprites
	//id sẽ phân biệt các sprite với nhau
	//Nhờ đó sẽ truy xuất vào đúng sprite cần lấy ra
	sprites[id] = sprite;
}



/*
	Làm sạch tất cả sprite đã load
*/
void CSprites::Clear()
{
	//Từ khóa auto có khả năng tự động nhận dạng được kiểu dữ liệu 
	//Duyệt qua mảng sprites
	for (auto x : sprites)
	{
		//Lấy từng sprite ra
		LPSPRITE s = x.second; //Truy cập đến giá trị của x. Lưu ý rằng x.first là key. x.second là value
		delete s; //Xóa sprite
	}

	//Làm sạch mảng sprites
	sprites.clear();
}



//Lấy thể hiện duy nhất của CSprite
CSprites* CSprites::GetInstance()
{
	if (_instance == NULL) _instance = new CSprites();
	return _instance;
}


