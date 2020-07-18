#include "CUtils.h"
#include <Windows.h>


//Cắt một chuỗi thành nhiều chuỗi dựa theo kí tự phân tách bất kì
vector<string> split(string line, string delimeter)
{
	vector<string> tokens; //Lưu lại kết quả sau khi split

	//size_t dùng để lưu size của một object. size_t được định nghĩa là kiểu không dấu có thể chứa kích thước của bất kì đối tượng nào.
	size_t last = 0; size_t next = 0;

	// phương thức find của lớp string để tìm kiếm vị trí xuất hiện của chuỗi con bên trong đối tượng mà bạn đang dùng để gọi phương thức find
	//delimeter là chuỗi cần tìm kiếm trong chuỗi line
	//last chính là vị trí bắt đầu tìm kiếm (mặc định là 0
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		//Nếu khác delimeter thì push cắt và push vào mảng tokens
		tokens.push_back(line.substr(last, next - last));

		//Tăng vị trí bắt đầu tìm kiếm lên
		last = next + 1;
	}

	//Cắt chuỗi còn lại
	tokens.push_back(line.substr(last));

	return tokens;
}

//Ép  kiểu string về wstring
//wstring là một dạng mở rộng của char có thể đáp ứng được ký tự Unicode.
//Cách này dùng để in chuỗi Unicode ra màn hình console mà không lỗi font chữ
wstring ToWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}


//Ép  kiểu string về LPCWSTR
//Viết tắt của "Long Pointer to Constant Wide String".
//LPCWSTR là một con trỏ 32 bit đến một chuỗi ký tự Unicode 16 bit không đổi
//Phổ biến cho bất kỳ mã C / C ++ nào phải xử lý các chuỗi chỉ không phải ASCII
LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}
