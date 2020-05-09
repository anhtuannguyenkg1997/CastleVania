#include "CTextures.h"

CTextures* CTextures::_instance = NULL;


//Thêm một texture mới vào mảng textures
void CTextures::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	//Mô tả thông tin của hình ảnh (làm texture)
	D3DXIMAGE_INFO info;

	//Lấy thông tin về hình ảnh
	HRESULT hr = D3DXGetImageInfoFromFile(filePath, &info);


	//Kiểm tra việc lấy thông tin hình ảnh có thành công hay không????
	if (hr != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	//Lấy thiết bị vẽ được tạo trong class CGame
	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	//Đại diện cho texture được load lên
	LPDIRECT3DTEXTURE9 texture;


	//Tạo một texture từ một tập tin
	hr = D3DXCreateTextureFromFileEx(
		d3ddv,				//Thiết bị vẽ
		filePath,			//Đường dẫn đến texture
		info.Width,			//Chiều rộng của texture
		info.Height,		//Chiều cao của texture
		1,					//Số lượng mip yêu cầu
		D3DUSAGE_DYNAMIC,	//Surface được xử lý động
		D3DFMT_UNKNOWN,		//Định dạng pixel được yêu cầu cho texture
		D3DPOOL_DEFAULT,	 //Lớp bộ nhớ trong đó texture nên được cài đặt
		D3DX_DEFAULT,		//Hình ảnh được lọc (mặc định)
		D3DX_DEFAULT,		//Hình ảnh được lọc (mặc định)
		transparentColor,	//Màu trong suốt
		&info,				//Mô tả dữ liệu hình ảnh nguồn
		NULL,				//Đại diện cho bảng màu 256 màu để điền vào
		&texture			//Đại diện cho đối tượng texture đã tạo
	);


	//Kiểm tra việc tạo texture
	if (hr != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	//Đưa texture cùng với id của nó
	//id này giúp ta truy xuất vào texture đó
	textures[id] = texture;


	//DebugOut(L"[INFO] Load texture successfully: id=%d, path=%s \n", id, filePath);
}



//Làm sạch mảng textures đã load
void CTextures::Clear()
{
	//Duyệt qua mảng texture, gán từng element cho biến x
	for (auto x : textures)
	{
		//x.second là giá trị của x tức là data texture . gán cho tex
		LPDIRECT3DTEXTURE9 tex = x.second;

		//nếu tex không NULL thì ta giải phóng giá trị đó...
		if (tex != NULL) tex->Release();
	}

	//Làm sạch mảng textures
	textures.clear();
}


//Lấy thể hiện duy nhất của textures
CTextures* CTextures::GetInstance()
{
	if (_instance == NULL) _instance = new CTextures();
	return _instance;
}