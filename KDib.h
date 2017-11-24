// KDib.h
#include <windows.h>
#include <fstream>
#include <string>
using namespace std;
#define FILE_NAME "olen.bmp"

class KDib {
public:
	KDib();
	~KDib();
	BOOL CreateDib24(int w, int h, const char* fileName);
	void StoreDib24();
	BOOL LoadFromFile(const char* fileName);
	bool closeFile();
	void SetPixel(int x, int y, COLORREF color);
	int Draw(HDC hdc, int xDst, int yDst, int wDst, int hDst,
		int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop);
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	const char* GetError() { return error.c_str(); }

private:	
	int width;
	int height;
	int bytesPerLine;
	BITMAPFILEHEADER fileHead;   // заголовок растрового файла
	BITMAPINFOHEADER infoHead;   // заголовок информационного блока
	BITMAPINFOHEADER* pInfoHead;
	BYTE* aBitmapBits;           // массив пикселей
	int fileHeadSize;
	int infoHeadSize;
	int imageSize;
	string error;

	ifstream inpFile;
	ofstream outFile;
};