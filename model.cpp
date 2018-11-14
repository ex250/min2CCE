#include "headerui.h"
#include "model.h"
#include <iostream>
#include <math.h>
using namespace std;

extern CommandLine comStr;
extern ModelWindow modelWindow;

Layer layer[MAXLAYER];
Layer* defaultLayer=layer;
Layer* currentLayer=layer;

//**************class Layer*********************************************
unsigned char Layer::countID=0;

Layer::Layer():
       	hide(OFF),ID(countID),
	speed(DEFAULT_SPEED),
	tool(DEFAULT_TOOL),
	Z(MIN_Z)
{
  switch(countID){
	case 1:
		color=0xff;
		type=PS_SOLID;
		width=1;
	break;

	case 2:
		color=0xff00;
		type=PS_DASH;
		width=1;
	break;

	case 3:
                color=0xff0000;
		type=PS_DOT;
		width=80;
	break;

        default:
                color=0;
		type=PS_SOLID;
		width=1;
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

float Layer::getZ()const{
  return Z;
}

float Layer::getSpeed()const{
  return speed;
}

toolType Layer::getTool()const{
  return tool;
}

void Layer::show()const{
}

void Layer::setZ(float zCoord){
	Z=zCoord;
}
void Layer::setSpeed(float spd){
	speed=spd;
}

void Layer::setTool(toolType tl){
	tool=tl;
}
//**************class Entity********************************************

unsigned char Entity::getLayerID(){
  return layerID;
}

bool Entity::setLayerID(unsigned char ID){
  layerID=ID;
  return true;
}	

void Entity::setZ(float zCoord){
  Z=zCoord;
}	

float Entity::getZ(){
	return Z;
}	

void Entity::setSpeed(float spd){
  speed=spd;
}	

float Entity::getSpeed(){
	return speed;
}	

void Entity::setTool(toolType tl){
  tool=tl;
}	

toolType Entity::getTool(){
	return tool;
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
  layerID=currentLayer->getID();
}


Point::Point(Layer* layer):x(0.0),y(0.0),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
  layerID=layer->getID();  
}

Point::Point(float xx,float yy):x(xx),y(yy),
		type(DEFAULTTYPE),width(DEFAULTWIDTH),color(DEFAULTCOLOR)
{
  layerID=currentLayer->getID();  
}


Point::Point(float xx,float yy,Layer* layer):x(xx),y(yy),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
  layerID=currentLayer->getID();  
}

Point::Point(vec2 V):x(V.x),y(V.y),
	type(currentLayer->getType()),
		width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
  layerID=currentLayer->getID();
}

void Point::show(){
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

bool Point::move(float dx,float dy){
	x+=dx;
	y+=dy;
	return true;
}

bool Point::rotate(float angle){
	float beta=angle*PI/180;
	float xt;
	xt=x*cos(beta)-y*sin(beta);
	y=x*sin(beta)+y*cos(beta);
	x=xt;
	return true;
}

bool Point::rotRad(float beta){
	float xt;
	xt=x*cos(beta)-y*sin(beta);
	y=x*sin(beta)+y*cos(beta);
	x=xt;
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
}

bool Point::getDataFromUser()
{
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
  layerID=currentLayer->getID();
  Z=currentLayer->getZ();
  speed=currentLayer->getSpeed();
  tool=currentLayer->getTool();
}


Line::Line(Layer* layer):start(0.0,0.0,layer),end(10.0,10.0,layer),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
 layerID=layer->getID();
}

Line::Line(Point *a,Point *b):start(*a),end(*b),type(currentLayer->getType()),width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
 layerID=currentLayer->getID();
}


Line::Line(Point *a,Point *b,Layer* layer):start(*a),end(*b),type(layer->getType()),width(layer->getWidth()),
		color(layer->getColor())
{
 layerID=layer->getID();
}

void Line::show()
{
  unsigned char R,G,B;
  HPEN hPen;

  R=(color&0xff0000)>>16;
  G=(color&0xff00)>>8;
  B=(color&0xff);

  if (selected==false)
  	hPen=CreatePen(type, width, RGB(R,G,B));
  else
  {
  	hPen=CreatePen(type, width, RGB(250,0,0));
  	modelWindow.marker(start.getX(),start.getY());
  	modelWindow.marker(end.getX(),end.getY());
  }

  modelWindow.setPen(&hPen);
  modelWindow.line(start.getX(),start.getY(),end.getX(),end.getY());
  DeleteObject(hPen);
}

