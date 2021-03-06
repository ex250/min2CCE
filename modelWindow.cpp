#include "headerui.h"
#include <stdio.h>

extern CommandLine comStr;

bool ModelWindow::init(LPCTSTR windowName,
	 	  DWORD dwStyle,
		  int x,
		  int y,
		  int cx,
		  int cy,
		  HWND hwndParent,
		  HMENU hMenu,
		  HINSTANCE hInst)
{
	BaseWindow::init(windowName,
	 	  dwStyle,
		  x,
		  y,
		  cx,
		  cy,
		  hwndParent,
		  hMenu,
		  hInst);
	       hCursor=LoadCursor(hInst,MAKEINTRESOURCE(IDC_CROSSPTR));
	       SetClassLong(hWnd,GCL_HCURSOR,(LONG)hCursor);
	       return true;
}	

bool ModelWindow::line(float xPrev,float yPrev,float x, float y)
{
  bool result;
  xPrev*=100;yPrev*=100;
  x*=100;y*=100;

  HPEN hOldPen;
  HBRUSH hOldBrush;
  hDC=GetDC(hWnd);

  hOldBrush=(HBRUSH)SelectObject(hDC, hBrush); 
  hOldPen=(HPEN)SelectObject(hDC, hPen);
  SetROP2(hDC, ROP2); 
  //SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  MoveToEx(hDC,xPrev,yPrev,NULL);
  result=LineTo(hDC,x,y);

  DeleteObject(SelectObject(hDC,hOldPen));
  DeleteObject(SelectObject(hDC,hOldBrush));
  ReleaseDC(hWnd,hDC);
  return result;
}

bool ModelWindow::marker(float x,float y)
{
  bool result;
  x*=100;y*=100;
  float size=80;//marker size
  XFORM transform;

  HPEN hOldPen;
  HBRUSH hOldBrush;
  hDC=GetDC(hWnd);

  if (GetWorldTransform(hDC,&transform))
	  size/=transform.eM11;

  hOldBrush=(HBRUSH)SelectObject(hDC, hBrush); 
  hOldPen=(HPEN)SelectObject(hDC, hPen);
  SetROP2(hDC, ROP2); 
  //SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  result=Rectangle(hDC,x-size,y-size,x+size,y+size);

  DeleteObject(SelectObject(hDC,hOldPen));
  DeleteObject(SelectObject(hDC,hOldBrush));
  ReleaseDC(hWnd,hDC);

	return result;
}

bool ModelWindow::markerOsnap(float x,float y)
{
  bool result;
  x*=100;y*=100;
  float size=140;//marker size
  XFORM transform;

  HPEN hOldPen;
  HBRUSH hOldBrush;
  hDC=GetDC(hWnd);

  if (GetWorldTransform(hDC,&transform))
	  size/=transform.eM11;

  //hOldBrush=(HBRUSH)SelectObject(hDC, hBrush); 
  hBrush=CreateSolidBrush(RGB(255,0,128)); 
  hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
  hOldPen=(HPEN)SelectObject(hDC, hPen);
  SetROP2(hDC, ROP2); 
  //SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  result=Rectangle(hDC,x-size,y-size,x+size,y+size);

  DeleteObject(SelectObject(hDC,hOldPen));
  DeleteObject(SelectObject(hDC,hOldBrush));
  ReleaseDC(hWnd,hDC);

	return result;
}

bool ModelWindow::myPolygon(CONST POINT * point, int nCount)
{
	bool result;
  hDC=GetDC(hWnd);
  SetROP2(hDC, ROP2); 
  result=Polygon(hDC,point,nCount);
  ReleaseDC(hWnd,hDC);
	return result;
}

bool ModelWindow::myRectangle(POINT pt1, POINT pt2)
{
	bool result;
  hDC=GetDC(hWnd);
  result=Rectangle(hDC,pt1.x,pt1.y,pt2.x,pt2.y);
  ReleaseDC(hWnd,hDC);
	return result;
}

