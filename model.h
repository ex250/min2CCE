#ifndef MODELH
#define MODELH
#include <vector>
#include <process.h>
#include <fstream>
#include <string>
#include <typeinfo>
#include <sstream>
#include "myvector.h"
#include <stdlib.h>

#define streq(a,b)	(!strcmp(a,b))

#define MAXFILENAME	256
#define OFF	0
#define SOLID	1
#define SMALL	1
#define BLACK	0
#define MAXLAYER 8
#define DEFAULTCOLOR 	BLACK
#define DEFAULTWIDTH 	SMALL
#define DEFAULTTYPE	SOLID
#define TRUE		1
#define FALSE		0

enum toolType {T0,T1,T2,T3,T4,T5,T6,T7,T8,T9};

const float DEFAULT_SPEED=10.0;//mm/min
const toolType DEFAULT_TOOL=T0;//T0
const float DEFAULT_Z=0.0;
const float MIN_Z=-10.0;
const float MAX_Z=10.0;

enum {ONE,TWO,THREE,FOR,FIFE,SIX,SEVEN,EIGHT,NINE,TEN};
enum entityType {tPoint,tLine,tArc};

const int MAX=255;

enum {OUTENTITY,INENTITY};

class Layer
{
  private:
	bool hide;
	int type,width,color;
	float Z;
	float speed;
	toolType tool;
	static unsigned char countID;
	unsigned char ID;
  public:

	Layer();
	static unsigned char getCount();
	unsigned char getID();
	int getType()const;
	int getWidth()const;
	int getColor()const;
	float getZ()const;
	float getSpeed()const;
	toolType getTool()const;
	void show()const;
	void setZ(float);
	void setSpeed(float);
	void setTool(toolType);
};

class Entity
{
  protected:
	unsigned char layerID;
	bool selected;
	float Z;
	float speed;
	toolType tool;
  public:
	virtual void show()=0;
	virtual bool getDataFromUser()=0;
	virtual entityType getType();
	virtual bool hitCursor(int,int,int)=0;

	unsigned char getLayerID();
	bool setLayerID(unsigned char);	

	void setZ(float);
	float getZ();

	void setSpeed(float);
	float getSpeed();

	void setTool(toolType);
	toolType getTool();

	virtual bool printInfo()=0;
	virtual bool getInfo(char *)=0;
	virtual bool scale(float)=0;
	virtual bool move(float,float)=0;
/*	virtual	unsigned char getLayer()=0;
	virtual	bool setLayer(unsigned char)=0;	
	virtual bool rotate()=0;
	virtual bool mirror()=0;
	virtual bool offset()=0;
	virtual bool trim()=0;
	virtual bool lenghten()=0;
*/
};

class Point:public Entity
{
  private:
	float x,y;
	int type,width,color;

  public:
	Point();

	Point(Layer*);
	
	Point(float,float);

	Point(float,float,Layer*);

	Point(vec2);

	void show();

	bool printInfo();

	bool getInfo(char*);

	void getPoint();
	
	float getX();

	float getY();

	bool setXY(float,float);

	bool getDataFromUser();

	bool scale(float);

	bool move(float,float);

	bool rotate(float);

	bool rotRad(float);

	bool hitCursor(int,int,int);
};

class Line:public Entity
{
  private:
	Point start,end;
	int type,width,color;

  public:
	Line();

	Line(Layer*);

	Line(Point*,Point*);

	Line(Point*,Point*,Layer*);

	bool scale(float );

	bool move(float, float);

	bool setStart(float,float);

	bool setEnd(float,float);

	void show();

	bool getInfo(char*);

	bool printInfo();

	void getLine();

	bool getDataFromUser();

	bool hitCursor(int,int,int);

	Point* getStart();

	Point* getEnd();
};


class ArcSegment:public Entity
{
  private:
	int type,width,color;
	float xc,yc;
	float radius;
	float xs,ys, xe,ye;
	int direction;

  public:
	ArcSegment();
	ArcSegment(float,float,float,float,float,float,float,int);
	void show();

	bool scale(float);

	bool move(float,float);

	bool getInfo(char*);

	bool printInfo();

	bool getDataFromUser();

	bool hitCursor(int,int,int);

	float getAngle();

	float getStartX();
	float getStartY();
	float getEndX();
	float getEndY();
	float getRadius();
	int getDirection();
	float getSignRad();
};

class Model
{
  private:
	char FileName[256];
	std::vector<Entity*> entities;
	std::vector<Entity*>::iterator iter;
	Layer *ptrToDefaultLayer;
	Layer *ptrToCurrentLayer;
  public:
	Model();

	int  setFileName(char*);
	
	bool	appendLine(Point*,Point*);
	bool	appendPoint();
	bool	appendArc(float, float, float, float, float, float, float, int);
	bool	appendArcCenAngRad(float, float, float, float, float);

	bool 	addEntity(Entity*);
	int	deleteEntity(int[]);
	int	transformEntity(int[]);
	bool	hitModel(int,int,int);

	int readModel(char *);
	int writeModel(const char *);
	int saveGcodeISO(const char *);
	int saveNC1000(const char *);
	int loadDXF(const char *fName);
	int scaleModel(float);
	void showModel();
	int printModelInfo()const;
	int deleteAll();
	float getDistance(float,float,float,float);
};

#endif

