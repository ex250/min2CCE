#include "headerui.h"
#include "model.h"
#include <iostream>
using namespace std;

extern CommandLine comStr;
extern ModelWindow modelWindow;

Layer layer[MAXLAYER];
Layer* defaultLayer=layer;
Layer* currentLayer=layer;

//**************class Layer*********************************************
unsigned char Layer::countID=0;

Layer::Layer():
    hide(OFF),ID(countID)
{
  switch(countID){
	case 1:
		color=0xff;
		type=PS_SOLID;
		width=5;
	break;

	case 2:
		color=0xff00;
		type=PS_DASH;
		width=1;
	break;

	case 3:
                color=0xff0000;
		type=PS_DOT;
		width=1;
	break;

        default:
                color=0;
		type=PS_SOLID;
		width=5;
  }

  countID++;
}


unsigned char Layer::getCount(){
  return countID;
}


unsigned char Layer::getID(){
  return ID;
}

int Layer::getType()const{
  return type;
}

int Layer::getWidth()const{
  return width;
}

int Layer::getColor()const{
  return color;
}

void Layer::show()const{
	//cout<<"���� "<<(int)ID<<" ��� �����: "<<type<<" ������� �����:"<<width
	//<<" ����: "<<color<<endl;
}

//**************class Entity********************************************

unsigned char Entity::getLayerID(){
  return layerID;
}

bool Entity::setLayerID(unsigned char ID){
  layerID=ID;
  return true;
}	

//**************class Point*********************************************

Point::Point():	type(currentLayer->getType()),
		width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
  //comStr.getFloatFloat();
  //x(0.0),y(0.0),
  //cout<<"POINT ����������� ��� ����������"<<endl;
  layerID=currentLayer->getID();
}


Point::Point(Layer* layer):x(0.0),y(0.0),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
  layerID=layer->getID();  
  //cout<<"POINT ����������� � ����� ����������"<<endl;
}

Point::Point(float xx,float yy):x(xx),y(yy),
		type(DEFAULTTYPE),width(DEFAULTWIDTH),color(DEFAULTCOLOR)
{
  layerID=currentLayer->getID();  
  //cout<<"POINT ����������� � ����� �����������"<<endl;	
}


Point::Point(float xx,float yy,Layer* layer):x(xx),y(yy),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
  layerID=currentLayer->getID();  
  //cout<<"POINT ����������� � ����� �����������"<<endl;	
}

void Point::show(){
  //cout<<"--------START POINT---------"<<endl;
  //cout<<"POINT x="<<x<<" y="<<y<<" type="<<type<<" width="<<width<<" color="<<color<<endl;
  //ptrToLayer->show();
  //cout<<"--------END OF POINT---------"<<endl;
}

bool Point::printInfo(){
  char buff[80];
  sprintf(buff,"x1=%f y1=%f ",x,y);
  modelWindow.printText(10,10,buff);
  return true;
}


bool Point::setXY(float xx,float yy){
  x=xx;
  y=yy;
  return true;
}

float Point::getX(){
  return x;
}

float Point::getY(){
  return y;
}

void Point::getPoint(){
  //cout<<"������� x:";
  //cin>>x;
  //cout<<"������� y:";
  //cin>>y;
}

bool Point::getDataFromUser()
{
  //cout<<"������� x:";
  //cin>>x;
  //cout<<"������� y:";
  //cin>>y;
  return TRUE;
}
//**************class Line *********************************************
Line::Line():start(),end(),
		type(currentLayer->getType()),
		width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
  //cout<<"LINE ����������� ��� ����������"<<endl;
  layerID=currentLayer->getID();
}


Line::Line(Layer* layer):start(0.0,0.0,layer),end(10.0,10.0,layer),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
 //cout<<"LINE ����������� � ����� ����������"<<endl;
 layerID=layer->getID();
}

