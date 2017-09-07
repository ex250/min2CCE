#include <windows.h>
#include <commctrl.h>
#include "resource.h"

#if !defined(HEADERUI)
#define HEADERUI

typedef long LONG_PTR;


class BaseWindow
{
  protected:
	HWND hWnd;
	const WNDCLASS  *WindowClass;	
	DWORD dwStyle;
	int xPos;
	int yPos;
	int width;
	int height;
	HBRUSH hBrush;
	HPEN hPen;
	XFORM xForm; 

  public:

	BaseWindow();

	HWND getHWND();

	bool init(LPCTSTR windowName);

	bool init(LPCTSTR windowName,
	 	  DWORD dwStyle,
		  int x,
		  int y,
		  int cx,
		  int cy,
		  HWND hwndParent,
		  HMENU hMenu,
		  HINSTANCE hInst);

	ATOM registerClass(WNDCLASS&);

	bool show(int);
        int  getWidth();
        int  getHeight();
	bool setBrush(HBRUSH*);
	bool setPen(HPEN*);
	bool SetWindowPosition(int x,int y,int cx,int cy);

	bool printText(int , int ,char*);

	bool lineTo(int ,int );

	bool line(int ,int, int, int );

	bool moveTo(int ,int );

	RECT getRect();

};

//---------------------TOOLBAR-----------------------------
class toolbarDraw:public BaseWindow
{
  protected:

    HIMAGELIST 	hImageList;         
    int 	cx,cy;
    UINT 	flag;
    int 	cInitial;
    int		cGrow;

    const int ImageListID;

 public:
    toolbarDraw();

    void setStyle(DWORD style);
    void setButtonSize(int ,int );

    int initSTD(  HWND hWndParent,
			LPCTSTR windowName,
			const int cInitial,
			TBBUTTON *tbButtons
			);


    int init(  HWND hWndParent,
			LPCTSTR windowName,
			const int cInitial,
			HBITMAP hBmpToolbar,
			TBBUTTON *tbButtons
			);

    void setPos(int,int);
    void setSize(int,int);
};

//---------------COMMAND LINE-------------------------------------------
class CommandLine:public BaseWindow
{
  private:
    static WNDPROC wpOrigEditProc;
    static LRESULT CALLBACK cmdProc(HWND,UINT,WPARAM,LPARAM);
    static LRESULT CALLBACK subProc(HWND,UINT,WPARAM,LPARAM);
    HWND hwndC;
    HWND hwndH;
    char buffer[128];   //����� �������
    char *pStrCmd;      //㪠��⥫� �� ��ப� ����� 
    int state;		//�������� ⥪�饣� ���ﭨ� ��⥬�
    float x1,y1,x2,y2;
 public:

    CommandLine();

    bool init(LPCTSTR windowName,
	  DWORD dwStyle,
	  int x, int y,
	  int cx,int cy,
	  HWND hwndParent,
	  HMENU hMenu,
	  HINSTANCE hInst);

    int getText(LPTSTR buffer, int maxChar);

    int getState();

    int setState(int);

    bool addCommand();

    bool segLine(float,float);

    bool segArc(float,float);

    POINT getStartPOINT();


};


//---------------MODEL WINDOW-------------------------------------------
class ModelWindow:public BaseWindow
{
 private:
    HDC hDC;
    POINT windowOrg;
    int xOrg, yOrg;
    float scaleFactor;
 public:
    ModelWindow():BaseWindow()
    {
	windowOrg.x=0;
	windowOrg.y=0;
	xOrg=0;
	yOrg=0;
    }

    HDC getHDC();

    bool setGM();		//���������� ����������� �����

    bool setWOrg(float,float);  

    bool setScale(float);
    
    bool line(int ,int, int, int );

};

#endif