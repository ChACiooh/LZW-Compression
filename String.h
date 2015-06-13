#ifndef STRING_H_INCLUDED
#define	STRING_H_INCLUDED

	#ifndef NULL
	#define	NULL	0x0
	#endif
#include <iostream>

/**
* String의 경우, 실제 String을 구현하기 위해
* 반드시 마지막 글자는 '\0'으로 초기화 해 주는 과정을 구현해 놓았다.
*/

class String
{
public:
	String() : size_(0), alloc_(1), data(new char[1]) { data[0] = 0; }
	String(const String& k) : size_(k.size_), alloc_(k.alloc_)
	{
		data = new char[alloc_];
		for (int i = 0; i < size_; i++)	data[i] = k.data[i];
		data[size_] = 0;
	}
	String(char c) : size_(1), alloc_(2)
	{
		data = new char[alloc_];
		data[0] = c;
		data[1] = 0;
	}
	String(const char* str)
	{
		int i;
		for (i = 0; str[i] != '\0'; i++);
		size_ = i;
		alloc_ = size_ + 1;
		data = new char[alloc_];
		for (i = 0; i < size_; i++)	data[i] = str[i];
		data[size_] = 0;
	}
	~String() { if (alloc_)	delete[] data; }
	void append(char c); // 글자 붙이기
	int length() const { return size_; }
	const char* c_str() const; // %s로 출력을 하기 위해 만든 함수.

	/* operator overloadings */
	String& operator=(const String& rvs);
	String& operator=(const char* str);
	char operator[](int idx) const { return data[idx]; } // r-value로 반환.
	
private:
	int size_;
	int alloc_;
	char* data; // char 배열
};

bool operator==(const String& a, const String& b);
bool operator!=(const String& a, const String& b);
String operator+(const String& a, const String& b);
String operator+(const String& a, const char b);
std::ostream& operator<<(std::ostream&, const String&);
bool operator>(const String& a, const String& b);
bool operator<(const String& a, const String& b);

#endif // STRING_H_INCLUDED