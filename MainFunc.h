#ifndef MAINFUNC_H_INCLUDED
#define	MAINFUNC_H_INCLUDED

#pragma once

#include "String.h"
#include "Vector.h"
#include "Hash.h"

union SaveSize // int�� size�� �����ϰ� char�� �ɰ��� �����ϱ� ���� union
{
	int size_;
	unsigned char gr[4];
};

void Input(Hash& myHash);
void Read(Hash&, Vector<String>&);
void Encode(Hash&, Vector<String>&);
void Decode();

#endif // !MAINFUNC_H_INCLUDED
