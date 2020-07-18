#include "CGame.h"
#include "CCamera.h"

CGame* CGame::_instance = NULL;



void CGame::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8Create failed!\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}



/*
	Khởi tạo DirectX. Tạo thiết bị direct3d để render mà không tạo cửa sổ. Khởi tạo thư viện sprite để render hình ảnh 2D.
	hInstance: Xử lí thể hiện của ứng dụng
	hWnd: xử lí thể hiện của cửa sổ
*/
void CGame::Init(HWND hWnd)
{
	//Tạo một đối tượng IDirect3D9
	//Nếu tạo thành công, hàm sẽ trả về con trỏ trỏ tới interface IDirect3D9
	//Hàm liệt kê và truy xuất khả năng của thiết bị thông qua đối tượng d3d này
	//Điều này cho phép bạn chọn thiết bị thay vì khởi tạo chúng
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	//Tạo một cấu trúc chứa cấu trúc thông tin của các thiết bị khác nhau
	D3DPRESENT_PARAMETERS d3dpp;


	//Làm sạch cấu trúc để sử dụng
	ZeroMemory(&d3dpp, sizeof(d3dpp));


	//Trả về hình chữ nhật không bao gồm các đường viền - khu vực để vẽ lên
	RECT rect;
	GetClientRect(hWnd, &rect);

	//Chạy chế độ cửa sổ
	d3dpp.Windowed = TRUE;

	//Loại bỏ các frame cũ
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//Định dạng bộ nhớ đệm RGB 32bit
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

	//Sổ lượng backbuffer cần có
	d3dpp.BackBufferCount = 1;

	//Chiều cao của backBuffer
	d3dpp.BackBufferHeight = rect.bottom + 1;

	//Chiều dài của backbuffer
	d3dpp.BackBufferWidth = rect.right + 1;


	//Tạo một lớp thiết bị bằng cách lấy thông tin từ d3dpp
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,						//Mặc định (luôn là như vậy)
		D3DDEVTYPE_HAL,							//Thiết bị mong muốn
		hWnd,									//Cửa sổ nắm giữ
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//Chỉ định xử lí phần cứng
		&d3dpp,									//Các tham số cho thiết bị được tạo
		&d3ddv);								//Đại diện cho thiết bị được tạo


	//Tạo thiết bị thất bại
	if (d3ddv == NULL)
	{
		DebugOut(L"[ERROR] CreateDevice failed\n");
		return;
	}

	//Lấy backbuffer từ chuỗi swap của thiết bị
	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);



	//Tạo một đối tượng sprite liên kết với một thiết bị cụ thể
	//Các đối tượng sprite được sử dụng để vẽ hình ảnh 2D lên màn hình
	D3DXCreateSprite(d3ddv, &spriteHandler);


	//Tạo font chữ (HUD)
	g_font = NULL;

	//Hàm AddFontResourceEx thêm resource phông chữ từ tệp đã chỉ định vào hệ thống.
	AddFontResourceEx(FILEPATH_FONT, FR_PRIVATE, NULL);


	//Tạo một đối tượng phông chữ cho một thiết bị
	//và Trả về một con trỏ tới interface ID3DXFont, đại diện cho đối tượng phông chữ được tạo.
	HRESULT hr = D3DXCreateFont(
		GetDirect3DDevice(), 16, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &g_font);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] Create font failed\n");
		return;
	}


	DebugOut(L"[INFO] Init Game done\n");
}



