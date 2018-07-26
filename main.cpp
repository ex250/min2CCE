#include "headerUI.h"
#include "wndclass.h"
#include "model.h"
#include <string>
#include "kdib.h"
#include <zmouse.h>
#include "myvector.h"

using namespace std;

long WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool arcBez(vec2,vec2 ,vec2 ,float,float);

BOOL APIENTRY dlgprc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;

BaseWindow mainWindow;
ModelWindow modelWindow;
BaseWindow statusBar;
BaseWindow tbStdWindow;
BaseWindow tbModifyWindow,tbDrawWindow;
CommandLine comStr;
toolbarDraw tbModify,tbDraw,tbStd;

BmpPic bmpPic;
TextEntities textEntities;

extern Layer* defaultLayer;
extern Layer* currentLayer;
extern Layer layer[];

Model myModel;
myCursor mycursor;

char buffer[180];
 
// Listing OFWIN_1
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
   MSG msg;                              // Windows message structure
   g_hInst=hInstance;


   mainWindow.registerClass(OFwin);
   mainWindow.init("Min2CCE");

   tbDrawWindow.registerClass(TBwin);
   tbDrawWindow.init("Примитив",
	WS_CHILD|WS_BORDER,
	0,
	HTBSTD,
	WTBDRAW,
	HTBVERT,
	mainWindow.getHWND(),
	NULL,
	g_hInst);

   tbModifyWindow.registerClass(TBwin);
   tbModifyWindow.init("Преобразовать",
	WS_CHILD|WS_BORDER,
	mainWindow.getWidth()-WTBMODIFY,
	HTBSTD,
	WTBMODIFY,
	HTBVERT,
	mainWindow.getHWND(),
	NULL,
        g_hInst);

   tbStdWindow.registerClass(TBwin);
   tbStdWindow.init("Стантартная",
	WS_CHILD|WS_BORDER,
	0,
	0,
	mainWindow.getWidth(),
	35,
	mainWindow.getHWND(),
	NULL,
        g_hInst);

   modelWindow.registerClass(modelWin);
   modelWindow.init("Окно модели",
	WS_CHILD|WS_BORDER,
	WTBDRAW,
	HTBSTD,
	mainWindow.getWidth()-tbDrawWindow.getWidth()-tbModifyWindow.getWidth()-10,
	HTBVERT,
	mainWindow.getHWND(),
	NULL,
        g_hInst);

   modelWindow.setGM();

   comStr.init("Командная строка",
	WS_CHILD|WS_BORDER,
	0,
	mainWindow.getHeight()-HSTBAR-HCOMSTR,
	mainWindow.getWidth(),
	HCOMSTR,
	mainWindow.getHWND(),
	NULL,
        g_hInst);


   statusBar.registerClass(TBwin);
   statusBar.init("Строка состояния",
	WS_CHILD|WS_BORDER,
	0,
	mainWindow.getHeight()-HSTBAR,
	mainWindow.getWidth(),
	HSTBAR,
	mainWindow.getHWND(),
	NULL,
        g_hInst);


   tbStd.setStyle(WS_CHILD|WS_BORDER);
   tbStd.setButtonSize(24,24);
   tbStd.initSTD(tbStdWindow.getHWND(),
	   "Стандартная Инструментальная панелька",
	   numTbStd,
	   TBSTD);

   tbDraw.setStyle(WS_CHILD|CCS_VERT|TBSTYLE_TOOLTIPS);

   HBITMAP hBmp = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDR_TOOLBAR1));

   tbDraw.init(tbDrawWindow.getHWND(),
	   "Draw",
	   numTbDraw,
	   hBmp,
	   TBDRAW);


   hBmp = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDR_TOOLBAR2));

   tbModify.setStyle(WS_CHILD|CCS_VERT|TBSTYLE_TOOLTIPS);
   tbModify.init(tbModifyWindow.getHWND(),
	   "Modify",
	   numTbModify,
	   hBmp,
	   TBMODIFY);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--------------создаем comboBox-------------------------------------
// Определение позиции и размеров для элемента управления Combo box
	HWND hwndCombo;
	int x, y, cx, cy;
	RECT rcItem;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// Создание элемента управления Combo box
	hwndCombo = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_LAYER,
		GetModuleHandle(NULL), 0);

