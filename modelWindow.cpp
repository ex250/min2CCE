#include "headerui.h"

bool ModelWindow::line(int xPrev,int yPrev,int x, int y)
{
  bool result;

  hDC=GetDC(hWnd);

  SelectObject(hDC, hBrush); 
  SelectObject(hDC, hPen);
  SetROP2(hDC, R2_NOTXORPEN);

  //SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  MoveToEx(hDC,xPrev,yPrev,NULL);
  result=LineTo(hDC,x,y);
  return result;
}

HDC ModelWindow::getHDC(){
  return hDC;
}

bool ModelWindow::setScale(float sf)
{
  RECT aRect;
  GetClientRect(hWnd,&aRect);

  hDC=GetDC(hWnd);

  scaleFactor=sf/10;
  xForm.eM11 = (FLOAT) scaleFactor; 
  xForm.eM12 = (FLOAT) 0.0; 
  xForm.eM21 = (FLOAT) 0.0; 
  xForm.eM22 = (FLOAT) scaleFactor; 
  xForm.eDx  = (FLOAT) 0.0; 
  xForm.eDy  = (FLOAT) 0.0; 
  
  SetWorldTransform(hDC, &xForm);
  InvalidateRect(hWnd,&aRect,true); 
  return true;
}

bool ModelWindow::setWOrg(float dx, float dy){

  RECT aRect;
  GetClientRect(hWnd,&aRect);

  xOrg=dx;
  yOrg=dy;

  hDC=GetDC(hWnd);

  xForm.eM11 = (FLOAT) scaleFactor; 
  xForm.eM12 = (FLOAT) 0.0; 
  xForm.eM21 = (FLOAT) 0.0; 
  xForm.eM22 = (FLOAT) scaleFactor; 
  xForm.eDx  = (FLOAT) dx; 
  xForm.eDy  = (FLOAT) dy; 
  SetWorldTransform(hDC, &xForm);
  InvalidateRect(hWnd,&aRect,true);  
  return true;
}

bool ModelWindow::setGM(){
  hDC=GetDC(hWnd);
  SetGraphicsMode(hDC, GM_ADVANCED);
  SetMapMode(hDC, MM_ISOTROPIC);
  SetViewportExtEx(hDC, width, height, NULL);
  float aspect=width/height;
  SetWindowExtEx(hDC, width, height, NULL);
  SetWindowOrgEx(hDC, windowOrg.x,windowOrg.y,NULL);

            xForm.eM11 = (FLOAT) scaleFactor; 
            xForm.eM12 = (FLOAT) 0.0; 
            xForm.eM21 = (FLOAT) 0.0; 
            xForm.eM22 = (FLOAT) scaleFactor; 
            xForm.eDx  = (FLOAT) xOrg; 
            xForm.eDy  = (FLOAT) yOrg; 
            SetWorldTransform(hDC, &xForm); 

  return true;
}