//Vẽ một sprite
//Tọa độ truyền vào là tọa độ sprite trong thế giới game
void CGame::Draw( int dependOnCamera, int orientation, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{

	//Lấy thể hiện duy nhất của camera => lấy tọa độ hiện tại của camera trong thế giới game
	CCamera* camera = CCamera::GetInstance();


	//vector3 mô tả tọa độ của sprite cần vẽ (chỉ dùng 2 chiều)
	//Chuyễn tọa độ của sprite (trong thế giới game) về tọa độ của Camera
	//Bởi vì ta cần vẽ trên camera chứ không phải vẽ trên thế giới game nên cần bước chuyển này
	//Vì khi camera di chuyển, tọa đọ của object luôn > camera 
	//Khi camera đứng yên (cameraPosition = 0) thì lúc này tọa độ thê giới game và camera trùng nhau
	D3DXVECTOR3 p = D3DXVECTOR3(x - camera->GetCameraPositon().x * dependOnCamera, y + camera->GetCameraPositon().y * dependOnCamera, 0);




	//Vùng bao của object (trong texture)
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;


	// flip sprite, using nx parameter
	//Ma trận dùng để một flip sprite
	D3DXMATRIX tempMatrix;
	D3DXMATRIX currentMatrix;


	//Lấy ma trân biến đổi temp (ban đầu của sprite)
	spriteHandler->GetTransform(&tempMatrix);

	//Tọa độ tâm của object trên camera
	D3DXVECTOR2 center = D3DXVECTOR2(p.x + (right - left) / 2, p.y + (bottom - top) / 2);

	//hướng theo chiều dương thì tiến hành flip X theo chiều ngang (horizontal)
	// (-1,1) => horizontal flipping
	D3DXVECTOR2 rotate = D3DXVECTOR2(orientation == OR_PLUS ? -1 : 1, 1);


	// Xây dựng một ma trận 2D lưu thông tin biến đổi (scale, rotate)
	//newTransform: Con trỏ tới cấu trúc D3DXMATRIX chứa kết quả của các phép biến đổi.
	//center: Con trỏ tới cấu trúc D3DXVECTOR2, một điểm xác định tâm phép biển đổi. Ở đây là tâm của sprite
	//0,0: hệ số scale
	//rotate: Con trỏ tới cấu trúc D3DXVECTOR2, một điểm xác định tỷ lệ. 
	//NULL: Con trỏ tới cấu trúc D3DXVECTOR2, một điểm xác định tâm xoay. không cần xoay nên đối số là NULL
	//0.0f: Các góc quay theo radian.
	//Con trỏ tới cấu trúc D3DXVECTOR2, xác định sự chuyển đổi
	D3DXMatrixTransformation2D(&currentMatrix, &center, 0.0f, &rotate, NULL, 0.0f, NULL);

	// Cần nhân với ma trận cũ để tính ma trận biến đổi cuối cùng
	D3DXMATRIX finalMatrix = currentMatrix * tempMatrix;

	//Trạng thái flip đc thiết lập
	spriteHandler->SetTransform(&finalMatrix);

	//Vẽ
	spriteHandler->Draw(texture, &rect, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	//Chuyển về trạng thái ban đầu
	spriteHandler->SetTransform(&tempMatrix);
}


//Nhấm phím
int CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}


//Đè phím
int CGame::IsKeyPress(int KeyCode)
{
	return IsKeyDown(KeyCode) && !(keyEvents[KeyCode].dwData);
}


//Thả phím
int CGame::IsKeyRelease(int KeyCode)
{
	return !IsKeyDown(KeyCode) && (keyEvents[KeyCode].dwData);
}



void CGame::ProcessKeyboard()
{
	HRESULT hr;

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE*)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}



