#include "headerUI.h"
#include "wndclass.h"
#include "model.h"
#include <string>

using namespace std;

long WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void TransformAndDraw(int iTransform, HWND hWnd,int scale,LPPOINT lpPoint);

HINSTANCE g_hInst;

BaseWindow mainWindow;
ModelWindow modelWindow;
BaseWindow statusBar;
BaseWindow tbStdWindow;
BaseWindow tbModifyWindow,tbDrawWindow;
CommandLine comStr;
toolbarDraw tbModify,tbDraw,tbStd;

extern Layer* defaultLayer;
extern Layer* currentLayer;
extern Layer layer[];

Model myModel;

char buffer[180];

// Listing OFWIN_1
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
   MSG msg;                              // Windows message structure
   g_hInst=hInstance;


   mainWindow.registerClass(OFwin);
   mainWindow.init("������� ����");

   tbDrawWindow.registerClass(TBwin);
   tbDrawWindow.init("��������",
	WS_CHILD|WS_BORDER,
	0,
	HTBSTD,
	WTBDRAW,
	HTBVERT,
	mainWindow.getHWND(),
	NULL,
	g_hInst);

   tbModifyWindow.registerClass(TBwin);
   tbModifyWindow.init("�������������",
	WS_CHILD|WS_BORDER,
	mainWindow.getWidth()-WTBMODIFY,
	HTBSTD,
	WTBMODIFY,
	HTBVERT,
	mainWindow.getHWND(),
	NULL,
        g_hInst);

   tbStdWindow.registerClass(TBwin);
   tbStdWindow.init("�����������",
	WS_CHILD|WS_BORDER,
	0,
	0,
	mainWindow.getWidth(),
	35,
	mainWindow.getHWND(),
	NULL,
        g_hInst);

   modelWindow.registerClass(modelWin);
   modelWindow.init("���� ������",
	WS_CHILD|WS_BORDER,
	WTBDRAW,
	HTBSTD,
	mainWindow.getWidth()-tbDrawWindow.getWidth()-tbModifyWindow.getWidth()-10,
	HTBVERT,
	mainWindow.getHWND(),
	NULL,
        g_hInst);

   modelWindow.setGM();

   comStr.init("��������� ������",
	WS_CHILD|WS_BORDER,
	0,
	mainWindow.getHeight()-HSTBAR-HCOMSTR,
	mainWindow.getWidth(),
	HCOMSTR,
	mainWindow.getHWND(),
	NULL,
        g_hInst);


   statusBar.registerClass(TBwin);
   statusBar.init("������ ���������",
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
	   "����������� ���������������� ��������",
	   numTbStd,
	   TBSTD);

   tbDraw.setStyle(WS_CHILD|CCS_VERT);

   HBITMAP hBmp = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDR_TOOLBAR1));

   tbDraw.init(tbDrawWindow.getHWND(),
	   "Draw",
	   numTbDraw,
	   hBmp,
	   TBDRAW);


   hBmp = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDR_TOOLBAR2));

   tbModify.setStyle(WS_CHILD|CCS_VERT);
   tbModify.init(tbModifyWindow.getHWND(),
	   "Modify",
	   numTbModify,
	   hBmp,
	   TBMODIFY);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--------------������� comboBox-------------------------------------
// ����������� ������� � �������� ��� �������� ���������� Combo box
	HWND hwndCombo;
	int x, y, cx, cy;
	RECT rcItem;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// �������� �������� ���������� Combo box
	hwndCombo = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_LAYER,
		GetModuleHandle(NULL), 0);

// ������������� ������ ��� hwndCombo

        for(int i=0;i<Layer::getCount();i++){
		sprintf(buffer,"���� %d",i);
		SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)buffer);
	}

	SendMessage(hwndCombo, CB_SETCURSEL, 0, 0);
 
	HWND hwndComboColor;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4+cx;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// �������� �������� ���������� Combo box
	hwndComboColor = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_COLOR,
		GetModuleHandle(NULL), 0);

