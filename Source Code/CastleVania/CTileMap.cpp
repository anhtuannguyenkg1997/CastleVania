#include "CTileMap.h"
#include "CCamera.h"
#include "RosaryItemEffect.h"

CTileMap* CTileMap::_instance = NULL;


CTileMap* CTileMap::GetInstance()
{
	if (_instance == NULL) _instance = new CTileMap();
	return _instance;
}



//Khởi tạo TileMap
void CTileMap::LoadMap(LPCWSTR tilemapTexture, LPCWSTR tilemapCoordinates, int mapWidth, int mapHeight)
{
	//KHỞI TẠO MAP
	this->tilemapTexture = tilemapTexture;	//path Tilemap image
	this->tilemapCoordinates = tilemapCoordinates;//path Tilemap coordinates

	this->mapWidth = mapWidth;		//Chiều dài của map
	this->mapHeight = mapHeight;		//Chiều cao của map

	rows = mapHeight / TILE_HEIGHT;	//Tổng số dòng blocktile của map
	cols = mapWidth / TILE_WIDTH;		//Tổng số cột blocktile của map


	// LOAD TẤT CẢ BLOCK TILE LƯU VÀO MẢNG SPRITES
	//Lấy thể hiện của Texture
	CTextures* texture = CTextures::GetInstance();

	//Thêm một texture map mới vào danh sách textures
	texture->Add(ID_TEX_TILEMAP, tilemapTexture, TILEMAP_COLOR_TRANS);


	//Lấy texture map từ instance texture
	LPDIRECT3DTEXTURE9 tilemapTex = texture->Get(ID_TEX_TILEMAP);

	// lấy thông tin về kích thước của texture lưu các block tiles (filePath_tex)

	//Mô tả một surface
	D3DSURFACE_DESC surfaceDescription;


	//Lấy một mô tả cấp độ của một texture resource
	//level: Xác định một mức độ của texture resource. Phương thức này trả về một mô tả bề mặt cho level được xác định bởi tham số này
	//surfaceDesc: Con trỏ tới cấu trúc D3DSURFACE_DESC, mô tả level được trả về.
	tilemapTex->GetLevelDesc(0, &surfaceDescription);

	// tính toán số hàng, số cột cần thiết để đọc các ô tile từ file ảnh texture map
	int rowTileMapTexture = surfaceDescription.Height / TILE_HEIGHT;
	int colTileMapTexture = surfaceDescription.Width / TILE_WIDTH;

	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	// block tile đầu tiên sẽ có id = blocktile0. Tăng "blocktile"+1 đơn vị cho các block tile tiếp theo
	int spriteID = 0;


	//Duyệt qua các dòng
	for (UINT i = 0; i < rowTileMapTexture; i++)
	{
		//Duyệt qua các cột
		for (UINT j = 0; j < colTileMapTexture; j++)
		{
			//ID của block tile hiện tại VD: map_1_tile_1
			string idTile = "blocktile" + to_string(spriteID);

			//Thêm block tile này vào mảng sprites (sau này dùng để vẽ lên màn hình)
			//với ID tile vừa tạo
			//Và Vị trí của block tile này trong texture map 
			sprites->Add(idTile,	//BlockTile ID
						  TILE_WIDTH * j,		//left
						  TILE_HEIGHT * i,		//top
						  TILE_WIDTH + TILE_WIDTH * j,	//right
				          TILE_HEIGHT+TILE_HEIGHT * i,	//bottom
						  tilemapTex);

			//Tăng id sprite lên 1 để không trùng ID
			spriteID += 1;
		}
	}
	
	//DebugOut(L"rowTileMapTexture: %d, colTileMapTexture:%d \n", rowTileMapTexture, colTileMapTexture);
	//DebugOut(L"row: %d, col:%d ", rows, cols);




	//LOAD MA TRẬN MAP
	//Mở file text map để load
	fstream fs;
	fs.open(tilemapCoordinates);


	//Lưu tạm 1 dòng trong file text
	string line;


	//Đọc từng dòng trong file đến cuối file
	while (!fs.eof())
	{
		//Lấy một dòng ra
		getline(fs, line);

		//Một dòng text tương ứng vói một mảng block tile (mảng sprite)
		//mảng này chính là 1 row trong map
		vector<LPSPRITE> rowTile;

		//Đưa dữ liệu line vào stringstream
		stringstream ss(line);


		//Lưu tạm id block tile (chỉ là số từ map txt)
		int n;


		//Truy xuất dữ liệu trong streamstring . Nếu tìm thấy bất cứ dữ liệu nào kiểu integer. lập tức gán cho n
		///https://www.geeksforgeeks.org/extract-integers-string-c/
		while (ss >> n)
		{
			//Tạo id tile.  để map với id tile lưu trong instance sprites
			string idTile = "blocktile" + to_string(n);

			//Đưa vào trong mảng tile block tile
			rowTile.push_back(sprites->Get(idTile));
		}
		//Thêm 1 dòng tile block vào tilemap (đến đây ta có 1 dòng hoàn chỉnh cho map). Nhiều dòng sẽ tạo thành 1 map hoàn thiện
		tilemap.push_back(rowTile);
	}

	//Đóng file
	fs.close();
	// 

}




void CTileMap::Render()
{
	CCamera* camera = CCamera::GetInstance();
	RosaryItemEffect* rosary = RosaryItemEffect::GetInstance();

	//Chỉ vẽ những tileblock nằm trong camera
	//Cột bắt đầu vẽ
	int from = (int)camera->GetCameraPositon().x / TILE_WIDTH;

	//Cột kết thúc vẽ
	int to = ceil((camera->GetCameraPositon().x + CAMERA_WIDTH) / TILE_WIDTH);


	//Random giá trị alpha
	int random = rand() % (2) + 0;
	int alpha = 255;
	if (random == 0) {
		alpha = 0;
	}
	else {
		alpha = 250;
	}

	//Vẽ tất cả các hàng
	for (UINT i = 0; i < rows; i++)
	{

		//Vẽ số lượng cột nhất định (trong vùng camera)
		for (UINT j = from; j < to; j++)
		{
			//Tọa độ trong thế giới game
			float x = TILE_WIDTH * (j - from) + camera->GetCameraPositon().x - (int)camera->GetCameraPositon().x % TILE_WIDTH; ;
			float y = TILE_HEIGHT * i;


			//Vẽ một blocktile lên màn hình, set accordingcamera, not flip trên tọa độ x,y
			//Nếu đang dùng rosary thì sẽ tạo hiệu ứng lấp lánh các tileblock bằng cách thay đổi tham số alpha
			if (rosary->isRosaryEffect && rosary->IsRosary()) {
				tilemap[i][j]->Draw(INSIDE_CAMERA, NOT_FLIPPING, x, y, alpha);
			}
			//Nếu không dùng rosary thì render tileblock như bình thường
			else {
				tilemap[i][j]->Draw(INSIDE_CAMERA, NOT_FLIPPING, x, y);
			}
			
		}
	}
}


//Clear map
void CTileMap::Clear() {

	//Duyệt hết mảng 2 chiều của tilemap
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			//Làm sạch từng tile
			tilemap[i][j] = NULL;
			delete tilemap[i][j];
		}

		//Làm sạch từng row
		tilemap[i].clear();
	}

	//Làm sạch mảng tilemap
	tilemap.clear();


	//DebugOut(L"Tilemap deleted %d \n", tilemap.size());

}