//Xét va chạm SweptAABB
//Một vật di chuyển và 1 vật đứng yên
void CGame::SweptAABB(
	float ml, float mt, float mr, float mb,  /*Vùng bao của vật đang di chuyển mà ta xét va chạm*/
	float dx, float dy,						 //Quãng đường đi được của vật đang di chuyển mà ta xét va chạm
	float sl, float st, float sr, float sb,  /*Vị trí vật đứng yên mà ta xét va chạm*/
	float& t, float& nx, float& ny			 /*Kết quả sau quá trình va chạm  theo trục X,Y*/
)
{
	float dx_entry, dy_entry;			//Khoảng cách cần để các vật bắt đầu va chạm
	float dx_exit, dy_exit;				//Khoảng cách cần đi khi hết va chạm
	float tx_entry, ty_entry;			//Thời gian cần để các vật bắt đầu va chạm
	float tx_exit, ty_exit;				//Thời gian để hết va chạm

	// thời gian va chạm là thời gian lớn nhất của 2 trục (2 trục phải cùng tiếp xúc thì mới va chạm)
	float t_entry;

	// thời gian hết va chạm là thời gian của 2 trục, (1 vật ra khỏi là object hết va chạm)
	float t_exit;

	t = -1.0f;		//Thời gian t không có va chạm (t<0: hai vật đang đi xa nhau)
	nx = ny = 0;	//Hướng va chạm (trả về khi không va chạm khi chúng =0)


	//Kiểm tra pha rộng :  
	//Chi phí thấp để kiểm tra nhanh chóng việc có hay không va chạm?
	//bl,bt, br, bb là các tọa độ tạm của movingObject để ta xét pha rộng
	float bl = dx > 0 ? ml : ml + dx;	//Trục X: Nếu Vật di chuyển về chiều dương thì giá trị left tạm = ml, nếu không thì left tạm=ml + dx
	float bt = dy > 0 ? mt : mt + dy;	//Trục Y: Nếu vật đang di chuyển về chiều dương (rơi xuống dưới) thì gán top tạm = mt , nếu không thì top tạm = mt+dy
	float br = dx > 0 ? mr + dx : mr;	//Trục X: Nếu vật đang di về chiều dương thì gán right tạm =  mr+dx, nếu không thì right tạm= mr
	float bb = dy > 0 ? mb + dy : mb;	//Trục Y: Nếu vật đang di chuyển về chiều dương (rơi xuống dưới) thì gán bottom tạm = mb+dy , nếu không thì bottom tạm = mb


	//Va chạm khi và chỉ khi xảy ra đồng thời trên 4 mặt của vùng bao 2 object (2 object chồng lên nhau)
	//Nếu 1 trong các mặt của vùng bao k va chạm => không có va chạm
	if (br < sl || bl > sr || bt > sb || bb < st)
		return;


	//Vật cần xét đang đứng yên => không cần xét va chạm
	if (dx == 0 && dy == 0)
		return;


	//XÉT TRÊN TRỤC X
	//Vật đang đi về chiều dương
	if (dx > 0)
	{
		dx_entry = sl - mr;	//khoảng cách cần để vật va chạm trên trục X (cạnh phải của vật di chuyển chạm cạnh trái của vật đứng yên)
		dx_exit = sr - ml;	//khoảng cách cần đi khi hết va chạm trên trục X (tổng chiều dài của hai vật và khoảng cách của chúng, đi qua vật va chạm)
	}
	//Vật đang về chiều âm
	else if (dx < 0)
	{
		dx_entry = sr - ml;	//(dấu âm)khoảng cách cần để vật va chạm trên trục X (cạnh phải của vật đứng yên chạm cạnh trái của vật di chuyển)
		dx_exit = sl - mr;	//(dấu âm)khoảng cách cần đi khi hết va chạm trên trục X (tổng chiều dài của hai vật và khoảng cách của chúng, đi qua vật va chạm)
	}

	//XÉT TRÊN TRỤC Y
	//Vật đang đi về chiều dương
	if (dy > 0)
	{
		dy_entry = st - mb;	//khoảng cách cần để vật va chạm trên trục Y (cạnh dưới của vật di chuyển chạm cạnh trên của vật đứng yên)
		dy_exit = sb - mt;	//khoảng cách cần đi khi hết va chạm trên trục Y (tổng chiều cao của hai vật và khoảng cách của chúng, đi qua vật va chạm)
	}
	//Vật đang đi về chiều âm
	else if (dy < 0)
	{
		dy_entry = sb - mt;	//(dấu âm)khoảng cách cần để vật va chạm trên trục Y (cạnh dưới của vật đứng yên chạm cạnh trên của vật di chuyển)
		dy_exit = st - mb;	//(dấu âm)khoảng cách cần đi khi hết va chạm trên trục Y (tổng chiều cao của hai vật và khoảng cách của chúng, đi qua vật va chạm)
	}


	//Vật đang đứng yên trên trục X, nhưng trên trục Y có di chuyển => không thể có va chạm trên trục X
	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	//Nếu không ta tính thời gian cần để bắt đầu va chạm trên trục X và thời gian kết thúc va chạm trên trục X
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}


	//Vật đang đứng yên trên trục Y => không thể có va chạm trên trục Y
	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	//Nếu không ta tính thời gian cần để bắt đầu va chạm trên trục Y và thời gian kết thúc va chạm trên trục Y
	else {
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}

	//t>1.0f Frame tiếp theo chưa thể có va chạm
	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;


	// thời gian va chạm là thời gian lớn nhất của 2 trục (2 trục phải cùng tiếp xúc thì mới va chạm)
	t_entry = max(tx_entry, ty_entry);

	// thời gian hết va chạm là thời gian của 2 trục, (1 cái ra khỏi là object hết va chạm)
	t_exit = min(tx_exit, ty_exit);

	//Thời gian kết thúc va chạm phải lớn hơn thời gian để va chạm (Vì va chạm xong rồi thì sau đó mới kết thúc va chạm)
	if (t_entry > t_exit) return;

	//Thời gian cần để va chạm
	t = t_entry;


	//Va chạm trên trục X
	if (tx_entry > ty_entry)
	{
		//Không va chạm trên trục Y
		ny = 0.0f;

		//Nếu vật đang di chuyển về chiều dương hướng va chạm trên trục x = phải nếu ko sẽ là trái
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	//Va chạm trên trục Y
	else
	{
		//Không va chạm trên trục X
		nx = 0.0f;

		//Nếu vật đang di chuyển về chiều dương hướng va chạm trên trục x = dưới nếu ko sẽ là trên
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}



//Kiểm tra 2 vật có va chạm với nhau từ trước đó hay không
////Cái này giải quyết việc SweepAABB không kiểm tra được
bool CGame::BroadPhaseTestAABB(float objectLeft, float objectTop, float objectRight, float objectBottom, float otherLeft, float otherTop, float otherRight, float otherBottom)
{
	//Thõa mãn 2 object có giao nhau trên cả trục X,Y thì nó chắc chắn va chạm nhau
	bool collisionX = objectRight >= otherLeft && otherRight >= objectLeft;
	bool collisionY = objectBottom >= otherTop && otherBottom >= objectTop;
	return collisionX && collisionY;

}




CGame* CGame::GetInstance()
{
	if (_instance == NULL) _instance = new CGame();
	return _instance;
}



CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}
