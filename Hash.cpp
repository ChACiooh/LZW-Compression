#include "Hash.h"

/* Data */
Data& Data::operator=(const Data& d)
{
	value = d.value;
	key = d.key;
	return *this;
}

int comp(const void* a, const void* b)
{
	return ((Data*)a)->key > ((Data*)b)->key;
}

/* construction */
Hash::Hash() : size_(0), alloc_(0), data(NULL) { }

Hash::Hash(const Hash& h) : size_(h.size_), alloc_(h.alloc_)
{
	data = new Data[alloc_];
	for (int i = 0; i < size_; i++){
		data[i] = h.data[i];
	}
}

Hash& Hash::operator=(const Hash& h)
{
	if (alloc_)	delete[] data;
	size_ = h.size_;
	alloc_ = h.alloc_;
	data = new Data[alloc_];
	for (int i = 0; i < size_; i++){
		data[i] = h.data[i];
	}
	return *this;
}

/* methods */
void Hash::resize(int n)
{
	if (n > alloc_)
	{
		Data* nd = new Data[n];
		for (int i = 0; i < size_; i++){
			nd[i] = data[i];
		}
		if (alloc_)	delete[] data;
		data = nd;
		alloc_ = n;
	}
	else if (n < alloc_)	size_ = n; // 잉여 부분은 dealing 안 함
}

void Hash::push(String key)
{
	if (size_ == alloc_)	resize(alloc_ + 1);
	Data d;
	d.key = key;
	d.value = size_;
	data[size_++] = d;
	//sort();
}

/// <summary>
///	value를 return한다. 못 찾으면 -1
/// </summary>
int Hash::Find(const String& str) const
{
	for (int i = 0; i < size_; i++)
	{
		if (str == data[i].key)	return data[i].value;
	}
	return NOT_FOUND;
}

String Hash::Find_s(int val) const
{
	String res = "";
	for (int i = 0; i < size_; i++){
		if (data[i].value == val) {
			res = data[i].key;
			break;
		}
	}
	return res;
}

int Hash::size() const
{
	return size_;
}

Data& Hash::operator[](int idx)
{
	return data[idx];
}

void Hash::sort()
{
	qsort(data, size_, sizeof(Data), comp);
	for (int i = 0; i < size_; i++){
		data[i].value = i;
	}
	return;
}