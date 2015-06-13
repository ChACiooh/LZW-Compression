#include "String.h"

void String::append(char c)
{
	if (size_ == alloc_-1){
		char* new_data = new char[++alloc_];
		for (int i = 0; i < size_; i++)	new_data[i] = data[i];
		if (alloc_)	delete[] data;
		data = new_data;
	}
	data[size_++] = c;
	data[size_] = 0; // 끝에는 null문자로.
}

const char* String::c_str() const
{
	return data;
}


/* operator overloadings */

String& String::operator=(const String& rvs)
{
	if (alloc_)
	{
		delete[] data;
		data = new char[alloc_];
	}
	size_ = rvs.size_;
	data = new char[alloc_ = rvs.alloc_];
	for (int i = 0; i <= size_; i++)	data[i] = rvs.data[i];
	return *this;
}

String& String::operator=(const char* str)
{
	if (alloc_)
	{
		delete[] data;
		data = new char[alloc_];
		size_ = 0;
	}
	for (int i = 0; str[i] != '\0'; i++){
		this->append(str[i]); // 알아서 마지막엔 널 문자를 넣어 줌.
	}
	return *this;
}

bool operator==(const String& a, const String& b)
{
	if (a.length() != b.length())	return false;
	for (int i = 0; i < a.length(); i++) {
		if (a[i] != b[i])	return false;
	}
	return true;
}

bool operator!=(const String& a, const String& b)
{
	return !(a == b);
}

String operator+(const String& a, const String& b)
{
	String lvs(a);
	for (int i = 0; i < b.length(); i++){
		lvs.append(b[i]);
	}
	return lvs;
}

String operator+(const String& a, const char b)
{
	String lvs(a);
	lvs.append(b);
	return lvs;
}

std::ostream& operator<<(std::ostream& os, const String& k)
{
	for (int i = 0; i < k.length(); i++){
		os << k[i];
	}
	return os;
}

bool operator>(const String& a, const String& b)
{
	if (a == b)	return false;
	int min_size = (a.length() < b.length() ? a.length() : b.length());
	int i;
	for (i = 0; i < min_size; i++)
	{
		if (a[i] > b[i])	return true;
		else if (a[i] < b[i])	return false;
	}

	if (min_size == a.length()) { // a가 b보다 더 앞선다.
		return false;
	}
	return true;
}

bool operator<(const String& a, const String& b)
{
	return !(a == b) && !(a > b);
}

// overloadings end