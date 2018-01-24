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
		width=30;
	break;

	case 2:
		color=0xff00;
		type=PS_DASH;
		width=8;
	break;

	case 3:
                color=0xff0000;
		type=PS_DOT;
		width=12;
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
	//cout<<"Слой "<<(int)ID<<" тип линии: "<<type<<" Толщина линии:"<<width
	//<<" Цвет: "<<color<<endl;
}

//**************class Entity********************************************

unsigned char Entity::getLayerID(){
  return layerID;
}

bool Entity::setLayerID(unsigned char ID){
  layerID=ID;
  return true;
}	
entityType Entity::getType(){
	if (typeid(*this)==typeid(Point))
		return tPoint;
	else if (typeid(*this)==typeid(Line))
		return tLine;
	else if (typeid(*this)==typeid(ArcSegment))
		return tArc;
	else
	{
		MessageBox(modelWindow.getHWND(),"invalid type",
				"Error:model.cpp:93",MB_OK);
	       	exit(-1);
	}
	return tPoint;
}

//**************class Point*********************************************

Point::Point():	type(currentLayer->getType()),
		width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
  //comStr.getFloatFloat();
  //x(0.0),y(0.0),
  //cout<<"POINT конструктор без параметров"<<endl;
  layerID=currentLayer->getID();
}


Point::Point(Layer* layer):x(0.0),y(0.0),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
  layerID=layer->getID();  
  //cout<<"POINT конструктор с одним параметром"<<endl;
}

Point::Point(float xx,float yy):x(xx),y(yy),
		type(DEFAULTTYPE),width(DEFAULTWIDTH),color(DEFAULTCOLOR)
{
  layerID=currentLayer->getID();  
  //cout<<"POINT конструктор с двумя параметрами"<<endl;	
}


Point::Point(float xx,float yy,Layer* layer):x(xx),y(yy),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
  layerID=currentLayer->getID();  
  //cout<<"POINT конструктор с тремя параметрами"<<endl;	
}

void Point::show(){
  //cout<<"--------START POINT---------"<<endl;
  //cout<<"POINT x="<<x<<" y="<<y<<" type="<<type<<" width="<<width<<" color="<<color<<endl;
  //ptrToLayer->show();
  //cout<<"--------END OF POINT---------"<<endl;
}

bool Point::printInfo(){
  char buff[2560];
  sprintf(buff,"x1=%f y1=%f ",x,y);
  comStr.addTextToHistory(buff);
  return true;
}

bool Point::getInfo(char* infPtr){
	char buff[256];
	sprintf(buff,"Point\tx=%3.2f, y=%3.2f\n",x,y);
	strcpy(infPtr,buff);
	return true;
}
bool Point::scale(float sf ){
	x*=sf;
	y*=sf;
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
  //cout<<"Введите x:";
  //cin>>x;
  //cout<<"Введите y:";
  //cin>>y;
}

bool Point::getDataFromUser()
{
  //cout<<"Введите x:";
  //cin>>x;
  //cout<<"Введите y:";
  //cin>>y;
  return TRUE;
}

bool Point::hitCursor(int xPos,int yPos,int size){
	bool result;
	char str[64];
	int xL=xPos-size;
	int xR=xPos+size;
	int yT=yPos-size;
	int yB=xPos+size;
	sprintf(str,"x=%f y=%f",x,y);
	if (x*100>=xL&&x*100<=xR&&y*100<=yB&&y*100>=yT){
		result=true;
	}
	else 
		result=false;

	return result;
}
//**************class Line ********************************************
Line::Line():start(),end(),
		type(currentLayer->getType()),
		width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
  //cout<<"LINE конструктор без параметров"<<endl;
  layerID=currentLayer->getID();
}


Line::Line(Layer* layer):start(0.0,0.0,layer),end(10.0,10.0,layer),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
 //cout<<"LINE конструктор с одним параметром"<<endl;
 layerID=layer->getID();
}

Line::Line(Point *a,Point *b):start(*a),end(*b),type(currentLayer->getType()),width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
 //cout<<"LINE конструктор с тремя параметрами"<<endl;
 layerID=currentLayer->getID();
 //char str[30];
 //sprintf(str,"x1=%d y1=%d x2=%d y2=%d",start.getX(),start.getY(),end.getX(),end.getY());
 //MessageBox(modelWindow.getHWND(),str,"Координаты линии",MB_OK);
 //modelWindow.line(start.getX(),start.getY(),end.getX(),end.getY());
}


Line::Line(Point *a,Point *b,Layer* layer):start(*a),end(*b),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
 //cout<<"LINE конструктор с тремя параметрами"<<endl;
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
  //cout<<"введите координаты начала и конца отрезка"<<endl;
  start.getPoint();
  end.getPoint();
}

bool Line::getDataFromUser()
{
  //cout<<"введите координаты начала и конца отрезка"<<endl;
  start.getPoint();
  end.getPoint();
  return TRUE;
}

