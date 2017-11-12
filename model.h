#include <vector>

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

enum {ONE,TWO,THREE,FOR,FIFE,SIX,SEVEN,EIGHT,NINE,TEN};

class Layer
{
  private:
	bool hide;
	int type,width,color;
	static unsigned char countID;
	unsigned char ID;
  public:

	Layer();
	static unsigned char getCount();
	unsigned char getID();
	int getType()const;
	int getWidth()const;
	int getColor()const;
	void show()const;
};

class Entity
{
  protected:
	unsigned char layerID;
	bool selected;
  public:
	virtual void show()=0;
	virtual bool getDataFromUser()=0;

	unsigned char getLayerID();
	bool setLayerID(unsigned char);	
	virtual bool printInfo()=0;
/*	virtual	unsigned char getLayer()=0;
	virtual	bool setLayer(unsigned char)=0;	
	virtual bool rotate()=0;
	virtual bool move()=0;
	virtual bool scale()=0;
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

	void show();

	bool printInfo();

	void getPoint();
	
	float getX();

	float getY();

	bool setXY(float,float);

	bool getDataFromUser();
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

	void show();

	bool printInfo();

	void getLine();

	bool getDataFromUser();
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
	ArcSegment(float,float,float,float,float,float,float);
	void show();

	bool printInfo();

	bool getDataFromUser();

};

class Model{
  private:
	std::vector<Entity*> entities;
	Layer *ptrToDefaultLayer;
	Layer *ptrToCurrentLayer;
  public:
	Model();
	
	bool	appendLine(Point*,Point*);
	bool	appendPoint();
	bool	appendArc(float, float, float, float, float, float, float);

	bool 	addEntity(Entity*);
	int	deleteEntity(int[]);
	int	transformEntity(int[]);

	int readModel();
	int writeModel();
	void showModel();
	int printModelInfo()const;
};


