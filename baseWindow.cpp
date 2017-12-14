#include "headerUI.h"

BaseWindow::BaseWindow()
{
	dwStyle=WS_OVERLAPPEDWINDOW;
	xPos=CW_USEDEFAULT;
	yPos=CW_USEDEFAULT;
	width=CW_USEDEFAULT;
	height=CW_USEDEFAULT;
	hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	LOGBRUSH logBrush={
			BS_SOLID,
			RGB(0,200,0),
			NULL
	};
	hBrush=CreateBrushIndirect(&logBrush);

}

HWND BaseWindow::getHWND(){
	return hWnd;
}

ATOM BaseWindow::registerClass(WNDCLASS &WndClass)
{
   WindowClass=&WndClass;
   return RegisterClass(WindowClass);
}

bool BaseWindow::init(LPCTSTR windowName)
{
	DWORD dwStyle=WS_OVERLAPPEDWINDOW;
	int x=CW_USEDEFAULT;
	int y=CW_USEDEFAULT;
	int cx=CW_USEDEFAULT;
	int cy=CW_USEDEFAULT;
	HWND hwndParent=NULL;
	HMENU hMenu=NULL;
	HINSTANCE hInst=NULL;

   hWnd = CreateWindow(
          WindowClass->lpszClassName,    // the window class name
          windowName,   		// The window title
          dwStyle,          // Window style as overlapped
          x,         // Default screen position of upper left
          y,         // corner of our window as x,y...
          cx,         // Default window size
          cy,         // .... 
          hwndParent,                     // No parent window
          hMenu,                     // No menu
          hInst,	         // Program Instance handle
          0                      // No window creation data
        );

   return (hWnd)?TRUE:FALSE;
}


bool BaseWindow::init(LPCTSTR windowName="New window",
			DWORD dwStyle=WS_OVERLAPPEDWINDOW,
			int x=CW_USEDEFAULT,
			int y=CW_USEDEFAULT,
			int cx=CW_USEDEFAULT,
			int cy=CW_USEDEFAULT,
			HWND hwndParent=NULL,
			HMENU hMenu=NULL,
			HINSTANCE hInst=NULL)
{

   hWnd = CreateWindow(
          WindowClass->lpszClassName,    // the window class name
          windowName,   		// The window title
          dwStyle,          // Window style as overlapped
          x,         // Default screen position of upper left
          y,         // corner of our window as x,y...
          cx,         // Default window size
          cy,         // .... 
          hwndParent,                     // No parent window
          hMenu,                     // No menu
          hInst,	         // Program Instance handle
          0                      // No window creation data
        );

   width=cx;
   height=cy;

   return (hWnd)?TRUE:FALSE;
}


bool BaseWindow::show(int nCmdShow=SW_SHOW)
{
   return ShowWindow(hWnd, nCmdShow);   // Display the window
}

RECT BaseWindow::getRect()
{
  RECT aRect; 
  GetClientRect(hWnd, &aRect);
  return aRect;
}

int BaseWindow::getWidth()
{
  RECT aRect; 
  GetClientRect(hWnd, &aRect);
  return aRect.right-aRect.left;
}

int BaseWindow::getHeight()
{
  RECT aRect; 
  GetClientRect(hWnd, &aRect);
  return aRect.bottom-aRect.top;
}

bool BaseWindow::SetWindowPosition(int x,int y,int cx,int cy)
{
  return SetWindowPos(hWnd,HWND_TOP,x,y,cx,cy,SWP_SHOWWINDOW);
}

bool BaseWindow::printText(int xt=0, int yt=0,const  char* str="")
{
  HDC hDC;
  hDC=GetDC(hWnd);
  SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  TextOut(hDC,xt,yt,str,strlen(str));
  ReleaseDC(hWnd,hDC);
  return TRUE;
}

bool BaseWindow::lineTo(int x,int y)
{
  bool result;
  HDC hDC;
  hDC=GetDC(hWnd);
  SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  result=LineTo(hDC,x,y);
  ReleaseDC(hWnd,hDC);
  return result;
}

bool BaseWindow::line(int xPrev,int yPrev,int x, int y)
{
  bool result;
  HDC hDC;
  HPEN hOldPen;
  HBRUSH hOldBrush;

  hDC=GetDC(hWnd);

  hOldPen=(HPEN)SelectObject(hDC, hBrush); 
  hOldBrush=(HBRUSH)SelectObject(hDC, hPen);
  SetROP2(hDC, R2_NOTXORPEN);

  //SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  MoveToEx(hDC,xPrev,yPrev,NULL);
  result=LineTo(hDC,x,y);
  //SelectObject(hDC, hOldPen); 
  //SelectObject(hDC, hOldBrush); 
  ReleaseDC(hWnd,hDC);
  return result;
}

bool BaseWindow::moveTo(int x,int y)
{
  bool result;
  HDC hDC;
  hDC=GetDC(hWnd);
  result=MoveToEx(hDC,x,y,NULL);
  ReleaseDC(hWnd,hDC);
  return result;
}	

bool BaseWindow::setBrush(HBRUSH*){

  return true;
}
bool BaseWindow::setPen(HPEN* phPen){

  hPen=*phPen;
  return true;
}
