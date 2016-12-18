#include "stdafx.h"
#include "StrOps.h"

int udIndex=0;
int dIndex=0;

unsigned char* uc_disposal[bin_size];
char* c_disposal[bin_size];
unsigned char* disp;

void Dispose(char* garbage){c_disposal[dIndex++]=garbage;}
void Dispose(unsigned char* garbage){uc_disposal[udIndex++]=garbage;}
void cClearBin(){for(int i=0; i<dIndex-1; i++) delete[] c_disposal[i]; dIndex=0;}
void ucClearBin(){for(int i=0; i<udIndex-1; i++) delete[] uc_disposal[i]; udIndex=0;}
void ClearBin(){cClearBin(); ucClearBin();}
void NtoC(int num)
{
	char temp[]="0000000000";
	short cpos=9;

	while(num>0)
	{
		temp[cpos]=48+num%10;
		cout<<temp<<endl;
		num/=10;
		cpos--;
	}

}
unsigned char* NewUStr(char* oldstr){
	unsigned char *NEWSTR;
	int size=strlen(oldstr)+1;
	NEWSTR=new unsigned char[size];
		
		for(int i=0; i<size; i++)
			NEWSTR[i]=oldstr[i];

		//NEWSTR[strlen(path)]='\0';
		return NEWSTR;
}
char* NewStr(char* oldstr)
{
	char *NEWSTR;
	int size=strlen(oldstr)+1;
	NEWSTR=new char[size];
		
		for(int i=0; i<size; i++)
			NEWSTR[i]=oldstr[i];

		//NEWSTR[strlen(path)]='\0';
		return NEWSTR;
}

int FindChar(char* str, int size, char ch)
{//first occurence of ch
	int INDEX;
	for(int i=0; i<size; i++)
	{
		if(str[i]==ch)
			return i;
	}
	return -1;
}

int FindChar(unsigned char* str, int size, unsigned char ch)
{//first occurence of ch
	int INDEX;
	for(int i=0; i<size; i++)
	{
		if(str[i]==ch)
			return i;
	}
	return -1;
}

bool CompareStr(char* str1, char* str2)
{
	//cout<<"Comparing Str1:"<<str1<<", Str2: "<<str2<<endl;
	if(strlen(str1)==strlen(str2))
	{//cout<<"Str1: "<<strlen(str1)<<", Str2: "<<strlen(str2)<<endl;
		for(int i=0; i<strlen(str1); i++)
		{
			if(str1[i]!=str2[i])
				{//cout<<"Strings not equal at "<<i<<endl; 
			
					return false;}

		}
		cout<<str1<<" = "<<str2<<endl;
		return true;
	}
	//cout<<"Strings not equal in length\n Str1: "<<str1<<", Str2: "<<str2<<endl;
	return false;
}


void Initialize(char* str, char ch)
{
for(int i=0; i<l; i++)
		str[i]=ch;
}

int FindLength(char* str)
{
	return FindLength(str, '\0');
}

int FindLength(char* str, char ch)
{
	for(int i=0; i<l; i++)
	{
		
		if(str[i]==ch)
			return i;
		
	}
}
int FindLength(unsigned char* str)
{
	return FindLength(str, '\0');
}

int FindLength(unsigned char* str, unsigned char ch)
{
	for(int i=0; i<l; i++)
	{
		
		if(str[i]==ch)
			return i;
		
	}
}
int cN(char nc)
{
	return int(nc)-48;
}

double ToNum(char *nstr, short digits)
{
	double N=0.0;

	
	for(int i=0; i<digits;i++)
	N+=cN(nstr[digits-1-i])*pow(10,(double)i);
	
return N;
}

int CleanNumeric(unsigned char *nstr)
{
	int position1=0, position2=0;
	
	while(nstr[position1]<48 || nstr[position1]>57)
	{position1++;}

	while(nstr[position2]>47 && nstr[position2]<58)
	{position2++;}

	nstr[position2]='\0';

	return position1;

}

int CleanNumeric(char *nstr)
{
	int position1=0, position2=0;
	
	while(nstr[position1]<48 || nstr[position1]>57)
	{position1++;}
position2=position1;
	while(nstr[position2]>47 && nstr[position2]<58)
	{position2++;}

	
	nstr[position2]='\0';

	return position1;

}

double CharToNum(char* str)
{
	return atof(str);

}

double CharToNum(unsigned char* str)
{
	return atof(tochar(str));
}

//char* MixedString(char* str, float num)
//{
//int lstr=strlen(str);
//int pos=0;
//for(int i=0; i<lstr; i++)
//	if(str[i]=='0')
//		pos=i;
//
// lstr= lstr+ strlen(NtoC(num));
// 
//
//
//}