bool Line::getInfo(char* infPtr){
	char buff[256];
  sprintf(buff,"LINE\tx1=%3.2f y1=%3.2f  x2=%3.2f y2=%3.2f\n",start.getX(),start.getY(),end.getX(),end.getY());
	strcpy(infPtr,buff);
	return true;
}

bool Line::printInfo(){
  char buff[2560];
  sprintf(buff,"x1=%f y1=%f  x2=%f y2=%f",start.getX(),start.getY(),end.getX(),end.getY());
  comStr.addTextToHistory(buff);
  return true;
}

bool Line::scale(float sf ){
	start.scale(sf);
	end.scale(sf);
	return true;
}

bool Line::hitCursor(int xPos,int yPos,int size){
	bool result;
	int xL=xPos-size;
	int xR=xPos+size;
	int yT=yPos-size;
	int yB=xPos+size;
	if (start.hitCursor(xPos,yPos,size)||end.hitCursor(xPos,yPos,size)){
		HDC hDC=GetDC(modelWindow.getHWND());
		HPEN hOldPen=(HPEN)SelectObject(hDC,CreatePen(PS_SOLID,width,RGB(255,0,0)));
		SetROP2(hDC,R2_COPYPEN);
		MoveToEx(hDC,start.getX()*100,start.getY()*100,NULL);
		LineTo(hDC,end.getX()*100,end.getY()*100);
		SelectObject(hDC,hOldPen);
		ReleaseDC(modelWindow.getHWND(),hDC);
		result=true;
	}
	else {
		result=false;
	}
	return result;
}

//--------------ARC SECTION------------------------------------------

ArcSegment::ArcSegment():
		type(currentLayer->getType()),
		width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
  layerID=currentLayer->getID();
}

ArcSegment::ArcSegment(float x1, float y1 ,float x2,float y2, float x, float y,
 float R, int ArcDir): color(currentLayer->getColor()),type(currentLayer->getType()),width(currentLayer->getWidth())
{
  xs=x1;
  ys=y1;
  xe=x2;
  ye=y2;
  xc=x;
  yc=y;
  radius=R;
  direction=ArcDir;
}

void ArcSegment::show()
{
  unsigned char R,G,B;
  R=(color&0xff0000)>>16;
  G=(color&0xff00)>>8;
  B=(color&0xff);
  HPEN hPen=CreatePen(type, width, RGB(R,G,B));
  modelWindow.setPen(&hPen);
  modelWindow._arc(xs,ys,xe,ye,xc,yc,radius,direction);
}


bool ArcSegment::getInfo(char* infPtr){
	char buff[256];
	char direct[16];
	if (direction==AD_COUNTERCLOCKWISE)
		strcpy(direct,"CCW");
	else 
		strcpy(direct,"CW");
  sprintf(buff,"ARC\tx1=%3.2f y1=%3.2f  x2=%3.2f y2=%3.2f Xc=%3.2f Yc=%3.2f R=%3.2f Direction=%s\n",xs,ys,xe,ye,xc,yc,radius,direct);
	strcpy(infPtr,buff);
	return true;
}

bool ArcSegment::printInfo()
{
  char buff[2560];
  sprintf(buff,"x1=%f y1=%f  x2=%f y2=%f Xc=%f Yc=%f R=%f Direction=%d",xs,ys,xe,ye,xc,yc,radius,direction);
  comStr.addTextToHistory(buff);
}

bool ArcSegment::getDataFromUser()
{ }

bool ArcSegment::scale(float sf ){
	xs*=sf;
	ys*=sf;
	xe*=sf;
	ye*=sf;
	xc*=sf;
	yc*=sf;
	radius*=sf;
	return true;
}

bool ArcSegment::hitCursor(int xPos,int yPos,int size){
	bool result;
	int xL=xPos-size;
	int xR=xPos+size;
	int yT=yPos-size;
	int yB=xPos+size;
	return false;
}

//**************class Model*********************************************
Model::Model()
{
ptrToDefaultLayer=defaultLayer;
ptrToCurrentLayer=currentLayer;
iter=entities.begin();
}

bool Model::addEntity(Entity* entity)
{
	entities.push_back(entity);
	return TRUE;
}

bool Model::appendLine(Point *start,Point *end){
	Line *ptrToLine;
	ptrToLine=new Line(start,end);
	if (!ptrToLine){
		MessageBox(modelWindow.getHWND(),"Model:appendLine:360. NULL pointer","Error", MB_OK);
		exit(-1);
	}
	if (entities.size()<(entities.max_size()-10))
		entities.push_back(ptrToLine);
	else 
		MessageBox(modelWindow.getHWND(),"Overflow vector!!!","Error",MB_OK);
	return TRUE;
}


bool Model::appendPoint(){
	Point *ptrToPoint;
	ptrToPoint=new Point();
	if (!ptrToPoint){
		MessageBox(modelWindow.getHWND(),"Model:appendPoint:372. NULL pointer","Error", MB_OK);
		exit(-1);
	}
	ptrToPoint->getDataFromUser();
	entities.push_back(ptrToPoint);
	return TRUE;
}

