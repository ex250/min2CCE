#include "headerui.h"

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
bool ModelWindow::setROP2(int rop2){
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
  SetGraphicsMode(hDC, GM_ADVANCED);
  SetMapMode(hDC, MM_ISOTROPIC);
  SetViewportExtEx(hDC, width, height, NULL);
  float aspect=width/height;
  SetWindowExtEx(hDC, width*10, height*10, NULL);
  //SetWindowOrgEx(hDC, windowOrg.x,windowOrg.y,NULL);
  SetWindowOrgEx(hDC,-width*5,height*5,NULL);

            xForm.eM11 = (FLOAT) scaleFactor; 
            xForm.eM12 = (FLOAT) 0.0; 
            xForm.eM21 = (FLOAT) 0.0; 
            xForm.eM22 = (FLOAT) scaleFactor; 
            xForm.eDx  = (FLOAT) xOrg; 
            xForm.eDy  = (FLOAT) yOrg; 
            SetWorldTransform(hDC, &xForm); 
  ReleaseDC(hWnd,hDC);

  return true;
}