//double CharToNum(unsigned char* str)
//{
//	int yin=dIndex;
//	double NUM=0.0;
//	bool negative=false;
//	int dlength=FindLength(str, '\0'); //length not counting null
//
//	if(str[0]=='-')
//		dlength--;
//	int ilength=dlength-1;
//	short dplace=FindChar(str,dlength,'.');
//	short decDigits=dplace-1;
//	
//
//	unsigned char *intchar=new unsigned char[ilength+1];
//	intchar[ilength]='\0';
//
//	//take out '.'
//	for(int i=0, j=0; i<dlength; i++)
//	{if(IsNumeric(str[i])){intchar[j]=str[i];j++;}}
//	cout<<"We have: "<<intchar<<endl;
//	
//
//	cout<<"<<<<<<<<<<< Converting "<<str<<">>>>>>>>>>>>\n";
//	if(negative)
//		NUM= -1*ToNum(str);
//	else
//		NUM= ToNum(str);
//
//	
//
//	cout<<"Converted to "<<NUM<<endl;
//	system("pause");
//	
//
//	return NUM;
//}

double ToNum(char *nstr)
{
	double N=0.0;
	int size=FindLength(nstr,'\0');
	bool negative=false;

	if(nstr[0]=='-')
	{size--;nstr++; negative=true;}
	

	short dplace=FindChar(nstr,size,'.');

	if(dplace>-1)
		{
			char *tempn;
			double restore=pow(10,double((size-dplace)-1));
			tempn=new char[size-2];
			
			
			for(int i=0, j=0; i<size; i++)
			{if(nstr[i]!='.'){tempn[j]=nstr[i]; j++;}}

			tempn[size-1]='\0';
			
			
				return ToNum(tempn)/restore;

			
			delete[] tempn;

		
				return N;
		}

	for(int i=0; i<size;i++)
	{
		N+=cN(nstr[size-1-i])*pow(10,(double)i);
	}

	
	
	return N;
}

double ToNum(unsigned char *nstr)
{
	cout<<"[ENTRY]\n";

	double N=0.0;
	int size=FindLength(nstr,'\0');
	//bool negative=false;

	//if(nstr[0]=='-')
	//{size--;nstr++; negative=true;}
	//cout<<"received "<<nstr<<endl;
	//cout<<"size: "<<size<<endl;

	//

	//short dplace=FindChar(nstr,size+1,'.');
	//cout<<"dplace= "<<dplace<<endl;

	//if(dplace>-1)
	//	{
	//		cout<<"--- dplace>-1 ---\n";

	//		unsigned char *tempn;
	//		double restore=pow(10,double((size-dplace)-1));

	//		cout<<"restore: "<<restore<<endl;

	//		tempn=new unsigned char[size-2];
	//		
	//		for(int i=0, j=0; i<size; i++)
	//		{if(nstr[i]!='.'){tempn[j]=nstr[i]; j++;}}

	//		

	//		tempn[size-1]='\0';
	//		cout<<"tempn: "<<tempn<<endl;
	//		cout<<"put null at "<<size-1<<endl;

	//		disp=tempn;

	//	Dispose(tempn);
	//		//cout<<"Put "<<tempn<<" at "<<dIndex-1<<endl;
	//		cout<<"**>>Returning "<<ToNum(tempn)/restore<<endl;
	//		
	//			return ToNum(tempn)/restore;

	//	}

	cout<<"--- dplace<-1 ---\n";
	for(int i=0; i<size;i++)
	{
		N+=cN(nstr[size-1-i])*pow(10,(double)i);
	}

	
	cout<<"**>> returning "<<N<<endl;

	return N;
}

char* CleanString(int size)
{
	char* NEW;
	NEW=new char[size]; 
	for(int i=0; i<size; i++)
		NEW[i]='\0';
	
	return NEW;
}



char* CleanString()
{
	return CleanString(l);
}

//exclusive between beg and end
char* Substr(char* str, int beg, int end)
{
	int sublength=(end-beg)-1;
	char* substr;
	substr=new char[sublength+1];
	beg++;//turn to begin of substr

	for(int i=0; i<sublength; i++)
	substr[i]=str[beg+i];
	
	substr[sublength]='\0';
	Dispose(substr);
	return substr;
}

unsigned char* Substr(unsigned char* str, int beg, int end)
{
	int sublength=(end-beg)-1;
	unsigned char* substr;
	
	substr=new unsigned char[sublength+1];
	beg++;//turn to begin of substr

	for(int i=0; i<sublength; i++)
	substr[i]=str[beg+i];

	substr[sublength]='\0';
	
	
	Dispose(substr);
	return substr;

}

char* AfterStr(char* line, int size, char s, int from)
{
	char *substr;
	int beg=0;
	int end=0;
	bool foundend=false;

	for(int i=from; i<size; i++)
	{

		if(line[i]==s)
		{
			beg=i;
			//now find end;
			for(int j=i+1; j<size-i; j++)
			{
				if(line[j]==s)
				{end=j;foundend=true;break;}
			}
		}

		if(foundend)
		{
			
		substr=Substr(line,beg,end);
		return substr;
		
		}
	}

}

