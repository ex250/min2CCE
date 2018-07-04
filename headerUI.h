#include <windows.h>
//#define _WIN32_IE	0x0300
#include <commctrl.h>
#include "resource.h"
#include <string>

const int STATE_WAIT_COMMAND=0;
const int STATE_LINE_POINT1=1;
const int STATE_LINE_POINT2=2;
const int STATE_ARC_POINT1=3;
const int STATE_ARC_POINT2=4;
const int STATE_ARC_POINT3=5;
const int STATE_ARC3P_POINT1=6;
const int STATE_ARC3P_POINT2=7;
const int STATE_ARC3P_POINT3=8;
const int STATE_CONTUR_ARC1=9;
const int STATE_CONTUR_ARC2=10;
const int STATE_CONTUR_ARC3=11;
const int STATE_CONTUR_LINE1=12;
const int STATE_CONTUR_LINE2=13;
const int STATE_CONTUR_SELECT=14;
const int STATE_TEXT_INSPOINT=20;
const int STATE_TEXT_ANGLE1=30;
const int STATE_TEXT_ANGLE2=35;
const int STATE_TEXT_LEN=40;
const float PI=3.14159265;
const int ON=1;
const int OFF=0;
const int MAXFILENAME=256;

const int HSTBAR=35; 	//высота строки состояния
const int WTBDRAW=30;   //ширина инстр. панели DRAW
const int HTBSTD=35;    //высота стандартной инстр. панели
const int WTBMODIFY=30;
const int HTBVERT=500;
const int XPOSWINDOW=50;
const int YPOSWINDOW=20;
const int HCOMSTR=90;
const int COMBO_SPACE_WIDTH=100;
const int COMBO_SPACE_HEIGHT=300;
const int SEPARATOR_WIDTH=20;


#if !defined(HEADERUI)
#define HEADERUI

typedef long LONG_PTR;


class BaseWindow
{
  protected:
	HWND hWnd;
	const WNDCLASS  *WindowClass;	
	DWORD dwStyle;
	int xPos;
	int yPos;
	int width;
	int height;
	HBRUSH hBrush;
	HPEN hPen;
	XFORM xForm; 

  public:

	BaseWindow();

	HWND getHWND();

	bool init(LPCTSTR windowName);

	bool init(LPCTSTR windowName,
	 	  DWORD dwStyle,
		  int x,
		  int y,
		  int cx,
		  int cy,
		  HWND hwndParent,
		  HMENU hMenu,
		  HINSTANCE hInst);

	ATOM registerClass(WNDCLASS&);

	bool show(int);
        int  getWidth();
        int  getHeight();
	bool setBrush(HBRUSH*);
	bool setPen(HPEN*);
	bool SetWindowPosition(int x,int y,int cx,int cy);

	bool printText(int , int ,const char*);

	bool lineTo(int ,int );

	bool line(int ,int, int, int );

	bool moveTo(int ,int );

	RECT getRect();

};

//---------------------TOOLBAR-----------------------------
class toolbarDraw:public BaseWindow
{
  protected:

    HIMAGELIST 	hImageList;         
    int 	cx,cy;
    UINT 	flag;
    int 	cInitial;
    int		cGrow;

    const int ImageListID;

 public:
    toolbarDraw();

    void setStyle(DWORD style);
    void setButtonSize(int ,int );

    int initSTD(  HWND hWndParent,
			LPCTSTR windowName,
			const int cInitial,
			TBBUTTON *tbButtons
			);


    int init(  HWND hWndParent,
			LPCTSTR windowName,
			const int cInitial,
			HBITMAP hBmpToolbar,
			TBBUTTON *tbButtons
			);

    void setPos(int,int);
    void setSize(int,int);
};

