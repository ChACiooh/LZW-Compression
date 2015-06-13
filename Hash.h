#ifndef HASH_H_INCLUDED
#define	HASH_H_INCLUDED

#pragma once

#include "String.h"
#include <stdlib.h>

	#ifndef NOT_FOUND
	#define	NOT_FOUND	-1
	#endif // !NOT_FOUND

struct Data
{
	Data& operator=(const Data& d);
	int value;
	String key;
};

int comp(const void* a, const void* b);

class Hash
{
public:
	Hash();
	Hash(const Hash& h);
	Hash& operator=(const Hash& h);

	void resize(int);
	void push(String key);
	int Find(const String&) const;
	String Find_s(int) const;
	int size() const;
	void sort();

	Data& operator[](int idx);
private:
	int size_;
	int alloc_;
	Data* data;
};

#endif // !HASH_H_INCLUDED