// ������������� ������ ��� hwndCombo
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"�� ����");
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"�������");
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"�����");
	SendMessage(hwndComboColor, CB_ADDSTRING, 0, (LPARAM)"�������");
	SendMessage(hwndComboColor, CB_SETCURSEL, 0, 0);


	HWND hwndComboType;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4+cx+cx;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// �������� �������� ���������� Combo box
	hwndComboType = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_TYPE,
		GetModuleHandle(NULL), 0);

// ������������� ������ ��� hwndCombo
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"�� ����");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"��������");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"���������");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"�������");
	SendMessage(hwndComboType, CB_ADDSTRING, 0, (LPARAM)"�����-�������");
	SendMessage(hwndComboType, CB_SETCURSEL, 0, 0);

	HWND hwndComboWidth;
	SendMessage(tbStd.getHWND(), TB_GETITEMRECT, 0, (LPARAM)&rcItem);
	x = rcItem.left + (24)*4+cx+cx+cx;     y = rcItem.top;
	cx = COMBO_SPACE_WIDTH;  cy = COMBO_SPACE_HEIGHT;

// �������� �������� ���������� Combo box
	hwndComboWidth = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE |
		CBS_DROPDOWNLIST, x, y, cx, cy, tbStd.getHWND(), (HMENU)IDC_TBCOMBO_WIDTH,
		GetModuleHandle(NULL), 0);