//---------------COMMAND LINE-------------------------------------------
class CommandLine:public BaseWindow
{
  private:
    static WNDPROC wpOrigEditProc;
    static LRESULT CALLBACK cmdProc(HWND,UINT,WPARAM,LPARAM);
    static LRESULT CALLBACK subProc(HWND,UINT,WPARAM,LPARAM);
    HWND hwndC;
    HWND hwndH;
    char buffer[128];   //буффер команды
    const char *pStrCmd;      //указатель на строку запрос 
    int state;		//индикатор текущего состояния системы
    int EnType;
    float x1,y1,x2,y2,x3,y3;// переменные для хранения координат точек
    float x4,y4,x5,y5,x6,y6;// переменные для хранения координат точек
    float xc,yc,R; // координаты центра и радиус окружности
    float fia,fib,fid;//angle 3 arc points
    int ArcDirection;
    int countStr;


 public:

    CommandLine();

    bool init(LPCTSTR windowName,
	  DWORD dwStyle,
	  int x, int y,
	  int cx,int cy,
	  HWND hwndParent,
	  HMENU hMenu,
	  HINSTANCE hInst);

    int getText(LPTSTR buffer, int maxChar);

    int getState();

    int setState(int);

    int setEnType(int);
    int getEnType();

    int enumEnType();

    bool addCommand();

    int  addCoordToHistory(float,float,int);

    int  addTextToHistory(const char *);

    bool segLine(float,float);

    bool segArc(float,float);

    bool contur(float,float);

    POINT getStartPOINT();

    int getRC(float x, float y);

    float getX1();
    float getY1();
    float getX2();
    float getY2();
    float getX3();
    float getY3();
    float getXc();
    float getYc();
    float getR();
    int getDirection();
    HWND getHWNDC();
};


//---------------MODEL WINDOW-------------------------------------------
class ModelWindow:public BaseWindow
{
 private:
    HDC hDC;
    POINT windowOrg;
    int xOrg, yOrg;
    int ROP2;
    float scaleFactor;
    HCURSOR hCursor;
 public:
    ModelWindow():BaseWindow()
    {
	windowOrg.x=0;
	windowOrg.y=0;
	xOrg=0;
	yOrg=0;
	ROP2=R2_COPYPEN;
    }

    bool init(LPCTSTR windowName,
	 	  DWORD dwStyle,
		  int x,
		  int y,
		  int cx,
		  int cy,
		  HWND hwndParent,
		  HMENU hMenu,
		  HINSTANCE hInst);

    HDC getHDC();

    bool setGM();		//єёЄрэютшЄ№ уЁрЇшўхёъшщ Ёхцшь

    bool setWOrg(float,float);  

    bool setScale(float);

    bool setROP2(int);
    
    bool line(float ,float, float, float );

    bool _arc(float, float, float, float, float, float, float, int );

    bool marker(float,float);

    bool myPolygon(CONST POINT *, int );

};

//----------------class coord system----------------------------------
class myCursor{
	private:
		float size;
		float arrowSize;
		float arrowAngle;
		bool visible;
	public:
		myCursor();
		void show();
		float getSize();
		float getArrowSize();
		bool setVisible(bool);
		bool setArrowSize(float);
		bool setArrowAngle(float);
};

//----------------class BmpPic----------------------------------
class BmpPic{
	private:
		static BOOL CALLBACK PicDlgProc(HWND,UINT,WPARAM,LPARAM);
		static std::string fileNames;
   		static OPENFILENAME ofn;
		int index;
		float curPosX;
		float curPosY;
		float alfa;
		float scale;
		int sizeX;
		int sizeY;
		float W;//width source image
		float H;
		float WW;//width transform image
		float HH;
		bool visible;
		BITMAP pv;
		HBITMAP hBitmap;

	public:

		BmpPic();
		bool open();
		void show();
		int getWidth();
		int getHeight();
		bool setVisible(bool);
		HBITMAP adaptedBMP(const char *);
};

class TextEntities {
	private:
		float insX;
		float insY;
		float angle;
		float len;
		short numLines;//index in combobox
		float tau;// tolerance
		short aproxSwitch;
		char *ptrStr;
		static BOOL CALLBACK TextDlgProc(HWND,UINT,WPARAM,LPARAM);

	public:

		TextEntities();
		void show();
		void setXY(float,float);
		void setAngle(float);
		float getInsX();
		float getInsY();
		float getAngle();
		bool makeText(LOGFONT *);
};

#endif

