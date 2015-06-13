#include "MainFunc.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

/**
* �� ���Ͽ����� ���̴� ��ü.
* �ʱ� Hashtable�� �����ϰ� ������ ũ�� ���� ������ �д�.
*/
Hash initHash;
int buf_size;
double original_byte, after_byte; // ���� ������ �ڵ����� 0���� �ʱ�ȭ

void Input(Hash& myHash)
{
	FILE *fp = fopen("input.txt", "rb");
	char temp; 
	puts("�Էµ� ���ڿ�");
	while (!feof(fp) && fscanf(fp, "%c", &temp) != EOF)
	{
		if (myHash.Find(temp) == NOT_FOUND)
			myHash.push(temp);
		printf("%c", temp);
		original_byte += 1.0;
	}
	puts("");
	fclose(fp);
	myHash.sort(); // ����
	initHash = myHash; // overloading ����!
	puts("");
} // Input end

void Read(Hash& myHash, Vector<String>& myVec)
{
	FILE *fp = fopen("input.txt", "rb");
	char temp;
	String str;
	while (!feof(fp) && fscanf(fp, "%c", &temp) != EOF)
	{
		String tmp_str(str + temp);
		if (myHash.Find(tmp_str) == NOT_FOUND)
		{
			myHash.push(tmp_str);
			myVec.push_back(str); // encode �޸��ϴ� �κ�.
			str = ""; // reset
		}
		str.append(temp);
	}
	myVec.push_back(str);
	fclose(fp);

	/* �Էµ� �� Ȯ���غ���. */
	printf("�ϼ��� Hash : \n");
	for (int i = 0; i < myHash.size(); i++) {
		printf("%5s ", myHash[i].key.c_str());
		if ((i+1) % 5 == 0)	std::cout << std::endl;
	}
	std::cout << std::endl;
} // Read end

void Encode(Hash& myHash, Vector<String>& myVec)
{
	// �ִ��� ���ϸ� �װ����κ��� buffer size�� ���Ѵ�.
	int max = myHash.Find(myVec[0]);
	for (int i = 1; i < myVec.size(); i++)
	{
		int value = myHash.Find(myVec[i]);
		if (max <= value)	max = value;
	}
	buf_size = (int)log2((double)max) + 1 ;
	const int buf_init_pos = buf_size - 1; // ��Ʈ ������ ������ �κ�
	printf("max val:%d -> max bit size:%d\n", max, buf_size); // Ȯ��

	// ����! �����ϴ� ����!
	size_t shifting = 1;
	Vector<unsigned char> result;
	FILE *ofp = fopen("output.txt", "wb");
	for (int i = 0; i < myVec.size(); i++)
	{
		int value = myHash.Find(myVec[i]);
		if (value == NOT_FOUND)	continue; // �ٵ� �̰� ����� �� ����

		int shift = buf_init_pos;
		for (size_t j = (shifting << shift); shift >= 0; j = (shifting << --shift)) {
			result.push_back((j & value) >> shift); // ���⼭ bit size��ŭ�� set�� ��� �־��ش�.
		}
	}
	
	// ����� 32bit �����ͷ� ����� �Է��ص���.
	SaveSize ss;
	ss.size_ = result.size();
	for (int i = 3; i >= 0; i--){
		fprintf(ofp, "%c", ss.gr[i]);
	}
	// ����� result�� ���������.
	unsigned char buf = 0x0;
	shifting = 1;
	for (int i = 0; i < result.size(); i++)
	{
		buf |= result[i];
		if (shifting == 8) // char ũ�⸸ŭ shifting �Ǿ��� ��.
		{
			shifting = 1;
			fprintf(ofp, "%c", buf);
			buf = 0x0;
			after_byte += 1.0;
		}
		else
		{
			buf <<= 1;
			shifting++;
		}
	}
	if (shifting != 1) // �׿����� ���� ���.
	{
		buf <<= (8 - shifting);
		fprintf(ofp, "%c", buf);
		after_byte += 1.0;
	}
	fclose(ofp);

	printf("���� ����!\n����� : %g", after_byte / original_byte * 100.0);
	puts("%");
} // Encode end

void Decode() // �Ű������� �ʿ� ����.
{
	FILE *ifp = fopen("output.txt", "rb");
	FILE *ofp = fopen("recoveredinput.txt", "w");
	unsigned char buf;
	const unsigned char check = 0x80;
	Vector<unsigned char> result;
	SaveSize ss;
	for (int i = 3; i >= 0; i--){
		fscanf(ifp, "%c", &ss.gr[i]); // bit size�� ��� ����
	}
	while (!feof(ifp) && fscanf(ifp, "%c", &buf) != EOF)
	{
		for (int i = 0; i < 8; i++)
		{
			result.push_back((buf & check) >> 7);
			ss.size_--; // bit �ϳ� �� ������ bit size�� �� �ش�.
			if (ss.size_ == 0)	break;
			buf <<= 1;
		}
	}
	fclose(ifp);

	puts("\n������ ���ڿ�");
	String str = "";
	size_t buf2 = 0;
	for (int i = 0; i < result.size(); i++)
	{
		int j;
		for (j = i; j < i+buf_size && j<result.size(); j++) // buf size ��ŭ ��Ʈ ������ �д´�.
		{
			buf2 |= result[j];
			if (j-i < buf_size - 1)	buf2 <<= 1;
		}
		i = j - 1;

		String now = initHash.Find_s(buf2);
		while (now == "") // �ƹ��� ���� ���� ���� ���� �ݺ�
		{
			str = str + str[0];
			initHash.push(str);
			now = initHash.Find_s(buf2);
		}
		fprintf(ofp, "%s", now.c_str());
		std::cout << now;
		if (str != ""){ // ó������ ���ο� ���ڿ��� ���� �ʴ´�.
			initHash.push(str + now[0]);
		}
		str = now;
		buf2 = 0;
	}
	fclose(ofp);
	std::cout << std::endl;
} // Decode end

/* �� �Ǿ��� ���� �����. �� ���� �˰����� ������� ���� �ٽ� ¥ ��.
void Decode()
{
	FILE *ifp = fopen("output.txt", "rb");
	FILE *ofp = fopen("recoveredinput.txt", "w");

	int temp, cnt = 0;
	size_t buf = 0;
	String str = "";
	while (!feof(ifp) && fscanf(ifp, "%1d", &temp) != EOF)
	{
		buf |= temp;
		cnt++;
		if (cnt == buf_size)
		{
			String now = initHash.Find_s(buf);
			while (now == "")
			{
				str = str + str[0];
				initHash.push(str);
				now = initHash.Find_s(buf);
			}
			fprintf(ofp, "%s", now.c_str());
			if (!(str == "")){
				initHash.push(str + now[0]);
			}
			str = now;
			buf = cnt = 0;
		}
		else buf <<= 1;
	}

	fclose(ofp);
	fclose(ifp);
}*/