// ������������� ������ ��� hwndCombo
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"�� ����");
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"������");
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"�������");
	SendMessage(hwndComboWidth, CB_ADDSTRING, 0, (LPARAM)"�������");
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
   static bool flagMiddlePress=false;
   int prevMode;
   RECT rect;
   char str[30];
   char str1[30];
   static char buf[50];
   float aspect;
   int wheel_number;
   short ticks=0;
   int direction=0;
   static int scaleFactor=10;
   RECT rectStr={100,10,300,30};
	
   HPEN hPen;
   int ItemIndex;

   switch(message)                // Process selected messages
   {  
      case WM_CREATE:
            if (hWnd==modelWindow.getHWND()){
		hDC=GetDC(hWnd);

		/* SetGraphicsMode(hDC,GM_ADVANCED);
		SetMapMode(hDC,	MM_ISOTROPIC);

		SetViewportExtEx(hDC, modelWindow.getWidth(),
					modelWindow.getHeight(),
					NULL);
                aspect=modelWindow.getWidth()/modelWindow.getHeight();
		SetWindowExtEx(hDC, 3000, 3000/aspect, NULL);
		
		ReleaseDC(hWnd,hDC);
		*/
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
		   if (flagMiddlePress==true)
		     {
		   	GetCursorPos(lpPoint);
		   	ScreenToClient(hWnd,lpPoint);
		   	DPtoLP(modelWindow.getHDC(),lpPoint,1);		   
		   	float dx=prevCursPos.x-lpPoint->x;
		   	float dy=prevCursPos.y-lpPoint->y;
		   	modelWindow.setWOrg(origin.x,origin.y);
		   	origin.x-=dx;
		   	origin.y-=dy;
		   	prevCursPos=*lpPoint;
		   	modelWindow.setWOrg(origin.x,origin.y);
		     }
		   else {
		   GetCursorPos(lpPoint);
		   ScreenToClient(hWnd,lpPoint);
		   DPtoLP(modelWindow.getHDC(),lpPoint,1);
		   sprintf(buffer,"x=%3d y=%3d ",lpPoint->x,lpPoint->y);
		   strcat(buffer,buf);
		   aRect=statusBar.getRect();
		   InvalidateRect(statusBar.getHWND(),&aRect,TRUE);
		   if (comStr.getState()==2){
			modelWindow.line(comStr.getStartPOINT().x,comStr.getStartPOINT().y,prevCursPos.x,prevCursPos.y);
			modelWindow.line(comStr.getStartPOINT().x,comStr.getStartPOINT().y,lpPoint->x,lpPoint->y);
		        prevCursPos=*lpPoint;
		   }
		   }
		}

		break;
	
	case WM_LBUTTONUP:

		break; 


	case WM_LBUTTONDOWN:
		SetFocus(modelWindow.getHWND());
		if (hWnd==modelWindow.getHWND()){

		  GetCursorPos(lpPoint);
		  ScreenToClient(hWnd,lpPoint);
		  DPtoLP(modelWindow.getHDC(),lpPoint,1);
		  switch (comStr.getState()){
			case 1:
				comStr.segLine(lpPoint->x,lpPoint->y);
			        prevCursPos=*lpPoint;
				break;
			case 2:
				comStr.segLine(lpPoint->x,lpPoint->y);
				break;
			case 3:
				comStr.segArc(lpPoint->x,lpPoint->y);
				break;
			case 4:
				comStr.segArc(lpPoint->x,lpPoint->y);
				break;
			case 5:
				comStr.segArc(lpPoint->x,lpPoint->y);
				break;

		  }
		}
		break;

	case WM_RBUTTONDOWN:

		break;

    case WM_MBUTTONDOWN:
		if (flagMiddlePress==false&&hWnd==modelWindow.getHWND()){
		   flagMiddlePress=true;

		   	GetCursorPos(lpPoint);
		   	ScreenToClient(hWnd,lpPoint);
		   	DPtoLP(modelWindow.getHDC(),lpPoint,1);		   
		   	prevCursPos=*lpPoint;

		}
	break;

    case WM_MBUTTONUP:
		if (flagMiddlePress==true){
		   flagMiddlePress=false;
		}

	break;

    case 0x020a:
        /* Should be WM_MOUSEWHEEL but my compiler doesn't recognize it */
    
        /*
         * XXX THIS IS SPECULATIVE -- John Fay, 10/2/03
         * XXX Should use WHEEL_DELTA instead of 120
         */
        wheel_number = LOWORD( wParam );
        ticks = ( short )HIWORD( wParam ) / 120;
        direction = 1;

        if( ticks < 0 )
        {
            direction = -1;
            ticks = -ticks;
        }

      scaleFactor-=ticks*direction;

      if (scaleFactor<1) 
		scaleFactor=1;

      modelWindow.setScale(scaleFactor); 

      sprintf(buffer,"x=%3d y=%3d     Scale:%f",lpPoint->x,lpPoint->y,scaleFactor);

      sprintf(buf,"     Scale:%3d",scaleFactor);

      statusBar.printText(10,10,buffer);
      aRect=statusBar.getRect();
      InvalidateRect(statusBar.getHWND(),&aRect,TRUE);

      break;

      case WM_PAINT:                       // Message is to redraw the window
		//statusBar.printText(10,10,buffer);
		hDC = BeginPaint(hWnd, &PaintSt);
		if (hWnd==statusBar.getHWND()){
  		    SetBkMode(hDC, TRANSPARENT);      // Set text background mode
  		    TextOut(hDC,10,10,buffer,strlen(buffer));
		}
	        EndPaint(hWnd, &PaintSt); // Terminate window redraw operation
		if (hWnd==modelWindow.getHWND())
		   TransformAndDraw(mode,modelWindow.getHWND(),scaleFactor,&origin);		
		break;

      case WM_COMMAND:
	 switch (LOWORD(wParam))
	 {
//----------------- STANTARD-------------------------------------------------
		case IDM_OPEN:
			MessageBox(hWnd, "������ ����� 'Open'", "���� File", MB_OK);
			break;

		case IDM_NEW:
			MessageBox(hWnd, "������ ����� 'New'", "���� File", MB_OK);
			break;

 		case IDM_SAVE:
			MessageBox(hWnd, "������ ����� 'Save'", "���� File", MB_OK);
			break;
		case IDM_EXIT:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
//-----------------DRAW---------------------------------------------------
		case IDM_LINE:
			//modelWindow.moveTo(10,10);
			comStr.segLine(0,0);
			//MessageBox(hWnd, "������ ����� '�������'", "���� ��������", MB_OK);
			break;
		case IDM_ARCSTARTENDRADIUS:
			comStr.segArc(0,0);
			//SendMessage(hwndCommand, WM_SETTEXT, 0, (LPARAM) "������� �������");
			//modelWindow.lineTo(150,150);
			//MessageBox(hWnd, "������ ����� '���� ���������, �������� �����, ������, �����������'", "���� ��������", MB_OK);
			break;
		case IDM_ARC3POINTS:
			MessageBox(hWnd, "������ ����� '���� �� 3 ������'", "���� ��������", MB_OK);
			break;
		case IDM_ARCCENTREANG1ANG2:
			MessageBox(hWnd, "������ ����� '���� ����� ������ ����1 ����2'", "���� ��������", MB_OK);
			break;

		case IDM_CIRCCENTRRAD:
			MessageBox(hWnd, "������ ����� '���������� ����� ������'", "���� ��������", MB_OK);
			break;
		case IDM_CIRCCENTRDIAM:
			MessageBox(hWnd, "������ ����� '���������� ����� �������'", "���� ��������", MB_OK);
			break;
		case IDM_CIRC3POINTS:
			MessageBox(hWnd, "������ ����� '���������� �� 3 ������'", "���� ��������", MB_OK);
			break;

		case IDM_CONTUR:
			MessageBox(hWnd, "������ ����� '������'", "���� ��������", MB_OK);
			break;
		case IDM_RECTANGLE:
			MessageBox(hWnd, "������ ����� '�������������'", "���� ��������", MB_OK);
			break;
		case IDM_POLYGON:
			MessageBox(hWnd, "������ ����� '�������'", "���� ��������", MB_OK);
			break;
		case IDM_TEXT:
			MessageBox(hWnd, "������ ����� '�����'", "���� ��������", MB_OK);
			break;

//-----------------MODIFY-------------------------------------------------
		case IDM_ERASE:
			statusBar.printText(10,10,"������ ���������. ������� ������� ��������.");
			//MessageBox(hWnd, "������ ����� 'Erase'", "���� �������������", MB_OK);
			break;

		case IDM_PCOPY:
			MessageBox(hWnd, "������ ����� 'PCOPY'", "���� �������������", MB_OK);
			break;

		case IDM_MIRROR:
			MessageBox(hWnd, "������ ����� 'MIRROR'", "���� �������������", MB_OK);
			break;

		case IDM_OFFSET:
			MessageBox(hWnd, "������ ����� 'OFFSET'", "���� �������������", MB_OK);
			break;

		case IDM_ARRAY:
			MessageBox(hWnd, "������ ����� 'ARRAY'", "���� �������������", MB_OK);
			break;

		case IDM_MOVE:
			MessageBox(hWnd, "������ ����� 'MOVE'", "���� �������������", MB_OK);
			break;

		case IDM_ROTATE:
			MessageBox(hWnd, "������ ����� 'ROTATE'", "���� �������������", MB_OK);
			break;

		case IDM_SCALE:
			MessageBox(hWnd, "������ ����� 'SCALE'", "���� �������������", MB_OK);
			break;

		case IDM_TRIM:
			MessageBox(hWnd, "������ ����� '��������'", "���� �������������", MB_OK);
			break;

		case IDM_LENGTH:
			MessageBox(hWnd, "������ ����� '��������'", "���� �������������", MB_OK);
			break;

		case IDM_CHAMFER:
			MessageBox(hWnd, "������ ����� '�����'", "���� �������������", MB_OK);
			break;

		case IDM_FILLET:
			//MessageBox(hWnd, "������ ����� '����������'", "���� �������������", MB_OK);
			modelWindow.printText(10,10,"���� ������. ������� ����������");
			break;
		case IDC_TBCOMBO_LAYER:
			if (HIWORD(wParam)==CBN_SELCHANGE){
                           ItemIndex = SendMessage((HWND) lParam, (UINT) CB_GETCURSEL, 
                               (WPARAM) 0, (LPARAM) 0);
			   if (ItemIndex<Layer::getCount()){
			       currentLayer=&layer[ItemIndex];
			       hPen=CreatePen(currentLayer->getType(), currentLayer->getWidth(), RGB((currentLayer->getColor()&0xff0000)>>16, (currentLayer->getColor()&0xff00)>>8,currentLayer->getColor()&0xff));
               		       modelWindow.setPen(&hPen);
			   }
			}
				
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