void Line::getLine()
{
  start.getPoint();
  end.getPoint();
}


Point* Line::getStart()
{
	return &start;
}


Point* Line::getEnd()
{
	return &end;
}

bool Line::getDataFromUser()
{
  start.getPoint();
  end.getPoint();
  return TRUE;
}

bool Line::setStart(float x,float y)
{
	return 	start.setXY(x,y);
}

bool Line::setEnd(float x,float y)
{
	return 	end.setXY(x,y);
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
	HRGN hRgn;
	int inflate=50;
	POINT point[2];
	vec2 a,b,norm;
	vec2 sum;
	a.x=start.getX();
	a.y=start.getY();
	b.x=end.getX();
	b.y=end.getY();
	sum=b-a;
	norm=sum.ort();
	norm.normalize();
	norm*=2;//inflate region
	//modelWindow.line(0,0,norm.x,norm.y);

	sum=a+norm;
	//modelWindow.line(0,0,sum.x,sum.y);
	point[0].x=(int)(sum.x*100);
	point[0].y=(int)(sum.y*100);

	sum=b+norm;
	//modelWindow.line(0,0,sum.x,sum.y);
	point[1].x=(int)(sum.x*100);
	point[1].y=(int)(sum.y*100);

	norm*=-1;
	sum=a+norm;
	//modelWindow.line(0,0,sum.x,sum.y);
	point[3].x=(int)(sum.x*100);
	point[3].y=(int)(sum.y*100);

	sum=b+norm;
	//modelWindow.line(0,0,sum.x,sum.y);
	point[2].x=(int)(sum.x*100);
	point[2].y=(int)(sum.y*100);

	if (start.getX()==end.getX())
		hRgn=CreateRectRgn((int)(start.getX()*100)-inflate,
				(int)(start.getY()*100),
				(int)(end.getX()*100)+inflate,
				(int)(end.getY()*100)
			);
	else if (start.getY()==end.getY())
		hRgn=CreateRectRgn((int)(start.getX()*100),
				(int)(start.getY()*100)-inflate,
				(int)(end.getX()*100),
				(int)(end.getY()*100)+inflate
			);
	else
	{
		hRgn=CreatePolygonRgn(point,4,ALTERNATE);
		//modelWindow.myPolygon(point,4);
	}
	result=PtInRegion(hRgn,xPos,yPos);

	if (result)
		comStr.pActivEntity=(int *)this;

	DeleteObject(hRgn);

	if (selected&&result)
		return result;
	else 
	{
		selected=result;
		show();
	}
	
	return result;
}

bool Line::move(float dx, float dy ){
	start.move(dx,dy);
	end.move(dx,dy);
	return true;
}

//--------------ARC SECTION------------------------------------------

ArcSegment::ArcSegment():
		type(currentLayer->getType()),
		width(currentLayer->getWidth()),
		color(currentLayer->getColor())
{
  layerID=currentLayer->getID();
}

ArcSegment::ArcSegment(float x1, float y1,float x2,float y2,
       	float x, float y, float R, int ArcDir):
	color(currentLayer->getColor()),type(currentLayer->getType()),width(currentLayer->getWidth())
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
  HPEN hPen;
  R=(color&0xff0000)>>16;
  G=(color&0xff00)>>8;
  B=(color&0xff);

  if (selected==false)
  	hPen=CreatePen(type, width, RGB(R,G,B));
  else
  	hPen=CreatePen(type, width, RGB(250,0,0));

  modelWindow.setPen(&hPen);
  modelWindow._arc(xs,ys,xe,ye,xc,yc,radius,direction);
  DeleteObject(hPen);
}

float ArcSegment::getStartX(){
	return xs;
}

float ArcSegment::getStartY(){
	return ys;
}

float ArcSegment::getEndX(){
	return xe;
}

float ArcSegment::getEndY(){
	return ye;
}

float ArcSegment::getRadius(){
	return radius;
}

