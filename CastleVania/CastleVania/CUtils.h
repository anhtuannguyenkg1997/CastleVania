#pragma once

#include <Windows.h>

#include <signal.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>


using namespace std;

//Cắt một chuỗi thành nhiều chuỗi dựa theo kí tự phân tách bất kì
vector<string> split(string line, string delimeter = "\t");

//Ép  kiểu string về wstring
wstring ToWSTR(string st);

//Ép  kiểu string về LPCWSTR
LPCWSTR ToLPCWSTR(string st);