int FindStr(char* str, char* fstr, int from)
{
	for(int i=from, j=0; str[i]!='\0'; i++)
	{
		if(str[i]==fstr[j])
			j++;
		else
		{
			if(j>0)
				{i-=j; j=0;}
		}

		if(fstr[j]=='\0')
			return i-j+1;
	}

	return -1;
}

int FindStr(unsigned char* str, unsigned char* fstr, int from, int to)
{
	for(int i=from, j=0; str[i]!='\0'; i++)
	{
		if(str[i]==fstr[j])
			j++;
		else
		{
			if(j>0)
				{i-=j; j=0;}
		}

		if(fstr[j]=='\0')
			return i-j+1;
	}

	return -1;

}

int FindStr(unsigned char* str, unsigned char* fstr, int from)
{
	for(int i=from, j=0; str[i]!='\0'; i++)
	{
		if(str[i]==fstr[j])
			j++;
		else
		{
			if(j>0)
				{i-=j; j=0;}
		}

		if(fstr[j]=='\0')
			return i-j+1;
	}

	return -1;
}

int FindStr(unsigned char* str, unsigned char* fstr)
{
	return FindStr(str, fstr,0);
}

int FindStr( char* str,  char* fstr)
{
	return FindStr(str, fstr,0);
}

int CountStr(char* str, char* cstr)
{
	int lastfound=0;
	int found=0;

	while(lastfound>-1)
	{
		lastfound=FindStr(str,cstr,lastfound+1);
		
		if(lastfound>-1)
		found++;
	}
	
	return found;
}

int CountStr(unsigned char* str, unsigned char* cstr, int from, int to)
{
	int lastfound=0;
	int found=0;
	unsigned char* substr=Substr(str,from,to);

	while(lastfound>-1)
	{
		lastfound=FindStr(substr,cstr,lastfound+1);	
		
		if(lastfound>-1)
		{found++; }
	}
	
	
	return found;

}

int CountStr(unsigned char* str, unsigned char* cstr)
{
	int lastfound=0;
	int found=0;
	
	while(lastfound>-1)
	{
		lastfound=FindStr(str,cstr,lastfound+1);
		
		if(lastfound>-1)
			found++; 
	}
	
	return found;
}
bool IsNumeric(unsigned char c){

	if(c>47 && c<58)
	return true;

	return false;

}

bool IsNumeric(char c){

	
	if(c>47 && c<58)
	return true;

	return false;

}

void FindNumeric(  char* str,int from, int& i1, int& i2) 
{

	
	int c1=0, c2=0;
	int i=0;
	bool FoundNumeric=false;

	while( str[i]!='\0')
	{
		
		 if(!IsNumeric(str[i]))
			{ c1++; i++;}
		 else
		 {
			
			FoundNumeric=true;
			cout<<str[i-1]<<" != -"<<endl;
			if(i>0 && str[i-1]=='-')
			{c1--;cout<<"Now at "<<str[c1]<<endl;}

			c2=c1; i++; break;
			 
		 }
	}

	while( str[i]!='\0')
	{
		 if(IsNumeric(str[i]))
			{c2++; i++;}
		 else
		 {
			 break;		 
		 }
	}

	if(FoundNumeric)
	{
		i1=c1; i2=c2;
		
	}

	

}

void FindNumeric(unsigned  char* str,int from, int& i1, int& i2) 
{

	
	int c1=0, c2=0;
	int i=from;
	bool FoundNumeric=false;

	while( str[i]!='\0')
	{
		
		 
		if(!IsNumeric(str[i]))
			{ i++;}
		 else
		 {
				FoundNumeric=true;
				c1=i;
			

			if(i>0)
			{ 
				if(str[i-1]=='-' )
				{c1=i-1;}
				
			}
			 
			c2=i; i++; break;
			 
		 }
	}

	while( str[i]!='\0')
	{
		 if(IsNumeric(str[i]) || str[i]=='.')
			{c2++; i++;}
		 else
		 {
			 break;		 
		 }
	}

	if(FoundNumeric)
	{
		i1=c1; i2=c2;
		
	}

	

}

void PrintRange(unsigned char* buffer, int from, int to){

	cout<<"[";
	for(int i=0; i<to-from+1; i++)
		cout<<buffer[from+i];
	cout<<"]"<<endl;
}

void PrintRange( char* buffer, int from, int to){

	cout<<"[";
	for(int i=0; i<to-from+1; i++)
		cout<<buffer[from+i];
	cout<<"]"<<endl;
}

char* tochar(unsigned char* ustr){
	int length=FindLength(ustr)+1;
	char *NEWSTR= new char[length];

	for(int i=0; i<length; i++)
		NEWSTR[i]=ustr[i];
	Dispose(NEWSTR);
	return NEWSTR;
}

unsigned char* touchar(char* ustr){
	int length=FindLength(ustr)+1;
	unsigned char *NEWSTR= new unsigned char[length];

	for(int i=0; i<length; i++)
		NEWSTR[i]=ustr[i];
	Dispose(NEWSTR);
	return NEWSTR;
}