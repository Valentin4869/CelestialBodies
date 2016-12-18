#include "stdafx.h"
#include"Primitives.h"







Object3d::Object3d(char* path){
	Object3d();
	LoadOBJ(path);

}



void Object3d::LoadOBJ(char* path){
	
	HANDLE hFile;
	LARGE_INTEGER fileSize;
	unsigned char *Buffer;
	DWORD bytesRead=0;
	
	Vector3d *vGarbage, *vGarbage2;
	Face3d	*fGarbage;
		 int *iGarbage, *iGarbage2;

				for(int i=0; i<nFace; i++)
				{
					
						iGarbage=face[i].iVertex;
						iGarbage2=face[i].iVertexNormal;
						delete[] face[i].iVertex;
						delete[] face[i].iVertexNormal;
				}
				
				vGarbage=vertex; 
				vGarbage2=vertexNormal;
				fGarbage=face; 

				delete[] vGarbage;
				//delete[] vertexNormal; 
				delete[] vGarbage2;
				
				delete[] fGarbage;

	hFile= CreateFileA(path, GENERIC_READ, 0, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(!GetFileSizeEx(hFile,&fileSize))
		cout<<"Failed to open file!\n";
			cout<<"File size: "<<fileSize.QuadPart<<endl;

			
	Buffer=new unsigned char[fileSize.QuadPart+1];

	
	if(!ReadFile(hFile,Buffer,fileSize.QuadPart,&bytesRead,NULL))
		cout<<"Failed to read file!\n";
		
	Buffer[bytesRead]='\0';

	
	unsigned char d=10,//0x0A
				  b=32,// ' '
				  v=118, //'v'
				  n=110, // 'n'
				  f=102, // 'f'
				  s=115; // 's'

	unsigned char vc[]={d,v,b,'\0'};
	unsigned char fc[]={d,f,b,'\0'};
	unsigned char sc[]={d,s,b,'\0'};
	unsigned char vnc[]={d,v,n,b,'\0'};
	int vi=0, i1=0, i2=0;
	int vni=0;

	//for(int c=0; Buffer[c]!='\0'; c++)
		//cout<<Buffer[c];

	nVertex=CountStr(Buffer,vc);
	nFace=CountStr(Buffer,fc);
	nVertexNormal=CountStr(Buffer,vnc);

			

	

	vertex=new Vector3d[nVertex];
	face=new Face3d[nFace];
	vertexNormal=new Vector3d[nVertexNormal];

	
	

	cout<<"Found vertices: "<<nVertex<<endl;
	cout<<"Found faces: "<<nFace<<endl;
	cout<<"Found vertex normals: "<<nVertexNormal<<endl;

	unsigned char blank[]=" ";
	unsigned char bslash[]="/\0";
	unsigned char blank2[]={d,'\0'};
	unsigned char* substr[3];

		for(int i=0; i<nVertex; i++)
		{
			
			vi=FindStr(Buffer,vc,vi+1);

			//i1=FindStr(Buffer,blank,vi);
			//i2=FindStr(Buffer,blank,i1+1);

			FindNumeric(Buffer,vi,i1,i2);
			
			//cout<<"Getting...\n";
			substr[0]=Substr(Buffer,i1-1,i2+1);
			vertex[i].x=CharToNum(substr[0]);

			/*i1=FindStr(Buffer,blank,i2);
			i2=FindStr(Buffer,blank,i1+1);*/
			FindNumeric(Buffer,i2+1,i1,i2);
			
			//cout<<"Getting...\n";
			substr[1]=Substr(Buffer,i1-1,i2+1);
			vertex[i].y=CharToNum(substr[1]);

			//i1=FindStr(Buffer,blank,i2);
			//i2=FindStr(Buffer,blank,i1+1);
			FindNumeric(Buffer,i2+1,i1,i2);
			
			//cout<<"Getting...\n";
			substr[2]=Substr(Buffer,i1-1,i2+1);
			vertex[i].z=CharToNum(substr[2]);
			
								
		//	cout<<"Vertex "<<i<<": "<<vertex[i].x<<", "<<vertex[i].y<<", "<<vertex[i].z<<endl;
		}

		for(int i=0; i<nVertexNormal; i++)
		{
			vni=FindStr(Buffer,vnc,vni+1);

			i1=FindStr(Buffer,blank,vni);
			i2=FindStr(Buffer,blank,i1+1);
			//cout<<"Getting...\n";
			
			substr[0]=Substr(Buffer,i1,i2);
			vertexNormal[i].x=CharToNum(substr[0]);
			
			i1=FindStr(Buffer,blank,i2);
			i2=FindStr(Buffer,blank,i1+1);
			//cout<<"Getting...\n";
			substr[1]=Substr(Buffer,i1,i2);
			vertexNormal[i].y=CharToNum(substr[1]);

			i1=FindStr(Buffer,blank,i2);
			i2=FindStr(Buffer,blank,i1+1);
			//cout<<"Getting...\n";
			substr[2]=Substr(Buffer,i1,i2);
			vertexNormal[i].z=CharToNum(substr[2]);

			

			//cout<<"Vertex normal"<<i<<": "<<vertexNormal[i].x<<", "<<vertexNormal[i].y<<", "<<vertexNormal[i].z<<endl;
		}

		int nFaceV=0;
		int fi=0, fe=0;
		int fi2=0, fe2=0;
		int tempi=0;
		i1=0;

		for(int i=0; i<nFace; i++)
		{	
			
			//find face line between current fc, to next fc
			fi=FindStr(Buffer,fc,fi+1);
			fe=FindStr(Buffer,fc,fi+1);
			if(fe==-1)
				fe=fileSize.QuadPart-1;
			
			substr[0]=Substr(Buffer,fi,fe);
			tempi=FindStr(substr[0],sc);	
			
			//delete[] substr[0];

			if(tempi>-1)
			{tempi+=fi; fe=tempi-1;}
			
			//count vertices for this face

			nFaceV=CountStr(Buffer,blank,fi,fe);
			
			
			face[i].nFaceV=nFaceV;
			face[i].iVertex=new int[nFaceV];
			face[i].iVertexNormal=new int[nFaceV];
			
			//parse vertex index for this face
			//fi2=0;
			//fi2=FindStr(Buffer,fc,fi+1);
			i1=fi;
		
						for(int c=0; c<face[i].nFaceV; c++)
								{
									i1=FindStr(Buffer,blank,i1+1);
									i2=FindStr(Buffer,bslash,i1+1);
									//cout<<"i1: "<<i1<<", i2: "<<i2<<endl;

									substr[0]=Substr(Buffer,i1,i2);
									face[i].iVertex[c]=CharToNum(substr[0]);
						

									i1=i2;
									i2=FindStr(Buffer,bslash,i1+1);
									i1=i2;
									if(c+1==face[i].nFaceV)
									i2=FindStr(Buffer,blank2,i1+1);
									else
										i2=FindStr(Buffer,blank,i1+1);

									if(i2==-1)
										i2=fileSize.QuadPart-1;
									//cout<<"i1: "<<i1<<", i2: "<<i2<<endl;
									//cout<<"vn substr: "<<Substr(Buffer,i1,i2)<<endl;
						
									substr[1]=Substr(Buffer,i1,i2);
									face[i].iVertexNormal[c]=CharToNum(substr[1]);

									//for(int ic=0; ic<2; ic++)
									//	delete[] substr[ic];
								//	cout<<face[i].iVertexNormal[c]<<", ";
								}
			//cout<<"\n";
			
			
		}

		
		float factor=1.5;
		for(int i=0; i<nVertex; i++)
	{vertex[i].x/=factor;
		vertex[i].y/=factor;
			vertex[i].z/=factor;}
		
	

	delete[] Buffer;
	//ClearBin();
	//system("pause");
	CloseHandle(hFile);
}