bool Model::appendArc(float x1, float y1, float x2, float y2, 
			float xc, float yc, float R, int ArcDir)
{
	ArcSegment *ptrToArc;
	ptrToArc=new ArcSegment(x1,y1,x2,y2,xc,yc,R,ArcDir);
	if (!ptrToArc){
		MessageBox(modelWindow.getHWND(),"Model:appendArc:386. NULL pointer","Error", MB_OK);
		exit(-1);
	}
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

int  Model::setFileName(char* fn){
	strcpy(FileName,fn);
	return 0;
}

int  Model::readModel(char * fn)
{
	char buff[256];
	int size;
	Entity * enPtr;
	entityType etype;
	ifstream is;
	is.open(fn,ios::binary);
	if (!is)
	{
		MessageBox(modelWindow.getHWND(),"Error open file:model.cpp:346","Error",MB_OK);
	}
	while(true){
		is.read((char*)&etype,sizeof(etype));
		if (is.eof())
			break;
		if (!is)
	{
		MessageBox(modelWindow.getHWND(),"read error type: model.cpp:355","Error",MB_OK);
	}
		switch(etype){
			case tPoint:
				enPtr=new Point;
				size=sizeof(Point);
				break;
			case tLine:
				enPtr=new Line;
				size=sizeof(Line);
				break;
			case tArc:
				enPtr=new ArcSegment;
				size=sizeof(ArcSegment);
				break;
			default:
		MessageBox(modelWindow.getHWND(),"ERROR READ: unknown Type","Control Read",MB_OK);
				exit( -1);
		}
		is.read((char*)enPtr,size);
		if (!is)
		{
		MessageBox(modelWindow.getHWND(),"ERROR READ","Control Read",MB_OK);
		}
		else{ 
			entities.push_back(enPtr);
		//	enPtr->printInfo();
		}
	}
	is.close();

	return 0;
}
int  Model::writeModel(char * fn)
{  
	setFileName(fn);
	ofstream ouf;
	entityType enType;
	int size;
	int j;
	char buff[256];

	if (FileName!="")
		ouf.open(FileName,ios::binary);
	else 
	{
		sprintf(buff,"empty file name %s",FileName);
		MessageBox(modelWindow.getHWND(),buff,"Error",MB_OK);
	       	exit(-1);
	}



   for (j=0;j<entities.size();j++){
	   enType=entities[j]->getType();
	   switch(enType){
		   case tPoint:
			   size=sizeof(Point);
			   break;
		   case tLine:
			   size=sizeof(Line);
			   break;
		   case tArc:
			   size=sizeof(ArcSegment);
			   break;
		   default:
			   cerr<<"unknown type"<<endl;
			   exit(-1);
	   }

	   ouf.write((char*)&enType,sizeof(enType));
	   if (!ouf){
		sprintf(buff,"error write type %d",enType);
		MessageBox(modelWindow.getHWND(),buff,"Error",MB_OK);
	   	exit(-1);
	   }

	   ouf.write((char*)entities[j],size);
	   if (!ouf)
	   { 
		sprintf(buff,"error write Entities %d",enType);
		MessageBox(modelWindow.getHWND(),buff,"Error",MB_OK);
	   	exit(-1);
	   }
   }

	ouf.close();

	return 0;
}
int Model::saveInfo(const char *fn){
   int j;
   char buff[256];
   char tmp[64];
   ofstream oufile;
   oufile.open(fn);
   if (!oufile)
	MessageBox(modelWindow.getHWND(),"Error open file","SAVE...",MB_OK);
   
   for (j=0;j<entities.size();j++){
	entities[j]->getInfo(buff);
	oufile.write(buff,strlen(buff));
	sprintf(tmp,"Len=%d",strlen(buff));
	strcat(buff,tmp);
	MessageBox(modelWindow.getHWND(),buff,"SAVE...",MB_OK);
   if (!oufile)
	MessageBox(modelWindow.getHWND(),"Error write file","SAVE...",MB_OK);

   }
   oufile.close();
   return 0;
}


void Model::showModel()
{
   iter=entities.begin();
   if (!entities.empty()&&(entities.size()<entities.max_size()))
   for (iter;iter!=entities.end();++iter)
	(*iter)->show();
}

int  Model::printModelInfo()const
{
	return 0;
}

int Model::deleteAll()
{
	int result=0;
	while(!entities.empty()){
		result++;
		entities.pop_back();
	}
	return result;
}
int Model::scaleModel(float sf){
   int j;
   for (j=0;j<entities.size();j++)
	entities[j]->scale(sf);
   return 0;
}

bool Model::hitModel(int xPos,int yPos,int size){
   int result=0;
   iter=entities.begin();
   if (!entities.empty()&&(entities.size()<entities.max_size()))
   for (iter;iter!=entities.end();++iter)
	result+=(*iter)->hitCursor(xPos,yPos,size);

	return result;
}
