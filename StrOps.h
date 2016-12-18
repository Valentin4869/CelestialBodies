#pragma once

#include "stdafx.h"


const int l=200;	//default length
extern int dIndex;
extern int udIndex;
 const int bin_size=5000000;
extern unsigned char* uc_disposal[bin_size];
extern char* c_disposal[bin_size];
void NtoC(int num);
int cN(char nc);
void Dispose(char*);
void Dispose(unsigned char*);
void cClearBin();
void ucClearBin();
void ClearBin();
char* MixedString(char* str, float num);
double ToNum(char *nstr, short digits);
double ToNum(char *nstr);
double ToNum(unsigned char *nstr);
int CleanNumeric(unsigned char *nstr);
int CleanNumeric(char *nstr);
void Initialize(char* str, char ch);
char* NewStr(char* oldstr);
unsigned char* NewUStr(char* oldstr);
char* CleanString(int size);
char* CleanString();
int FindChar(unsigned char* str, int size, unsigned char ch);
int FindChar(char* str, int size, char ch);
bool CompareStr(char* str1, char* str2);
int FindLength(char* str);
int FindLength(char* str, char ch);
int FindLength(unsigned char* str);
int FindLength(unsigned char* str, unsigned char ch);
char* Substr(char* str, int beg, int end);
unsigned char* Substr(unsigned char* str, int beg, int end);
char* AfterStr(char* line, int size, char s, int from);
int FindStr(char* str, char* fstr, int from);
int FindStr(char* str, char* fstr);
int FindStr(unsigned char* str, unsigned char* fstr, int from);
int FindStr(unsigned char* str, unsigned char* fstr, int from, int to);
int FindStr(unsigned char* str, unsigned char* fstr);
int CountStr(char* str, char* cstr);
int CountStr(unsigned char* str, unsigned char* cstr);
int CountStr(unsigned char* str, unsigned char* cstr, int from, int to);
double CharToNum(unsigned char* str);
double CharToNum(char* str);
void PrintRange(char* buffer, int from, int to);
void PrintRange(unsigned char* buffer, int from, int to);
void FindNumeric( char* str,int from, int& i1, int&i2); 
void FindNumeric(unsigned char* str,int from, int& i1, int&i2); 
bool IsNumeric(unsigned char c);
bool IsNumeric(char c);

char* tochar(unsigned char* ustr);
unsigned char* touchar(char* ustr);