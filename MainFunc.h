#ifndef MAINFUNC_H_INCLUDED
#define	MAINFUNC_H_INCLUDED

#pragma once

#include "String.h"
#include "Vector.h"
#include "Hash.h"

union SaveSize // int에 size를 저장하고 char로 쪼개서 저장하기 위한 union
{
	int size_;
	unsigned char gr[4];
};

void Input(Hash& myHash);
void Read(Hash&, Vector<String>&);
void Encode(Hash&, Vector<String>&);
void Decode();

#endif // !MAINFUNC_H_INCLUDED
