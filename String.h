#ifndef STRING_H_INCLUDED
#define	STRING_H_INCLUDED

	#ifndef NULL
	#define	NULL	0x0
	#endif
#include <iostream>

/**
* String�� ���, ���� String�� �����ϱ� ����
* �ݵ�� ������ ���ڴ� '\0'���� �ʱ�ȭ �� �ִ� ������ ������ ���Ҵ�.
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
	void append(char c); // ���� ���̱�
	int length() const { return size_; }
	const char* c_str() const; // %s�� ����� �ϱ� ���� ���� �Լ�.

	/* operator overloadings */
	String& operator=(const String& rvs);
	String& operator=(const char* str);
	char operator[](int idx) const { return data[idx]; } // r-value�� ��ȯ.
	
private:
	int size_;
	int alloc_;
	char* data; // char �迭
};

bool operator==(const String& a, const String& b);
bool operator!=(const String& a, const String& b);
String operator+(const String& a, const String& b);
String operator+(const String& a, const char b);
std::ostream& operator<<(std::ostream&, const String&);
bool operator>(const String& a, const String& b);
bool operator<(const String& a, const String& b);

#endif // STRING_H_INCLUDED