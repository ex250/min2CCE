// pictogramm coordinate system
//
#include "headerUI.h"
#include <math.h>
extern ModelWindow modelWindow;

myCursor::myCursor():size(30.0),visible(true),
			arrowSize(10.0),arrowAngle(PI/6)
{}

float myCursor::getArrowSize(){
	return arrowSize;
}

bool myCursor::setArrowSize(float sz){
	arrowSize=sz*PI/180;
	return true;
}
bool myCursor::setArrowAngle(float beta){
	arrowAngle=beta;
	return true;
}

void myCursor::show(){

	int prevROP;
	HPEN hOldPen;
	HDC hDC=GetDC(modelWindow.getHWND());
	prevROP=SetROP2(hDC,R2_COPYPEN);
  	HPEN hPen=CreatePen(PS_SOLID, 50, RGB(0xFF,0,0));
  	hOldPen=(HPEN)SelectObject(hDC, hPen);

	if (visible){
		MoveToEx(hDC,(INT)(size*100),0,NULL);
		LineTo(hDC,0,0);
		LineTo(hDC,0,(INT)size*100);

		MoveToEx(hDC,(INT)((size-arrowSize*cos(arrowAngle))*100),
				(INT)(100*arrowSize*sin(arrowAngle)),NULL);
		LineTo(hDC,(INT)(size*100),0);
		LineTo(hDC,(INT)((size-arrowSize*cos(arrowAngle))*100),
				(INT)(-100*arrowSize*sin(arrowAngle)));

		MoveToEx(hDC,(INT)((-arrowSize*sin(arrowAngle))*100),
			(INT)((size-arrowSize*cos(arrowAngle))*100),NULL);
		LineTo(hDC,0,(INT)(size*100));
		LineTo(hDC,(INT)((arrowSize*sin(arrowAngle))*100),
			(INT)((size-arrowSize*cos(arrowAngle))*100));

	}
	SetROP2(hDC,prevROP);
  	DeleteObject(SelectObject(hDC,hOldPen));
	ReleaseDC(modelWindow.getHWND(),hDC);
}

float myCursor::getSize(){
	return size;
}

bool myCursor::setVisible(bool v){
	visible=v;
	return v;
}