int ArcSegment::getDirection(){
	return direction;
}

float ArcSegment::getSignRad(){
	vec2 start(xs,ys);
	vec2 end(xe,ye);
	vec2 center(xc,yc);
	vec2 ox(1,0);
	vec2 se;
	float angle;
	float tempX,tempY;
	int signRad;
	se=end-start;
	se.normalize();
	angle=acos(dot(se,ox));
	center-=start;
	if (se.y>0)
		angle=-angle;
	tempY=center.x*sin(angle)+center.y*cos(angle);
	if (direction==AD_CLOCKWISE){
		if (tempY<0)
			signRad=-1;
		else 
			signRad=1;
	}
	else{
		if (tempY<0)
			signRad=1;
		else 
			signRad=-1;
	}

	return radius*signRad;
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
  float angle=getAngle()*180/PI;
  sprintf(buff,"x1=%f y1=%f  x2=%f y2=%f Xc=%f Yc=%f R=%f Direction=%d angle=%f",xs,ys,xe,ye,xc,yc,radius,direction,angle);
  comStr.addTextToHistory(buff);
  return hitCursor(10,10,10);
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

bool ArcSegment::move(float x, float y ){
	xc+=x;
	yc+=y;
	xs+=x;
	ys+=y;
	xe+=x;
	ye+=y;
	return true;
}

float ArcSegment::getAngle()
{
	vec2 s,e,c;
	vec2 sc,ec;
	vec2 ortX(1,0);
	s.x=xs;
	s.y=ys;
	e.x=xe;
	e.y=ye;
	c.x=xc;
	c.y=yc;

	sc=s-c;
	ec=e-c;

	sc.normalize();
	ec.normalize();

	float angle;
	float beta;
	angle=acos(dot(sc,ec));

	beta=acos(sc.x/sc.length());
	if (sc.y<0)
		beta=2*PI-beta;

	beta=-beta;
	float x,y;
	x=sc.x*cos(beta)-sc.y*sin(beta);
	y=sc.x*sin(beta)+sc.y*cos(beta);
	sc.x=x;
	sc.y=y;
	//modelWindow.line(0,0,x*100,y*100);
	x=ec.x*cos(beta)-ec.y*sin(beta);
	y=ec.x*sin(beta)+ec.y*cos(beta);
	//modelWindow.line(0,0,x*100,y*100);
	if (y<0&&direction==AD_CLOCKWISE)
		angle=2*PI-angle;
	else if (y>0&&direction==AD_COUNTERCLOCKWISE)
		angle=2*PI-angle;
	return angle;
}

bool ArcSegment::hitCursor(int xPos,int yPos,int size){
	bool result;
	HRGN hRgn;
	int nVertex=20;
	POINT *lpPoint;
	POINT *lpBegin;
	lpPoint=new POINT[nVertex*2];
	lpBegin=lpPoint;
	vec2 s,e,c;
	vec2 sc,res;

	s.x=xs;
	s.y=ys;
	e.x=xe;
	e.y=ye;
	c.x=xc;
	c.y=yc;
	sc=s-c;

	res=sc;
	res.normalize();
	res*=2;
	sc+=res;

	Point ptStart(sc.x,sc.y);

	float delta=getAngle()/(nVertex-1);
	if (direction==AD_COUNTERCLOCKWISE)
		delta*=-1;

	while(lpPoint<(lpBegin+nVertex*2))
	{
		res.x=ptStart.getX();
		res.y=ptStart.getY();
		res+=c;

		lpPoint->x=(int)(res.x*100);
		lpPoint->y=(int)(res.y*100);
		//modelWindow.marker(res.x,res.y);
		//modelWindow.line(xc,yc,res.x,res.y);

		lpPoint++;

		if (lpPoint==(lpBegin+nVertex))
		{
			delta=-delta;
			res.x=ptStart.getX();
			res.y=ptStart.getY();
			sc=res;
			sc.normalize();
			sc*=4;
			res-=sc;
			ptStart.setXY(res.x,res.y);
			continue;
		}
		ptStart.rotRad(delta);
	}
		hRgn=CreatePolygonRgn(lpBegin,nVertex*2,ALTERNATE);
		//modelWindow.myPolygon(lpBegin,nVertex*2);

	result=PtInRegion(hRgn,xPos,yPos);

	if (result)
		comStr.pActivEntity=(int *)this;

	DeleteObject(hRgn);

	if (selected&&result)
		return result;
	else 
	{
		selected=result;
		show();
	}

	delete [] lpBegin;

	return result;
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

	ptrToLine->setZ(currentLayer->getZ());

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
	ptrToArc->setZ(currentLayer->getZ());
	entities.push_back(ptrToArc);
	return TRUE;
}

bool Model::appendArcCenAngRad(float xc, float yc, float startAn, 
		float endAn, float R)
{

	return 0;
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
	strcpy(FileName,fn);
	Entity * enPtr;
	entityType etype;
	ifstream is;
	is.open(FileName,ios::binary);
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
int  Model::writeModel(const char * fn)
{  
	ofstream ouf;
	entityType enType;
	int size;
	int j;
	char buff[256];

	strcpy(FileName,fn);

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
		sprintf(buff,"error write type %d\nCod:%d\ngood:%d\neof:%d\nfail:%d\nbad:%d",enType,ouf.rdstate(),ouf.good(),ouf.eof(),ouf.fail(),ouf.bad());
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
int Model::saveGcodeISO(const char *fn)
{
   int j;
   int numLine;
   char buff[256];
   char tmp[64];
   float lastX;
   float lastY;
   Line * ptrLine;
   ArcSegment * ptrArc;
   ofstream oufile;
   entityType etype;
   float safeDistZ=2;
   float workDistZ=-2;
   enum {SAFE_STRING,G0,G1,G2,G3,SAFE_Z};
   const char *prgFormatCode[]={
	   "N%i G21 G40 G49 G54 G80 G90\n",
	   "N%i X%.3f Y%.3f G0\n",
	   "N%i X%.3f Y%.3f G1\n",
	   "N%i X%.3f Y%.3f R%.3f G2\n",
	   "N%i X%.3f Y%.3f R%.3f G3\n",
	   "N%i Z%.3f G1\n"
   };

	if (FileName!="")
		oufile.open(fn);
	else 
	{
		sprintf(buff,"empty file name %s",FileName);
		MessageBox(modelWindow.getHWND(),buff,"Error",MB_OK);
	       	return -1;
	}

   lastX=0.0;
   lastY=0.0;
   numLine=0;

   for (j=0;j<entities.size();j++){

	etype=entities[j]->getType();

	if (!numLine){
		numLine+=10;
		sprintf(tmp,prgFormatCode[SAFE_STRING],numLine);
		strcpy(buff,tmp);
		numLine+=10;
	}

	switch(etype){
		case tPoint:
			strcpy(buff,"tPoint\n");
			break;
		case tLine:
			ptrLine=(Line*)(entities[j]);

   			workDistZ=ptrLine->getZ();

			if (lastX==ptrLine->getStart()->getX()&&
			    lastY==ptrLine->getStart()->getY()){
			sprintf(tmp,prgFormatCode[G1],numLine,
				ptrLine->getEnd()->getX(),
				ptrLine->getEnd()->getY()
			);
			strcat(buff,tmp);
			}
			else  //move to new point 
			{
			sprintf(tmp,prgFormatCode[SAFE_Z],numLine,
				safeDistZ);
			strcat(buff,tmp);
			numLine+=10;

			sprintf(tmp,prgFormatCode[G0],numLine,
				ptrLine->getStart()->getX(),
				ptrLine->getStart()->getY()
			);
			strcat(buff,tmp);
			numLine+=10;

			sprintf(tmp,prgFormatCode[SAFE_Z],numLine,
				workDistZ);
			strcat(buff,tmp);
			numLine+=10;

			sprintf(tmp,prgFormatCode[G1],numLine,
				ptrLine->getEnd()->getX(),
				ptrLine->getEnd()->getY()
			);
			strcat(buff,tmp);
			}
			numLine+=10;
			lastX=ptrLine->getEnd()->getX();
			lastY=ptrLine->getEnd()->getY();

			break;
		case tArc:
			ptrArc=(ArcSegment*)(entities[j]);

   			workDistZ=ptrArc->getZ();

			if (lastX==ptrArc->getStartX()&&
			    lastY==ptrArc->getStartY()){
		  	  if (ptrArc->getDirection()==AD_COUNTERCLOCKWISE)
			    sprintf(tmp,prgFormatCode[G2],numLine,
				ptrArc->getEndX(),
				ptrArc->getEndY(),
				ptrArc->getSignRad()
				);
			  else
			    sprintf(tmp,prgFormatCode[G3],numLine,
				ptrArc->getEndX(),
				ptrArc->getEndY(),
				ptrArc->getSignRad()
				);
			strcat(buff,tmp);
			}
			else //move to new point 
			{
			sprintf(tmp,prgFormatCode[SAFE_Z],numLine,
				safeDistZ);
			strcat(buff,tmp);
			numLine+=10;

			sprintf(tmp,prgFormatCode[G0],numLine,
				ptrArc->getStartX(),
				ptrArc->getStartY()
			);
			strcat(buff,tmp);
			numLine+=10;

			sprintf(tmp,prgFormatCode[SAFE_Z],numLine,
				workDistZ);
			strcat(buff,tmp);
			numLine+=10;

			  if (ptrArc->getDirection()==AD_COUNTERCLOCKWISE)
			     sprintf(tmp,prgFormatCode[G2],numLine,
				ptrArc->getEndX(),
				ptrArc->getEndY(),
				ptrArc->getSignRad()
				);
			  else
			sprintf(tmp,prgFormatCode[G3],numLine,
				ptrArc->getEndX(),
				ptrArc->getEndY(),
				ptrArc->getSignRad()
				);

			  strcat(buff,tmp);
			}
			numLine+=10;
			lastX=ptrArc->getEndX();
			lastY=ptrArc->getEndY();
		
			break;
		default:
				strcpy(buff,"NONE\n");
		}

	oufile.write(buff,strlen(buff));
	strcpy(buff,"");
   if (!oufile)
	MessageBox(modelWindow.getHWND(),"Error write file","SAVE...",MB_OK);

   }
   sprintf(buff,"N%d M30\n",numLine);
   oufile.write(buff,strlen(buff));
   oufile.close();
   return 0;
}

int Model::saveNC1000(const char *fn)
{
	float maxDistForTransition=300;//if distance>300mm use L=PSU

	int commentoSize;
	int centroSize;
	int parametriSize;
	int utensiliSize;
	int labelfSize;
	int labelcSize;
	int foraturaSize;
	int contornaturaSize;
	int tabelleforiSize;
	int confassistSize;
	int attrezzaggioSize;
	int customsezSize;

	string commento("[COMMENTO]\x0a");
       	string centro("[CENTRO01]\x0a");
       	string parametri("[PARAMETRI01]\x0a");
       	string utensili("[UTENSILI01]\x0a");
       	string contornatura("[CONTORNATURA01]\x0a");
       	string foratura("[FORATURA01]\x0a");
       	string tabellefori("[TABELLEFORI01]\x0a");
       	string labelc("[LABELC01]\x0a");
       	string labelf("[LABELF01]\x0a");
       	string confassist("[CONFASSIST01]\x0a");
       	string attrezzaggio("[ATTREZZAGGIO01]\x0a");
       	string customsez("[CUSTOMSEZ01]\x0a");

	string parametriValue("N10 G71 HC=1 LY=500.00 PLPZ=18 PCSG=0 PDM=0 PUOS=0 NFIL=0 BLO=0 ADAC=0 ACC=0 RUO=0 PRS=1 PRL=1 LZ=18.00 FIL1=0.000 FIL2=0.000 FIL=0 PPWQ=0.000 VAL1=0.000 VAL2=0.000 VAL3=0.000 KA=0 PVER=1.000 LX=478.00 POAY=0.00                   \x0a");

	int lenSection;

   	int j;
   	int numLine;
   	string codeLineBuffer;//buffer for code line
   	string programmBuffer;//pointer to buffer for program code
	char tmp[1024];//for convert num to string

   	float lastX;
   	float lastY;
	float nextX;
	float nextY;

   	Line * ptrLine;
   	ArcSegment * ptrArc;
   	ofstream oufile;
   	entityType etype;
   	float safeDistZ=2;
   	float workDistZ=-2;
   	float LPY=500;// invert coordinate
   	float LPZ=18;

   	enum {SELECT_TOOL,POSITIONING,G1,G2,G3,LPSU,G1Z};
   	const char *prgFormatCode[]=
   {
	   "N%i PAN=1 ST1=\"FR4\" ST3=\"NULL\" ST4=\"NULL\" L=PCUA\x0a",
	   "N%i X%.3f Y%.3f Z=PRK TP=1 PRF=%3.3f F=3 VF=3 AX=X,Y,Z G40 PFLO=0 PUL=1 M40 WBY(0)=1 L=PON TRZ=0\x0a",
	   "N%i X%.3f Y%.3f G1\x0a",
	   "N%i X%.3f Y%.3f R%.3f G5\x0a",
	   "N%i X%.3f Y%.3f R%.3f G4\x0a",
	   "N%i L=PSU\x0a",
	   "N%i Z%.3f G1\x0a"
   };

	if (FileName!="")
		oufile.open(fn,ios::binary);
	else 
	{
		sprintf(tmp,"empty file name %s",FileName);
		MessageBox(modelWindow.getHWND(),tmp,"Error",MB_OK);
	       	return -1;
	}

   	lastX=0.0;
   	lastY=0.0;
   	numLine=0;


   for (j=0;j<entities.size();j++)
   {
	etype=entities[j]->getType();

	if (etype==tLine)
	{
			ptrLine=(Line*)(entities[j]);
			ptrArc=NULL;
			nextX=ptrLine->getStart()->getX();
			nextY=ptrLine->getStart()->getY();
   			workDistZ=ptrLine->getZ();
	}
		else if (etype==tArc)
	{
			ptrArc=(ArcSegment*)(entities[j]);
			ptrLine=NULL;
			nextX=ptrArc->getStartX();
			nextY=ptrArc->getStartY();
   			workDistZ=ptrArc->getZ();
	}

	if (!numLine){
		numLine+=10;
		sprintf(tmp,prgFormatCode[SELECT_TOOL],numLine);
		programmBuffer=tmp;
		numLine+=10;

		sprintf(tmp,prgFormatCode[POSITIONING],numLine,
				nextX,
				LPY-nextY,
				workDistZ
			);
		

		programmBuffer+=tmp;
		numLine+=10;
	}
	else
	{
		if (lastX!=nextX&&lastY!=nextY)
		{
			//move to new point 
		   	float dist=getDistance(
					lastX,
					lastY,
					nextX,
					nextY
					);

			if(dist>maxDistForTransition)
			{	
			  sprintf(tmp,prgFormatCode[LPSU],numLine);
			  programmBuffer+=tmp;
			  numLine+=10;

			  sprintf(tmp,prgFormatCode[POSITIONING],numLine,
				nextX,
				LPY-nextY,
				workDistZ);
			  programmBuffer+=tmp;

			  numLine+=10; //far transition
			}
			else//short transition
			{
			  sprintf(tmp,prgFormatCode[G1Z],numLine,
				-LPZ-safeDistZ
			       );
			  programmBuffer+=tmp;
			  numLine+=10;

			  sprintf(tmp,prgFormatCode[G1],numLine,
				nextX,
				LPY-nextY
			       );
			  programmBuffer+=tmp;
			  numLine+=10;

			  sprintf(tmp,prgFormatCode[G1Z],numLine,
				-LPZ+workDistZ
			       );
			  programmBuffer+=tmp;
			  numLine+=10;
			}
		}
	}


	switch(etype){
		case tPoint:
			strcpy(tmp,"tPoint\x0a");
			break;
		case tLine:

			sprintf(tmp,prgFormatCode[G1],numLine,
				ptrLine->getEnd()->getX(),
				LPY-ptrLine->getEnd()->getY()
			);
			programmBuffer+=tmp;

			numLine+=10;
			lastX=ptrLine->getEnd()->getX();
			lastY=ptrLine->getEnd()->getY();

			break;
		case tArc:

			if (ptrArc->getDirection()==AD_COUNTERCLOCKWISE)
			     sprintf(tmp,prgFormatCode[G2],numLine,
				ptrArc->getEndX(),
				LPY-ptrArc->getEndY(),
				-ptrArc->getSignRad()
				);
			else
			     sprintf(tmp,prgFormatCode[G3],numLine,
				ptrArc->getEndX(),
				LPY-ptrArc->getEndY(),
				ptrArc->getSignRad()
				);

			programmBuffer+=tmp;

			numLine+=10;
			lastX=ptrArc->getEndX();
			lastY=ptrArc->getEndY();
		
			break;
		default:
				strcpy(tmp,"NONE\x0a");
		}
   }
   
   	sprintf(tmp,"N%d L=POFF\x0a",numLine);
	programmBuffer+=tmp;

   //[COMMENTO]
   	commento+=" \x0a\x0a";
	commentoSize=commento.length();

   //[PARAMETRI01]
   	parametriValue+="\%\x0a";
	parametri+=parametriValue;
	parametriSize=parametri.length();

   //[UTENSILI01]
	utensili+="FR4\n\%\x0a";
	utensiliSize=utensili.length();

   //[CONTORNATURA01]
   	programmBuffer+="\%\x0a";
	contornatura+=programmBuffer;
	contornaturaSize=contornatura.length();
	
   //[FORATURA01]
   	foratura+="\%\x0a";
	foraturaSize=foratura.length();

   //[TABELLEFORI]
   	tabellefori+="\%\x0a";
	tabelleforiSize=tabellefori.length();

   //[LABELC01]
   	labelc+="\%\x0a";
	labelcSize=labelc.length();

   //[LABELF01]
   	labelf+="\%\x0a";
	labelfSize=labelf.length();


   //[CONFASSIST01]
   	confassist+="\%\x0a";
	confassistSize=confassist.length();

   //[ATTREZZAGGIO01]
   	attrezzaggio+="\%\x0a";
	attrezzaggioSize=attrezzaggio.length();

   //[CUSTOMSEZ01]
   	customsez+="\%\x0a\[]";
	customsezSize=customsez.length();

   //[CENTRO01]
	sprintf(tmp,
"\x0a\
PARAMETRI=%09d\x0a\
UTENSILI=%09d\x0a\
LABELF=%09d\x0a\
LABELC=%09d\x0a\
FORATURA=%09d\x0a\
CONTORNATURA=%09d\x0a\
TABELLEFORI=%09d\x0a\
CONFASSIST=%09d\x0a\
ATTREZZAGGIO=%09d\x0a\
CUSTOMSEZ=%09d\x0a\x0a",
0,
0,
0,
0,
0,
0,
0,
0,
0,
0
);

	centroSize=centro.size()+strlen(tmp);

	int parametriOffset=commentoSize+centroSize+5;
	int utensiliOffset=parametriOffset+parametriSize;
	int contornaturaOffset=utensiliOffset+utensiliSize;
	int foraturaOffset=contornaturaOffset+contornaturaSize;
	int tabelleforiOffset=foraturaOffset+foraturaSize;
	int labelcOffset=tabelleforiOffset+tabelleforiSize;
	int labelfOffset=labelcOffset+labelcSize;
	int confassistOffset=labelfOffset+labelfSize;
	int attrezzaggioOffset=confassistOffset+confassistSize;
	int customsezOffset=attrezzaggioOffset+attrezzaggioSize;


	sprintf(tmp,"\x0a");
	centro+=tmp;
	sprintf(tmp, "PARAMETRI=%09d\x0a",parametriOffset);
	centro+=tmp;
	sprintf(tmp,"UTENSILI=%09d\x0a",utensiliOffset);
	centro+=tmp;
	sprintf(tmp,"LABELF=%09d\x0a",labelfOffset);
	centro+=tmp;
	sprintf(tmp,"LABELC=%09d\x0a",labelcOffset);
	centro+=tmp;
	sprintf(tmp,"FORATURA=%09d\x0a",foraturaOffset);
	centro+=tmp;
	sprintf(tmp,"CONTORNATURA=%09d\x0a",contornaturaOffset);
	centro+=tmp;
	sprintf(tmp,"TABELLEFORI=%09d\x0a",tabelleforiOffset);
	centro+=tmp;
	sprintf(tmp,"CONFASSIST=%09d\x0a",confassistOffset);
	centro+=tmp;
	sprintf(tmp,"ATTREZZAGGIO=%09d\x0a",attrezzaggioOffset);
	centro+=tmp;
	sprintf(tmp,"CUSTOMSEZ=%09d\x0a\x0a",customsezOffset);

	centro+=tmp;
	
	oufile<<commento<<centro<<parametri<<utensili<<contornatura;
	oufile<<foratura<<tabellefori<<labelc<<labelf<<confassist;
	oufile<<attrezzaggio<<customsez;

   if (!oufile)
	MessageBox(modelWindow.getHWND(),
			"Error write file","SAVE...",MB_OK);
   oufile.close();

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
int Model::scaleModel(float sf)
{
   int j;
   for (j=0;j<entities.size();j++)
	entities[j]->scale(sf);
   return 0;
}

bool Model::hitModel(int xPos,int yPos,int size){
   int result=0;
   int count=0;
   iter=entities.begin();
   if (!entities.empty()&&(entities.size()<entities.max_size()))
   for (iter;iter!=entities.end();++iter)
   {
	result=(*iter)->hitCursor(xPos,yPos,size);
	if (result)
		count++;
   }
   if (count==0)
		comStr.pActivEntity=NULL;
	return count;
}

int Model::loadDXF(const char *fName){
	char buffer[MAX];
	ifstream infile(fName);
	int line=0;
	int flag=OUTENTITY;
	float x1,y1,x2,y2,R;
	float startAngle,endAngle;
	float xc,yc;

	while(!infile.eof()){
		infile.getline(buffer,MAX);
		line++;
		if (streq(buffer,"ENTITIES"))
		{
			flag=INENTITY;
		}
		else 
			if (streq(buffer,"LINE")&&flag)
			{
			infile.getline(buffer,MAX);
			line++;
			while(strcmp(buffer,"  0"))
			{
				if (streq(buffer," 10")){
					infile.getline(buffer,MAX);
					line++;
					x1=atof(buffer);
					}
				else if (streq(buffer," 20")){
					infile.getline(buffer,MAX);
					line++;
					y1=atof(buffer);
					}
				else if (streq(buffer," 11")){
					infile.getline(buffer,MAX);
					line++;
					x2=atof(buffer);
					}
				else if (streq(buffer," 21")){
					infile.getline(buffer,MAX);
					line++;
					y2=atof(buffer);
					}
				infile.getline(buffer,MAX);
					line++;
			}
			Point p1(x1,y1);
			Point p2(x2,y2);
			appendLine(&p1,&p2);
			}
		else 
			if (streq(buffer,"ARC")&&flag){
			infile.getline(buffer,MAX);
			line++;
			while(strcmp(buffer,"  0"))
			{
				if (streq(buffer," 10")){
					infile.getline(buffer,MAX);
					line++;
					xc=atof(buffer);
					}
				else if (streq(buffer," 20")){
					infile.getline(buffer,MAX);
					line++;
					yc=atof(buffer);
					}
				else if (streq(buffer," 40")){
					infile.getline(buffer,MAX);
					line++;
					R=atof(buffer);
					}
				else if (streq(buffer," 50")){
					infile.getline(buffer,MAX);
					line++;
					startAngle=atof(buffer);
					}
				else if (streq(buffer," 51")){
					infile.getline(buffer,MAX);
					line++;
					endAngle=atof(buffer);
					}
				infile.getline(buffer,MAX);
					line++;
			}
			
			float anSt,anEn;
			anSt=startAngle*PI/180;
			anEn=endAngle*PI/180;
			x1=xc+R*cos(anSt);
			x2=xc+R*cos(anEn);
			y1=yc+R*sin(anSt);
			y2=yc+R*sin(anEn);

			appendArc(x1,y1,x2,y2,xc,yc,R,AD_CLOCKWISE);

			/*
			  appendArcCenAngRad(xc,yc,
					startAngle,
					endAngle,
					R);
			*/

			}
	}
	return 0;
}

float Model::getDistance(float xs, float ys, float xe, float ye)
{
	float res=sqrt((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys));
	return res;
}
