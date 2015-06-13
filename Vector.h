#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#pragma once

template<class T>
class Vector
{
public:
	Vector() : size_(0), alloc_(0), data(NULL) {}
	Vector(int size_);
	Vector(const Vector<T>& vec);

	void resize(int); // size를 조절.
	void push_back(T); // 
	void pop() { size_--; }
	int size() const { return size_; }
	const T* const begin() const { return data; }
	const T* const end() const { return data + size_; }

	/* operator overloadings */
	T& operator[](int i) { return data[i]; }
	Vector<T>& operator=(const Vector<T>&);

private:
	int size_;
	int alloc_;
	T* data;
};

template<class T>
Vector<T>::Vector(int size_)
{
	this.size_ = 0;
	alloc_ = size_;
	data = new T[size_];
}
template<class T>
Vector<T>::Vector(const Vector<T>& vec) : size_(vec.size_), alloc_(vec.alloc_)
{
	data = new T[alloc_];
	for (int i = 0; i < size_; i++){
		data[i] = vec.data[i];
	}
}
template<class T>
void Vector<T>::resize(int s)
{
	if (s == alloc_ || s < 0)	return;
	if (s < alloc_) size_ = s; // 기존의 size_ - s 만큼의 데이터는 신경 안씀
	else if (s > alloc_)
	{
		T* new_data = new T[alloc_ = s];
		for (int i = 0; i < size_; i++){
			new_data[i] = data[i];
		}
		delete[] data;
		data = new_data;
	}
}
template<class T>
void Vector<T>::push_back(T item)
{
	if (size_ == alloc_)	resize(alloc_ + 1); // 1칸 늘려주자.
	data[size_++] = item;
}
template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& vec)
{
	if (alloc_)	delete[] data;
	size_ = vec.size_;
	data = new T[alloc_ = vec.alloc_];
	for (int i = 0; i < size_; i++){
		data[i] = vec.data[i];
	}
	return *this;
}
#endif // VECTOR_H_INCLUDED