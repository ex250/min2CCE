#include "headerui.h"
#include <objbase.h>
#include "model.h"
#include "math.h"

UINT CHANGEWINDOWPROC=NULL;
extern BaseWindow mainWindow;

extern Model myModel; //внешняя переменная

CommandLine::CommandLine():BaseWindow()
{
	pStrCmd="Command";

/*	sbInfo.cbSize=sizeof(SCROLLINFO);
	sbInfo.fMask =SIF_POS;
	sbInfo.nMin=0;
	sbInfo.nMax=1000;
	sbInfo.nPage=2;
	sbInfo.nPos=1;
*/
}

WNDPROC CommandLine::wpOrigEditProc=0;

bool CommandLine::init(LPCTSTR windowName,
			DWORD dwStyle,
			int x, int y,
			int cx,int cy,
			HWND hwndParent,
			HMENU hMenu,
			HINSTANCE hInst)
{

// определение класса окна командной строки
WNDCLASS CommandWindow= {
      CS_HREDRAW | CS_VREDRAW,
      CommandLine::cmdProc,
      0,          // No extra bytes after the window class
      0,          // structure or the window instance
      NULL,   // Application instance handle
      LoadIcon(0, IDI_APPLICATION),
      LoadCursor(0, IDC_ARROW),
      static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
      0,           // No menu, so no menu resource name
      "CommandLine"  // Set class name
};

   // регистрируем класс окна
   RegisterClass(&CommandWindow);

   // создаем главное окно

   hWnd = CreateWindow(
          CommandWindow.lpszClassName,    // the window class name
          windowName,   		// The window title
          dwStyle,          // Window style as overlapped
          x,         // Default screen position of upper left
          y,         // corner of our window as x,y...
          cx,         // Default window size
          cy,         // .... 
          hwndParent,                     // No parent window
          hMenu,                     // No menu
          hInst,	         // Program Instance handle
          this                      // No window creation data
        );

   // создаем элемент управления EDIT

   hwndH=CreateWindow("LISTBOX",
		NULL,
		WS_CHILD|WS_VISIBLE|WS_VSCROLL,
		10,1,
		cx-10,
		cy-29,
		hWnd,
		NULL,
		GetModuleHandle(NULL),
		0);



   hwndC=CreateWindow("EDIT",
		NULL,
		WS_CHILD|WS_VISIBLE|ES_LEFT,
		120,
		cy-29,
		cx-120,
		25,
		hWnd,
		NULL,
		GetModuleHandle(NULL),
		0);

   wpOrigEditProc=(WNDPROC)SetWindowLong(hwndC,
				GWL_WNDPROC,
				(LONG)subProc);

   SendMessage(hwndC,WM_USER+1,(WPARAM)this,NULL);

   SetFocus(hwndC);

   return TRUE;	
}


int CommandLine::getText(LPTSTR buffer, int maxChar){
   return GetWindowText(hwndC,buffer,maxChar);
}

bool CommandLine::addCommand()
{

	RECT aRect;
	aRect.left=0;
	aRect.top =40;
	aRect.right=180;
	aRect.bottom=80;

	bool result;
	result=GetWindowText(hwndC,buffer,50);
	switch(state){
          case STATE_WAIT_COMMAND:
		if (!strcmp(buffer,"line")){
		segLine(0,0);
        	}
		else if(!strcmp(buffer,"arc")){
		       segArc(0,0);
		     }
		else {
		  strcat(buffer,"-unknown command");
		  SendMessage(hwndH,(UINT) LB_ADDSTRING,(WPARAM) 0,(LPARAM) &buffer);
        	  SendMessage(hwndH,WM_VSCROLL,SB_BOTTOM,NULL);
	          InvalidateRect(hWnd,&aRect,TRUE);
	        }
	  break;
	  case STATE_LINE_POINT1:
	        //анализ buffer для получения координат
		//вызов метода parse();
		//вызов метода segLine(x,y)
	  break;
	}
	
	SetWindowText(hwndC,"");

	//SendMessage(hwndH,(UINT) LB_ADDSTRING,(WPARAM) 0,(LPARAM) &buffer);
        //SendMessage(hwndH,WM_VSCROLL,SB_BOTTOM,NULL);
        //InvalidateRect(hWnd,&aRect,TRUE);
 	return result;
}

int CommandLine::getState(){
	return state;
}

int CommandLine::setState(int st){
	state=st;
	return state;
}

bool CommandLine::segLine(float x,float y){
	bool result;
	RECT aRect={0,40,180,80}; 	
        char str[30];

	switch (state){
		case STATE_WAIT_COMMAND:
           pStrCmd="координаты x,y: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   SendMessage(hwndH,(UINT) LB_ADDSTRING,(WPARAM) 0,(LPARAM) "Отрезок");
           SendMessage(hwndH,WM_VSCROLL,SB_BOTTOM,NULL);
	   state=STATE_LINE_POINT1;
	  break;

		case STATE_LINE_POINT1:
	    state=STATE_LINE_POINT2;
	    x1=x;
	    y1=y;
	  break;

		case STATE_LINE_POINT2:
	    Point start(x1,y1);
	    Point end(x,y);
	    myModel.appendLine(&start,&end);
	    state=STATE_WAIT_COMMAND;
            pStrCmd="команда: ";
	    InvalidateRect(hWnd,&aRect,TRUE);	
            //sprintf(str,"x1=%d y1=%d x2=%d y2=%d",x1,y1,x2,y2);
	    //MessageBox(hWnd,str,"Координаты линии",MB_OK);
	  break;
	}
	result = true; //myModel.appendLine();
	return result;
}

