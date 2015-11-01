
#ifndef __PDF_CREATE_H__
#define __PDF_CREATE_H__

typedef struct 
{
	unsigned char ObjIndex;
	unsigned char ReWriteIndex;
}ObjectHeader;

typedef  struct
{
	ObjectHeader;
	char* ObjInfo;
}GeneralObject;
typedef struct
{
	unsigned long	ObjAddr;
	unsigned int	ReWriteIndex;
	char IsUsed;
}CrossRef;

typedef struct 
{
	char* ObjName;
	ObjectHeader objHeader;
	CrossRef* crossRef;
	int corssRefLenth;
}CrossRefTable;

typedef struct
{
	char* ObjName;
	unsigned int Addr;
}StartXref;

typedef struct
{
	char* ObjName;
  int Size;
	int RootObjIndex;
}Trailer;

typedef struct
{
	char* PdfVersion;
  unsigned char* externInf;
}FileHeader;

typedef struct 
{
	FileHeader fileHeader;
	
}PDF_FILE;




#endif