Line::Line(Point *a,Point *b):start(*a),end(*b),type(currentLayer->getType()),width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
 //cout<<"LINE ����������� � ����� �����������"<<endl;
 layerID=currentLayer->getID();
 //char str[30];
 //sprintf(str,"x1=%d y1=%d x2=%d y2=%d",start.getX(),start.getY(),end.getX(),end.getY());
 //MessageBox(modelWindow.getHWND(),str,"���������� �����",MB_OK);
 //modelWindow.line(start.getX(),start.getY(),end.getX(),end.getY());
}


Line::Line(Point *a,Point *b,Layer* layer):start(*a),end(*b),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
 //cout<<"LINE ����������� � ����� �����������"<<endl;
 layerID=layer->getID();
}

void Line::show()
{
  unsigned char R,G,B;
  R=(color&0xff0000)>>16;
  G=(color&0xff00)>>8;
  B=(color&0xff);
  HPEN hPen=CreatePen(type, width, RGB(R,G,B));
  modelWindow.setPen(&hPen);
  modelWindow.line(start.getX(),start.getY(),end.getX(),end.getY());
  //printInfo();
}

void Line::getLine()
{
  //cout<<"������� ���������� ������ � ����� �������"<<endl;
  start.getPoint();
  end.getPoint();
}

bool Line::getDataFromUser()
{
  //cout<<"������� ���������� ������ � ����� �������"<<endl;
  start.getPoint();
  end.getPoint();
  return TRUE;
}

bool Line::printInfo(){
  char buff[80];
  sprintf(buff,"x1=%f y1=%f  x2=%f y2=%f",start.getX(),start.getY(),end.getX(),end.getY());
  modelWindow.printText(10,10,buff);
  return true;
}

//--------------ARC SECTION------------------------------------------

ArcSegment::ArcSegment(float x1, float y1 ,float x2,float y2, float x, float y,
 float R)
{
  width=5;
  type=PS_SOLID;
  xs=x1;
  ys=y1;
  xe=x2;
  ye=y2;
  xc=x;
  yc=y;
  radius=R;
}

void ArcSegment::show()
{
  unsigned char R,G,B;
  R=(color&0xff0000)>>16;
  G=(color&0xff00)>>8;
  B=(color&0xff);
  HPEN hPen=CreatePen(type, width, RGB(R,G,B));
  modelWindow.setPen(&hPen);
  modelWindow._arc(xs,ys,xe,ye,xc,yc,radius);
  printInfo();
}

bool ArcSegment::printInfo()
{ }

bool ArcSegment::getDataFromUser()
{ }

//**************class Model*********************************************
Model::Model()
{
ptrToDefaultLayer=defaultLayer;
ptrToCurrentLayer=currentLayer;
}

bool Model::addEntity(Entity* entity)
{
	//cout<<"Model:: addEntity(Entity *entity)"<<endl;
	entities.push_back(entity);
	return TRUE;
}

bool Model::appendLine(Point *start,Point *end){
	Line *ptrToLine;
	ptrToLine=new Line(start,end);
	entities.push_back(ptrToLine);
	return TRUE;
}


bool Model::appendPoint(){
	Point *ptrToPoint;
	ptrToPoint=new Point();
	ptrToPoint->getDataFromUser();
	entities.push_back(ptrToPoint);
	return TRUE;
}

bool Model::appendArc(float x1, float y1, float x2, float y2, 
			float xc, float yc, float R)
{
	ArcSegment *ptrToArc;
	ptrToArc=new ArcSegment(x1,y1,x2,y2,xc,yc,R);
	entities.push_back(ptrToArc);
	return TRUE;
}

int  Model::deleteEntity(int selectedEntities[])
{
	return 0;
}
int  Model::transformEntity(int selectedEntities[])
{
	return 0;
}

int  Model::readModel()
{
	return 0;
}
int  Model::writeModel()
{
	return 0;
}

void Model::showModel()
{
   for (int j=0;j<entities.size();j++)
	entities[j]->show();
}

int  Model::printModelInfo()const
{
	return 0;
}