bool CommandLine::segArc(float x,float y){
	bool result;
	RECT aRect={0,40,180,80}; 	
        char str[30];
	float Radius;
	switch (state){
		case STATE_WAIT_COMMAND:
           pStrCmd="координаты x,y: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   SendMessage(hwndH,(UINT) LB_ADDSTRING,(WPARAM) 0,(LPARAM) "Дуга 2 точки, радиус");
           SendMessage(hwndH,WM_VSCROLL,SB_BOTTOM,NULL);
	   state=STATE_ARC_POINT1;
	  break;

		case STATE_ARC_POINT1:
	    x1=x;
	    y1=y;
	    state=STATE_ARC_POINT2;
	  break;

		case STATE_ARC_POINT2:
	   //Point p1(x1,y1);
	   //Point p2(x,y);
           pStrCmd="радиус R: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   state=STATE_ARC_POINT3;
	  break;

		case STATE_ARC_POINT3:
	    Radius=4;//вычислить радиус
	    //myModel.appendArc(&start,&end,&pt3, Radius);
            pStrCmd="команда: ";
	    InvalidateRect(hWnd,&aRect,TRUE);	
	    state=STATE_WAIT_COMMAND;
	  break;
	}
	result = true; //myModel.appendLine();
	return result;
}


POINT CommandLine::getStartPOINT(){
	//char str[30];
	//sprintf(str,"state=%d",state);
	//MessageBox(hWnd,str,"counter",MB_OK);
	POINT start={x1,y1};
	return start;
}

//**********************************************************************

LRESULT CALLBACK CommandLine::cmdProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    static HWND m_hWnd;
    HDC hDC;
    PAINTSTRUCT ps;
    RECT aRect;
    

    if (uMsg == (WM_CREATE))
    {
        m_hWnd=hWnd;

	LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;

        CommandLine *pCmd = (CommandLine *)pcs->lpCreateParams;

        SetWindowLong(
            hWnd,
            GWL_USERDATA,
            PtrToUlong(pCmd)
            );
        
        result = 1;
    }
    else
    {
        CommandLine *pCmd = reinterpret_cast<CommandLine *>(static_cast<LONG_PTR>(
            GetWindowLong(
                hWnd,
                GWL_USERDATA
                )));

        bool wasHandled = false;

	
        if (pCmd)
        {
            switch (uMsg)
            {   

	        case WM_KEYDOWN:
			
		break;

	        case WM_KEYUP:

		break;

        	case WM_CHAR:

		break;

                case WM_COMMAND:

                break;

                case WM_DISPLAYCHANGE:
                {
                    InvalidateRect(hWnd, NULL, FALSE);
                }
                wasHandled = true;
                result = 0;
                break;

            case WM_DESTROY:
                {
                    PostQuitMessage(0);
                }

                wasHandled = true;
                result = 1;
                break;

	    case WM_PAINT:
		hDC=BeginPaint(hWnd,&ps);
		TextOut(hDC,10,40,pCmd->pStrCmd,strlen(pCmd->pStrCmd));
		EndPaint(hWnd,&ps);
		pCmd->line(0,(INT)(pCmd->getHeight()/2),(INT)(pCmd->getWidth()),(INT)(pCmd->getHeight()/2));
		break;

	    case WM_SIZE:
		SetWindowPos(pCmd->hwndH,HWND_TOP,10,1,pCmd->getWidth()-10,pCmd->getHeight()-29,SWP_SHOWWINDOW);
		break;
            }
        }

        if (!wasHandled)
        {
           result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    return result;
}


LRESULT CALLBACK CommandLine::subProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    static HWND m_hWnd;
    HDC hDC;
    PAINTSTRUCT ps;
    RECT aRect;
    

    if (uMsg == WM_USER+1)
    {
        m_hWnd=hWnd;
//	MessageBox(hWnd, "IT'S WORK", "Tёх ЁрсюЄрхЄ", MB_OK); 

        CommandLine *pCmd = (CommandLine *)wParam;

        SetWindowLong(
            hWnd,
            GWL_USERDATA,
            PtrToUlong(pCmd)
            );
        
        result = 1;
    }
    else
    {
        CommandLine *pCmd = reinterpret_cast<CommandLine *>(static_cast<LONG_PTR>(
            GetWindowLong(
                hWnd,
                GWL_USERDATA
                )));

        bool wasHandled = false;

	
        if (pCmd)
        {
            switch (uMsg)
            {   
		case WM_GETDLGCODE: 
               		result = DLGC_WANTALLKEYS;
	                wasHandled = TRUE;
			break;

	        case WM_KEYDOWN:
	                //wasHandled = TRUE;
        	        result = 0;

		break;

	        case WM_KEYUP:
		        //wasHandled = TRUE;				
        	        result = 0;

		break;

        	case WM_CHAR:
		        if (wParam==VK_RETURN){
	            		wasHandled=TRUE;
				pCmd->addCommand();
			}
 		        result = 0;

		break;


            }
        }

        if (!wasHandled)
        {
		return CallWindowProc(wpOrigEditProc, hWnd, uMsg, 
        		wParam, lParam); 
        }
    }

    return result;
}
