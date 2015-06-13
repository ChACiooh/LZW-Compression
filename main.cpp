#include <windows.h>
#include "MainFunc.h"

using namespace std;

int main()
{
	Hash myHash;
	Vector<String> myVec;
	Input(myHash);
	Read(myHash, myVec);
	Encode(myHash, myVec);
	cout << "압축 해제를 ";
	system("PAUSE"); // 압축률 확인
	Decode();
	system("PAUSE");
	return 0;
}