// Инициализация списка для hwndCombo

        for(int i=0;i<Layer::getCount();i++){
		sprintf(buffer,"слой %d",i);
		SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)buffer);
	}

	SendMessage(hwndCombo, CB_SETCURSEL, 0, 0);
 
	HWND hwndComboColor;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4+cx;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// Создание элемента управления Combo box
	hwndComboColor = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_COLOR,
		GetModuleHandle(NULL), 0);

// Инициализация списка для hwndCombo
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"по слою");
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"зеленый");
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"синий");
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"красный");
	SendMessage(hwndComboColor, CB_SETCURSEL, 0, 0);


	HWND hwndComboType;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4+cx+cx;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// Создание элемента управления Combo box
	hwndComboType = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_TYPE,
		GetModuleHandle(NULL), 0);

// Инициализация списка для hwndCombo
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"по слою");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"Сплошная");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"Штриховая");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"Пунктир");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"Штрих-пунктир");
	SendMessage(hwndComboType, CB_SETCURSEL, 0, 0);

	HWND hwndComboWidth;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4+cx+cx+cx;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// Создание элемента управления Combo box
	hwndComboWidth = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_WIDTH,
		GetModuleHandle(NULL), 0);

// Инициализация списка для hwndCombo
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"по слою");
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"Тонкая");
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"Средняя");
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"Толстая");
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"0.05mm");
	SendMessage(hwndComboWidth, CB_SETCURSEL, 0, 0);

   mainWindow.SetWindowPosition(XPOSWINDOW,YPOSWINDOW,
		(HTBSTD+HTBVERT+HSTBAR+HCOMSTR)*1.62,
		HTBSTD+HTBVERT+HSTBAR+HSTBAR+HCOMSTR);

   mainWindow.show(SW_SHOW);
   tbDrawWindow.show(SW_SHOW);
   tbModifyWindow.show(SW_SHOW);
   tbStdWindow.show(SW_SHOW);
   statusBar.show(SW_SHOW);
   modelWindow.show(SW_SHOW);
   comStr.show(SW_SHOW);

   // The message loop
   while(GetMessage(&msg, 0, 0, 0) == TRUE)   // Get any messages
   {
      TranslateMessage(&msg);                 // Translate the message
      DispatchMessage(&msg);                  // Dispatch the message
   }

   return msg.wParam;                         // End so return to Windows
}


//**********************************************************************
 
long WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam,
                       LPARAM lParam)
{
   HDC hDC;                       // Display context handle
   PAINTSTRUCT PaintSt;           // Structure defining area to be drawn
   RECT aRect;                    // A working rectangle
   static LPPOINT lpPoint;//****
   POINT cursPos;
   lpPoint=&cursPos;
   static char buffer[128];
   static int xPrev,yPrev;
   static int mode=0;
   static POINT origin={0,0};
   static POINT prevCursPos;
   static POINT prevPosition;
   static XFORM xForm;
   static bool flagMiddlePress=false;
   static bool flagRegen=false;
   static bool bTracking=false;
   int prevMode;
   RECT rect;
   char str[30];
   char str1[30];
   static char buf[50];
   float aspect;
   int wheel_number;
   short ticks=0;
   int direction=0;
   static float scaleFactor=1;
   RECT rectStr={100,10,300,30};
	
   HPEN hPen;
   int ItemIndex;
   XFORM rMatrix;

   BOOL success;
   static OPENFILENAME ofn;
   char szFile[MAXFILENAME];
   int dX, dY;
   int ws, hs, wd, hd;
   BOOL isFileLoaded;
   BOOL isImageLoaded;
   int substrate;

   static HINSTANCE hInst;
   static HICON hIcon;

   static vec2 tempA,tempB;

   LPTOOLTIPTEXT lpTTT;

   static HMENU hMenuShape;

   switch(message)                // Process selected messages
   {  
      case WM_CREATE:
	    if (mode++==0){
	       ofn.lStructSize = sizeof(OPENFILENAME);
	       ofn.hwndOwner = hWnd;
	       ofn.lpstrFile = szFile;
	       ofn.nMaxFile = sizeof(szFile);
	       hInst=GetModuleHandle(NULL);
	       hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICONMIN2C));
	       SetClassLong(hWnd,GCL_HICON,(LONG)hIcon);

	       hMenuShape=LoadMenu(GetModuleHandle(NULL),
			       MAKEINTRESOURCE(IDR_MENU_SHAPE));
	       hMenuShape=GetSubMenu(hMenuShape,0);
	    }

      break;

      case WM_SIZE:
	 tbStdWindow.SetWindowPosition(0,0,mainWindow.getWidth(),HTBSTD);
	 SendMessage(tbStd.getHWND(),TB_AUTOSIZE,0,0);

	 tbDrawWindow.SetWindowPosition(0,
			HTBSTD,
			WTBDRAW,
			mainWindow.getHeight()-HSTBAR-HTBSTD-HCOMSTR);

	 SendMessage(tbDraw.getHWND(),TB_AUTOSIZE,0,0);

	 tbModifyWindow.SetWindowPosition(mainWindow.getWidth()-WTBMODIFY,
			HTBSTD,
			WTBMODIFY,
			mainWindow.getHeight()-HSTBAR-HTBSTD-HCOMSTR);

	 SendMessage(tbModify.getHWND(),TB_AUTOSIZE,0,0);

	 modelWindow.SetWindowPosition(tbDrawWindow.getWidth(),
  	  HTBSTD,
	  mainWindow.getWidth()-tbDrawWindow.getWidth()-tbModifyWindow.getWidth(),
	  mainWindow.getHeight()-HSTBAR-HTBSTD-HCOMSTR);

	 comStr.SetWindowPosition(0,
	  mainWindow.getHeight()-HSTBAR-HCOMSTR,
	  mainWindow.getWidth(),
	  HCOMSTR);

	 statusBar.SetWindowPosition(0,
	   mainWindow.getHeight()-HSTBAR,
	   mainWindow.getWidth(),
	   HSTBAR);

	 break;


	case WM_MOUSEMOVE:
		if (hWnd==modelWindow.getHWND()){
			hDC=GetDC(modelWindow.getHWND());
		   if (flagMiddlePress==true)
		     {
		      flagRegen=false;
		      GetCursorPos(lpPoint);
		      int dx=lpPoint->x-prevPosition.x;
		      int dy=lpPoint->y-prevPosition.y;
		      xForm.eM11=1;
		      xForm.eM22=1;
		      xForm.eDx=dx*50;
		      xForm.eDy=-dy*50;
		      
	      	      ModifyWorldTransform(hDC,&xForm,MWT_RIGHTMULTIPLY);
	      	      GetWorldTransform(hDC,&xForm);
		      prevPosition=*lpPoint;
	              GetClientRect(hWnd,&aRect);
	              InvalidateRect(hWnd,&aRect,true);
		     }
		   else {
		   GetCursorPos(lpPoint);

		   prevPosition=*lpPoint;
		   ScreenToClient(hWnd,lpPoint);
		   DPtoLP(hDC,lpPoint,1);


		   sprintf(buffer,"Coordinate x=%4.3f y=%4.3f       ",static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		   strcat(buffer,buf);
      		   statusBar.printText(10,10,buffer);


		   switch (comStr.getState()){
			   case STATE_SELECTRECT:
			if (flagRegen==true)
				modelWindow.myRectangle(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
			modelWindow.myRectangle(comStr.getX1(),comStr.getY1(),static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		        prevCursPos=*lpPoint;
			flagRegen=true;
				   break;

			case STATE_RECT_POINT2:
			if (flagRegen==true)
				modelWindow.myRectangle(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
			modelWindow.myRectangle(comStr.getX1(),comStr.getY1(),static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		        prevCursPos=*lpPoint;
			flagRegen=true;
				   break;

			case STATE_MOVE_P2:
			if (flagRegen==true){
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
			}
			modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		        prevCursPos=*lpPoint;
			flagRegen=true;
				   break;

			   case STATE_LINE_POINT2:
		   	if (flagRegen==true)
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
			modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		        prevCursPos=*lpPoint;
			flagRegen=true;
			break;
			   case STATE_ARC_POINT1:
		        prevCursPos=*lpPoint;
			break;
			   case STATE_ARC_POINT2:

		   	if (flagRegen==true)
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
			modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		        prevCursPos=*lpPoint;
			flagRegen=true;

		        break;

			   case STATE_ARC_POINT3:
			comStr.getRC(static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);

		     if (flagRegen==true)
			modelWindow._arc(comStr.getX1(),
					 comStr.getY1(),
					 comStr.getX3(),
					 comStr.getY3(),
                                         comStr.getXc(),
                                         comStr.getYc(),
					 comStr.getR(),
					 comStr.getDirection()
			);

			comStr.getRC(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);

			modelWindow._arc(comStr.getX1(),
					 comStr.getY1(),
					 comStr.getX3(),
					 comStr.getY3(),
                                         comStr.getXc(),
                                         comStr.getYc(),
					 comStr.getR(),
					 comStr.getDirection()
			);

		        prevCursPos=*lpPoint;
				flagRegen=true;
			break;
			   case STATE_CONTUR_LINE2:
				if (flagRegen==true)
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		        	prevCursPos=*lpPoint;
			flagRegen=true;
			break;
			   case STATE_CONTUR_ARC2:
			if (flagRegen==true)
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
			modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
		        prevCursPos=*lpPoint;
			flagRegen=true;

		        break;
			   case STATE_CONTUR_ARC3:
			comStr.getRC(static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);

		     if (flagRegen==true)
			modelWindow._arc(comStr.getX1(),
					 comStr.getY1(),
					 comStr.getX3(),
					 comStr.getY3(),
                                         comStr.getXc(),
                                         comStr.getYc(),
					 comStr.getR(),
					 comStr.getDirection()
			);

			comStr.getRC(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);

			modelWindow._arc(comStr.getX1(),
					 comStr.getY1(),
					 comStr.getX3(),
					 comStr.getY3(),
                                         comStr.getXc(),
                                         comStr.getYc(),
					 comStr.getR(),
					 comStr.getDirection()
			);

		        prevCursPos=*lpPoint;
			flagRegen=true;

			case STATE_TEXT_ANGLE1:
				if (flagRegen==true)
				modelWindow.line(textEntities.getInsX(),
						textEntities.getInsY(),
				static_cast<float>(prevCursPos.x)/100,
				static_cast<float>(prevCursPos.y)/100);

				modelWindow.line(textEntities.getInsX(),
						textEntities.getInsY(),
				static_cast<float>(lpPoint->x)/100,
				static_cast<float>(lpPoint->y)/100);

		        	prevCursPos=*lpPoint;
				flagRegen=true;
			break;
		   }
		   }
		   ReleaseDC(modelWindow.getHWND(),hDC);
		}

		break;
	
	case WM_LBUTTONUP:

		break; 


	case WM_LBUTTONDOWN:
		SetFocus(modelWindow.getHWND());
		if (hWnd==modelWindow.getHWND()){
		  hDC=GetDC(modelWindow.getHWND());

		  GetCursorPos(lpPoint);
		  ScreenToClient(hWnd,lpPoint);
		  DPtoLP(hDC,lpPoint,1);
		  switch (comStr.getState()){
			case STATE_WAIT_COMMAND:
 	           	if (!myModel.hitModel(lpPoint->x,lpPoint->y,
					mycursor.getSize()))
			{
				comStr.selectRect(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
			}
				break;
			case STATE_SELECTRECT:
				comStr.selectRect(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				flagRegen=false;
				break;

			case STATE_RECT_POINT1:
				comStr.segRect(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				break;

			case STATE_RECT_POINT2:
				comStr.segRect(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				flagRegen=false;
				break;

			case STATE_MOVE_P1:
				comStr.segMove(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				break;

			case STATE_MOVE_P2:
				comStr.segMove(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				flagRegen=false;
				break;

			case STATE_LINE_POINT1:
			        prevCursPos=*lpPoint;
				comStr.segLine(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				break;
			case STATE_LINE_POINT2:
				comStr.segLine(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				myModel.showModel();
				flagRegen=false;
				break;
			case STATE_ARC_POINT1:
				comStr.segArc(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				break;
			case STATE_ARC_POINT2:
				comStr.segArc(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
			modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
				flagRegen=false;
				break;
			case STATE_ARC_POINT3:
				comStr.segArc(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				myModel.showModel();
				flagRegen=false;
				break;
				
			case STATE_CONTUR_LINE1:
			        prevCursPos=*lpPoint;
				comStr.contur(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				break;
			case STATE_CONTUR_LINE2:
				comStr.contur(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				flagRegen=false;
				break;

			case STATE_CONTUR_ARC1:
				comStr.contur(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				break;

			case STATE_CONTUR_ARC2:
				comStr.contur(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
			modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
				flagRegen=false;
				break;

			case STATE_CONTUR_ARC3:
				comStr.contur(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				flagRegen=false;
				break;

			case STATE_TEXT_INSPOINT:
				textEntities.setXY(static_cast<float>(lpPoint->x)/100,static_cast<float>(lpPoint->y)/100);
				comStr.addTextToHistory("Input point");
				comStr.setState(STATE_WAIT_COMMAND);
				textEntities.show();
			break;

			case STATE_TEXT_ANGLE1:
			{
				modelWindow.line(textEntities.getInsX(),
						textEntities.getInsY(),
				static_cast<float>(prevCursPos.x)/100,
				static_cast<float>(prevCursPos.y)/100);

			        prevCursPos=*lpPoint;

				tempA.x=textEntities.getInsX();
				tempA.y=textEntities.getInsY();
				tempB.x=((float)(lpPoint->x))/100;
				tempB.y=((float)(lpPoint->y))/100;
				tempB-=tempA;
				tempA.x=1;
				tempA.y=0;
				tempB.normalize();

				float cosAn=dot(tempA,tempB);
				float an=acos(cosAn)*180/PI;

				if (tempB.y<0&&tempB.x<0)
					an=360-an;
				else if (tempB.y<0&&tempB.x>0)
					an=360-an;
					

				textEntities.setAngle(an);
				comStr.setState(STATE_WAIT_COMMAND);
				textEntities.show();
	    			modelWindow.setROP2(R2_COPYPEN);
			}
			break;

		  }
		  ReleaseDC(modelWindow.getHWND(),hDC);
		}
		break;

	case WM_RBUTTONDOWN:
		
		if (comStr.getState()==STATE_CONTUR_LINE2||
			comStr.getState()==STATE_CONTUR_ARC2)
		{
		      comStr.enumEnType();
		      if (comStr.getEnType()==tLine)
			      comStr.setState(STATE_CONTUR_LINE2);
		      else
			      comStr.setState(STATE_CONTUR_ARC2);
		}
		break;

    case WM_MBUTTONDOWN:
		if (hWnd==modelWindow.getHWND()){

			hDC=GetDC(modelWindow.getHWND());
			flagMiddlePress=true;
		   	GetCursorPos(&prevPosition);
			ReleaseDC(modelWindow.getHWND(),hDC);

		}
	break;

    case WM_MBUTTONUP:
		if (flagMiddlePress==true){
		   flagMiddlePress=false;
      		   aRect=modelWindow.getRect();
      		   InvalidateRect(modelWindow.getHWND(),&aRect,TRUE);

		}

	break;

    case WM_MOUSEWHEEL:
	if (hWnd==modelWindow.getHWND()){

	flagRegen=false;

	hDC=GetDC(hWnd);
	ticks=(short)HIWORD(wParam);
	      GetCursorPos(lpPoint);
	      
	      if (ticks>0){
	      xForm.eM11=1.25;
	      xForm.eM22=1.25;
	      }
	      else{
	      xForm.eM11=0.8;
	      xForm.eM22=0.8;
	      }
	      xForm.eDx=static_cast<float>(lpPoint->x)*(1-xForm.eM11);
	      xForm.eDy=static_cast<float>(lpPoint->y)*(1-xForm.eM11);
	      ModifyWorldTransform(hDC,&xForm,MWT_RIGHTMULTIPLY);
	      GetWorldTransform(hDC,&xForm);
	  ReleaseDC(hWnd,hDC);
	      GetClientRect(hWnd,&aRect);
	      InvalidateRect(hWnd,&aRect,true);
	}

      sprintf(buf,"Scale:%3.3f       ",xForm.eM11);

      statusBar.printText(10,10,buffer);

      break;

      case WM_KEYDOWN:
      switch(wParam){
	      case VK_ESCAPE:
		      comStr.setState(STATE_WAIT_COMMAND);
		      modelWindow.setROP2(R2_COPYPEN);
		      myModel.showModel();
		      flagRegen=false;
		break;
	      case VK_SPACE:
		switch(comStr.getState()){
			case STATE_CONTUR_LINE2:
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
				break;
			case STATE_CONTUR_ARC2:
				modelWindow.line(comStr.getX1(),comStr.getY1(),static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);
				break;
			case STATE_CONTUR_ARC3:
			comStr.getRC(static_cast<float>(prevCursPos.x)/100,static_cast<float>(prevCursPos.y)/100);

			modelWindow._arc(comStr.getX1(),
					 comStr.getY1(),
					 comStr.getX3(),
					 comStr.getY3(),
                                         comStr.getXc(),
                                         comStr.getYc(),
					 comStr.getR(),
					 comStr.getDirection()
			);
				break;
		}
		comStr.setState(STATE_WAIT_COMMAND);

		break;
	      case VK_TAB:
		SetFocus(comStr.getHWNDC());
		break;
      }
      break;

      case WM_CONTEXTMENU:
		  hDC=GetDC(modelWindow.getHWND());
		  GetCursorPos(lpPoint);
		  ScreenToClient(hWnd,lpPoint);
		  DPtoLP(hDC,lpPoint,1);
		  switch (comStr.getState()){
			case STATE_WAIT_COMMAND:
				if (comStr.pActivEntity)
				if (((Entity *)(comStr.pActivEntity))->
				hitCursor(lpPoint->x,lpPoint->y,4))
				{
		  	GetCursorPos(lpPoint);
			TrackPopupMenuEx(hMenuShape,0,
				cursPos.x,cursPos.y,hWnd,
				NULL);
				}
		   ReleaseDC(modelWindow.getHWND(),hDC);
			break;
		  }
      break;
      case WM_PAINT:                       // Message is to redraw the window
		//statusBar.printText(10,10,buffer);
		hDC = BeginPaint(hWnd, &PaintSt);
		if (hWnd==statusBar.getHWND()){
  		    SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  		    TextOut(hDC,10,10,buffer,strlen(buffer));
		}
		if (hWnd==modelWindow.getHWND()){
		   if (flagMiddlePress==false)
		   		bmpPic.show();
    		   myModel.showModel();
		   mycursor.show();
		}
	        EndPaint(hWnd, &PaintSt); // Terminate window redraw operation
		
		break;
      case WM_NOTIFY:
		lpTTT=(LPTOOLTIPTEXT)lParam;
		if (lpTTT->hdr.code==TTN_NEEDTEXT){
			lpTTT->hinst=GetModuleHandle(NULL);
			lpTTT->lpszText=MAKEINTRESOURCE(lpTTT->hdr.idFrom);
		}
		break;

      case WM_COMMAND:
	 switch (LOWORD(wParam))
	 {
//----------------- STANTARD-------------------------------------------------
		case IDM_OPEN:
			if (strlen(ofn.lpstrFile)>1){
				MessageBox(hWnd,"close prev File","Information",MB_HELP);
			}
			strcpy(szFile, "");
			success = GetOpenFileName(&ofn);
			if (success){
				myModel.readModel(ofn.lpstrFile);
				myModel.showModel();
			}
			else
				MessageBox(hWnd,"error open File","ERROR",MB_HELP);
			break;
		
		

		case IDM_NEW:
			
			MessageBox(hWnd, "Выбран пункт 'New'", "Меню File", MB_OK);
			break;

 		case IDM_SAVE:
			myModel.saveInfo("info.txt");
			MessageBox(hWnd, "Выбран пункт 'Save'", "Меню File", MB_OK);
			break;

 		case IDM_SAVEAS:
			//strcpy(szFile, "");
			success = GetSaveFileName(&ofn);
			if (success){
				myModel.writeModel(ofn.lpstrFile);
			}
			else
				MessageBox(hWnd, "GetSaveFileName",
				"Отказ от выбора или ошибка", MB_ICONWARNING);
			break;


		case IDM_EXIT:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
//-----------------DRAW---------------------------------------------------
		case IDM_LINE:
			comStr.segLine(0,0);
			break;
		case IDM_ARCSTARTENDRADIUS:
			comStr.segArc(0,0);
			break;
		case IDM_ARC3POINTS:
			comStr.setState(STATE_WAIT_COMMAND);
			break;
		case IDM_ARCCENTREANG1ANG2:
			MessageBox(hWnd, "Выбран пункт 'ДУГА ЦЕНТР РАДИУС УГОЛ1 УГОЛ2'", "Меню Примитив", MB_OK);
			break;

		case IDM_CIRCCENTRRAD:
			MessageBox(hWnd, "Выбран пункт 'ОКРУЖНОСТЬ ЦЕНТР РАДИУС'", "Меню Примитив", MB_OK);
			break;
		case IDM_CIRCCENTRDIAM:
			MessageBox(hWnd, "Выбран пункт 'ОКРУЖНОСТЬ ЦЕНТР ДИАМЕТР'", "Меню Примитив", MB_OK);
			break;

		case IDM_CIRC3POINTS:
			MessageBox(hWnd, "Выбран пункт 'ОКРУЖНОСТЬ ПО 3 ТОЧКАМ'", "Меню Примитив", MB_OK);
			break;

		case IDM_CONTUR:
			comStr.contur(0,0);
			break;
		case IDM_RECTANGLE:
			comStr.segRect(0,0);
			break;
		case IDM_POLYGON:
			MessageBox(hWnd, "Выбран пункт 'ПОЛИГОН'", "Меню Примитив", MB_OK);
			break;
		case IDM_TEXT:
			textEntities.show();
			break;

//-----------------MODIFY-------------------------------------------------
		case IDM_ERASE:
			myModel.deleteAll();	
			break;

		case IDM_PCOPY:
			MessageBox(hWnd, "Выбран пункт 'PCOPY'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_MIRROR:
			MessageBox(hWnd, "Выбран пункт 'MIRROR'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_OFFSET:
			MessageBox(hWnd, "Выбран пункт 'OFFSET'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_ARRAY:
			MessageBox(hWnd, "Выбран пункт 'ARRAY'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_MOVE:
			comStr.segMove(0,0);
			break;

		case IDM_ROTATE:
			MessageBox(hWnd, "Выбран пункт 'ROTATE'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_SCALE:
			myModel.scaleModel(3.63);
			MessageBox(hWnd, "Выбран пункт 'SCALE'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_TRIM:
			MessageBox(hWnd, "Выбран пункт 'ОБРЕЗАТЬ'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_LENGTH:
			MessageBox(hWnd, "Выбран пункт 'УДЛИНИТЬ'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_CHAMFER:
			MessageBox(hWnd, "Выбран пункт 'ФАСКА'", "Меню Преобразовать", MB_OK);
			break;

		case IDM_FILLET:
			MessageBox(hWnd, "Выбран пункт 'СОПРЯЖЕНИЕ'", "Меню Преобразовать", MB_OK);
			//modelWindow.printText(10,10,"Окно модели. Выбрано сопряжение");
			break;
//------------------S U B S T R A T E-------------------------------
		case IDM_INSERTBMP:
			bmpPic.open();
			
			break;

//-----------------menu VIEW------------------------------------------
		case  IDM_REDRAW:
			 myModel.showModel();
			break;
		case IDC_TBCOMBO_LAYER:
			if (HIWORD(wParam)==CBN_SELCHANGE){
                           ItemIndex = SendMessage((HWND) lParam, (UINT) CB_GETCURSEL, 
                               (WPARAM) 0, (LPARAM) 0);
			   if (ItemIndex<Layer::getCount()){
			       currentLayer=&layer[ItemIndex];
			       hPen=CreatePen(currentLayer->getType(), currentLayer->getWidth(), RGB((currentLayer->getColor()&0xff0000)>>16, (currentLayer->getColor()&0xff00)>>8,currentLayer->getColor()&0xff));
			       /*LOGBRUSH LBrush;
			       LBrush.lbStyle=BS_SOLID;
			       LBrush.lbColor=RGB(0,0,0);
			       LBrush.lbHatch=HS_CROSS;
			       hPen=ExtCreatePen(currentLayer->getType()|PS_GEOMETRIC,
			       currentLayer->getWidth(),
			       &LBrush,
			       NULL,
			       NULL
			       );
			       */
               		       modelWindow.setPen(&hPen);
			   }
			}
			break;
		case IDM_HELPABOUT:
	   		DialogBox(NULL,MAKEINTRESOURCE(IDD_HELPDIALOG1),
					hWnd,dlgprc);
			break;
				
		case IDM_HELPINFO:
			MessageBox(hWnd, "Help info", "help info", MB_OK);
			break;

        	default:
			break;
	}
	break;
     

      case WM_DESTROY:             // Window is being destroyed
         PostQuitMessage(0);
         break;

      default:             // Any other message - we don't want to know,
                           // so call default message processing
         return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

//************************************************************************
BOOL APIENTRY dlgprc(HWND hDlg, UINT uMsg,WPARAM wParam, LPARAM lParam){
	static HICON hIcon;
	switch(uMsg){
		case WM_INITDIALOG:
		
	        hIcon=LoadIcon((HINSTANCE)GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICONMIN2C));
		if(SendDlgItemMessage(
					hDlg,
					IDI_ICONMIN3C,
					STM_SETICON,
					(WPARAM)hIcon,
					(LPARAM)NULL
					))
			MessageBox(hDlg,"ERROR LOAD ICON","ERROR",MB_OK);
		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;

	}
	return false;
}
