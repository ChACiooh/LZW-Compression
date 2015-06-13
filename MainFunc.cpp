#include "MainFunc.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

/**
* 이 파일에서만 쓰이는 객체.
* 초기 Hashtable을 저장하고 버퍼의 크기 또한 저장해 둔다.
*/
Hash initHash;
int buf_size;
double original_byte, after_byte; // 전역 변수는 자동으로 0으로 초기화

void Input(Hash& myHash)
{
	FILE *fp = fopen("input.txt", "rb");
	char temp; 
	puts("입력된 문자열");
	while (!feof(fp) && fscanf(fp, "%c", &temp) != EOF)
	{
		if (myHash.Find(temp) == NOT_FOUND)
			myHash.push(temp);
		printf("%c", temp);
		original_byte += 1.0;
	}
	puts("");
	fclose(fp);
	myHash.sort(); // 정렬
	initHash = myHash; // overloading 했음!
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
			myVec.push_back(str); // encode 메모하는 부분.
			str = ""; // reset
		}
		str.append(temp);
	}
	myVec.push_back(str);
	fclose(fp);

	/* 입력된 후 확인해보자. */
	printf("완성된 Hash : \n");
	for (int i = 0; i < myHash.size(); i++) {
		printf("%5s ", myHash[i].key.c_str());
		if ((i+1) % 5 == 0)	std::cout << std::endl;
	}
	std::cout << std::endl;
} // Read end

void Encode(Hash& myHash, Vector<String>& myVec)
{
	// 최댓값을 구하면 그것으로부터 buffer size를 구한다.
	int max = myHash.Find(myVec[0]);
	for (int i = 1; i < myVec.size(); i++)
	{
		int value = myHash.Find(myVec[i]);
		if (max <= value)	max = value;
	}
	buf_size = (int)log2((double)max) + 1 ;
	const int buf_init_pos = buf_size - 1; // 비트 연산을 시작할 부분
	printf("max val:%d -> max bit size:%d\n", max, buf_size); // 확인

	// 본격! 압축하는 과정!
	size_t shifting = 1;
	Vector<unsigned char> result;
	FILE *ofp = fopen("output.txt", "wb");
	for (int i = 0; i < myVec.size(); i++)
	{
		int value = myHash.Find(myVec[i]);
		if (value == NOT_FOUND)	continue; // 근데 이거 실행될 리 없음

		int shift = buf_init_pos;
		for (size_t j = (shifting << shift); shift >= 0; j = (shifting << --shift)) {
			result.push_back((j & value) >> shift); // 여기서 bit size만큼의 set를 끊어서 넣어준다.
		}
	}
	
	// 여기다 32bit 데이터로 사이즈를 입력해두자.
	SaveSize ss;
	ss.size_ = result.size();
	for (int i = 3; i >= 0; i--){
		fprintf(ofp, "%c", ss.gr[i]);
	}
	// 여기다 result로 출력해주자.
	unsigned char buf = 0x0;
	shifting = 1;
	for (int i = 0; i < result.size(); i++)
	{
		buf |= result[i];
		if (shifting == 8) // char 크기만큼 shifting 되었을 때.
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
	if (shifting != 1) // 잉여항이 있을 경우.
	{
		buf <<= (8 - shifting);
		fprintf(ofp, "%c", buf);
		after_byte += 1.0;
	}
	fclose(ofp);

	printf("압축 성공!\n압축률 : %g", after_byte / original_byte * 100.0);
	puts("%");
} // Encode end

void Decode() // 매개변수가 필요 없다.
{
	FILE *ifp = fopen("output.txt", "rb");
	FILE *ofp = fopen("recoveredinput.txt", "w");
	unsigned char buf;
	const unsigned char check = 0x80;
	Vector<unsigned char> result;
	SaveSize ss;
	for (int i = 3; i >= 0; i--){
		fscanf(ifp, "%c", &ss.gr[i]); // bit size를 얻는 과정
	}
	while (!feof(ifp) && fscanf(ifp, "%c", &buf) != EOF)
	{
		for (int i = 0; i < 8; i++)
		{
			result.push_back((buf & check) >> 7);
			ss.size_--; // bit 하나 쓸 때마다 bit size를 빼 준다.
			if (ss.size_ == 0)	break;
			buf <<= 1;
		}
	}
	fclose(ifp);

	puts("\n복원된 문자열");
	String str = "";
	size_t buf2 = 0;
	for (int i = 0; i < result.size(); i++)
	{
		int j;
		for (j = i; j < i+buf_size && j<result.size(); j++) // buf size 만큼 비트 정보를 읽는다.
		{
			buf2 |= result[j];
			if (j-i < buf_size - 1)	buf2 <<= 1;
		}
		i = j - 1;

		String now = initHash.Find_s(buf2);
		while (now == "") // 아무런 값도 없는 동안 무한 반복
		{
			str = str + str[0];
			initHash.push(str);
			now = initHash.Find_s(buf2);
		}
		fprintf(ofp, "%s", now.c_str());
		std::cout << now;
		if (str != ""){ // 처음에는 새로운 문자열을 넣지 않는다.
			initHash.push(str + now[0]);
		}
		str = now;
		buf2 = 0;
	}
	fclose(ofp);
	std::cout << std::endl;
} // Decode end

/* 잘 되었을 때의 저장용. 이 때의 알고리즘을 기반으로 위에 다시 짜 둠.
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