bool ModelWindow::myRectangle(float xPrev,float yPrev,float x,float y)
{
  bool result;
  x*=100;y*=100;
  xPrev*=100;yPrev*=100;

  HPEN hOldPen;
  HBRUSH hOldBrush;
  hDC=GetDC(hWnd);

  hOldBrush=(HBRUSH)SelectObject(hDC, hBrush); 
  hOldPen=(HPEN)SelectObject(hDC, hPen);
  SetROP2(hDC, ROP2); 
  result=Rectangle(hDC,xPrev,yPrev,x,y);

  DeleteObject(SelectObject(hDC,hOldPen));
  DeleteObject(SelectObject(hDC,hOldBrush));
  ReleaseDC(hWnd,hDC);

	return result;
}

//*****************************************************************
//
//                **1(x,y)
//              **     *        1 - start point
//             *   xy   *       2 - end point
//             *        *       xy - center
//              **     *        R - radius
//                *2(x,y)       ArcDir - direction CW/CCW
//                
//*****************************************************************
bool ModelWindow::_arc(float x1, float y1, float x2, float y2,
			 float x, float y, float R, int ArcDir)
{
  bool result;
  HPEN hOldPen;
  HBRUSH hOldBrush;
  x1*=100;y1*=100;
  x2*=100;y2*=100;
  x*=100;y*=100;
  R*=100;
  hDC=GetDC(hWnd);
  SetArcDirection(hDC,ArcDir);
  hOldBrush=(HBRUSH)SelectObject(hDC, hBrush); 
  hOldPen=(HPEN)SelectObject(hDC, hPen);
  SetROP2(hDC, ROP2);
  result=Arc(hDC,x-R,y+R,x+R,y-R, x1,y1, x2,y2);
  DeleteObject(SelectObject(hDC,hOldPen));
  DeleteObject(SelectObject(hDC,hOldBrush));
  ReleaseDC(hWnd,hDC);
  return result;

}

HDC ModelWindow::getHDC(){
  return hDC;
}

int ModelWindow::setROP2(int rop2){
	ROP2=rop2;
	return ROP2;
}

bool ModelWindow::setScale(float sf)
{
  XFORM transform;
  RECT aRect;
  GetClientRect(hWnd,&aRect);

  hDC=GetDC(hWnd);

  scaleFactor=sf;
  transform.eM11 = (FLOAT) scaleFactor; 
  transform.eM12 = (FLOAT) 0.0; 
  transform.eM21 = (FLOAT) 0.0; 
  transform.eM22 = (FLOAT) scaleFactor; 
  transform.eDx  = (FLOAT) 0.0; 
  transform.eDy  = (FLOAT) 0.0; 
  
  //SetWorldTransform(hDC, &xForm);
  ModifyWorldTransform(hDC,&transform, MWT_RIGHTMULTIPLY);
  //InvalidateRect(hWnd,&aRect,true); 
  ReleaseDC(hWnd,hDC);
  return true;
}

float ModelWindow::getScale()
{
  XFORM transform;
  bool result;
  hDC=GetDC(hWnd);
  result=GetWorldTransform(hDC,&transform);
  ReleaseDC(hWnd,hDC);
  return result?transform.eM11:-1;
}

bool ModelWindow::setWOrg(float dx, float dy){

  XFORM transform;
  RECT aRect;
  GetClientRect(hWnd,&aRect);

  xOrg=dx;
  yOrg=dy;

  hDC=GetDC(hWnd);

  transform.eM11 = (FLOAT) 1; 
  transform.eM12 = (FLOAT) 0.0; 
  transform.eM21 = (FLOAT) 0.0; 
  transform.eM22 = (FLOAT) 1; 
  transform.eDx  = (FLOAT) dx; 
  transform.eDy  = (FLOAT) dy; 
  ModifyWorldTransform(hDC,&transform, MWT_RIGHTMULTIPLY);
  //SetWorldTransform(hDC, &xForm);
  //InvalidateRect(hWnd,&aRect,true);  
  ReleaseDC(hWnd,hDC);
  return true;
}

bool ModelWindow::setGM(){

  hDC=GetDC(hWnd);
  SetMapMode(hDC, MM_HIMETRIC);
  SetGraphicsMode(hDC, GM_ADVANCED);
  SetViewportOrgEx(hDC,20,height-50,NULL);
  ReleaseDC(hWnd,hDC);

  return true;
}
