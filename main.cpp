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
	cout << "���� ������ ";
	system("PAUSE"); // ����� Ȯ��
	Decode();
	system("PAUSE");
	return 0;
}