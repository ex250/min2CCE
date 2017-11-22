#include "headerui.h"
#include <objbase.h>
#include "model.h"
#include "math.h"
#include <stdio.h>

UINT CHANGEWINDOWPROC=NULL;
extern BaseWindow mainWindow;

extern Model myModel; 

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

// ����������� ������ ���� ��������� ������
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

   // ������������ ����� ����
   RegisterClass(&CommandWindow);

   // ������� ������� ����

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

   // ������� ������� ���������� EDIT

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
		  addTextToHistory(buffer);
	        }
	  break;
	  case STATE_LINE_POINT1:
	        //������ buffer ��� ��������� ���������
		//����� ������ parse();
		//����� ������ segLine(x,y)
	  break;
	}
	
	SetWindowText(hwndC,"");

	//SendMessage(hwndH,(UINT) LB_ADDSTRING,(WPARAM) 0,(LPARAM) &buffer);
        //SendMessage(hwndH,WM_VSCROLL,SB_BOTTOM,NULL);
        //InvalidateRect(hWnd,&aRect,TRUE);
 	return result;
}

int  CommandLine::addCoordToHistory(float x,float y, int index)
{
	   char str[256];
	   sprintf(str,"x%d=%3.2f,y%d=%3.2f",index,x,index,y);
	   SendMessage(hwndH,(UINT) LB_ADDSTRING,(WPARAM) 0,(LPARAM) str);
           SendMessage(hwndH,WM_VSCROLL,SB_BOTTOM,NULL);
	   return strlen(str);
}
int  CommandLine::addTextToHistory(char * pStr){
	SendMessage(hwndH,(UINT) LB_ADDSTRING,(WPARAM) 0,(LPARAM) pStr);
        SendMessage(hwndH,WM_VSCROLL,SB_BOTTOM,NULL);
	return strlen(pStr);
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

	switch (state){
		case STATE_WAIT_COMMAND:
           pStrCmd="���������� x,y: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   addTextToHistory("Line");
	   state=STATE_LINE_POINT1;
	  break;

		case STATE_LINE_POINT1:
	    x1=x;
	    y1=y;
	    addCoordToHistory(x,y,1);
	    state=STATE_LINE_POINT2;
	  break;

		case STATE_LINE_POINT2:
	    Point start(x1,y1);
	    Point end(x,y);
	    myModel.appendLine(&start,&end);
	    addCoordToHistory(x,y,2);
            pStrCmd="�������: ";
	    InvalidateRect(hWnd,&aRect,TRUE);	
	    state=STATE_WAIT_COMMAND;
	  break;
	}
	result = true; //myModel.appendLine();
	return result;
}

bool CommandLine::segArc(float x,float y){
	bool result;
	RECT aRect={0,40,180,80}; 	
        char str[256];
	switch (state){
		case STATE_WAIT_COMMAND:
           pStrCmd="���������� x,y: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   addTextToHistory("Arc 3 points");
	   state=STATE_ARC_POINT1;
	  break;

		case STATE_ARC_POINT1:
	    x1=x;
	    y1=y;
	    addCoordToHistory(x,y,1);
	    state=STATE_ARC_POINT2;
	  break;

		case STATE_ARC_POINT2:
	   x2=x;
	   y2=y;
	   addCoordToHistory(x,y,2);
	   state=STATE_ARC_POINT3;
	  break;

		case STATE_ARC_POINT3:
	   x3=x;
	   y3=y;	
	   addCoordToHistory(x,y,3);
	    if (!getRC(x,y))
		    myModel.appendArc(x1,y1,x3,y3,xc,yc,R,ArcDirection);

            sprintf(str,"R=%4.2f, Xc=%4.2f, Yc=%4.2f",R,xc,yc);
	    
	    if (ArcDirection==AD_COUNTERCLOCKWISE)
		    strcat(str," rotation: CCW");
	    else
		    strcat(str," rotation: CW");

	    //MessageBox(hWnd,str,"Arc parameters",MB_OK);
	    addTextToHistory(str);

            pStrCmd="�������: ";
	    InvalidateRect(hWnd,&aRect,TRUE);	
	    state=STATE_WAIT_COMMAND;
	  break;
		default:
            pStrCmd="Do not known STATE: ";
	    InvalidateRect(hWnd,&aRect,TRUE);	
	    state=STATE_WAIT_COMMAND;

	}
	result = true; 
	return result;
}


POINT CommandLine::getStartPOINT(){
	POINT start={x1,y1};
	return start;
}

float CommandLine::getX1()
{ return x1;}
float CommandLine::getY1()
{ return y1;}
float CommandLine::getX2()
{ return x2;}
float CommandLine::getY2()
{ return y2;}
float CommandLine::getX3()
{ return x3;}
float CommandLine::getY3()
{ return y3;}
float CommandLine::getXc()
{ return xc;}
float CommandLine::getYc()
{ return yc;}
float CommandLine::getR()
{ return R;}
int CommandLine::getDirection()
{ return ArcDirection;}
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
//	MessageBox(hWnd, "IT'S WORK", "T�� ��������", MB_OK); 

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

int CommandLine::getRC(float xd, float yd)
{

//            
//            ...
//          .     b             A(xa,ya)
//         .       .            B(xb,yb)
//         a_______d            D(xd,yd) 
//          .     .             R - radius
//            ...               C(xc,yc) - center
//
//
        float xa=x1, ya=y1;
 	float xb=x2, yb=y2;

	float e,Kab;
	if (xa==xb && ya==yb)   // A = B
	{
		//cout<<"A = B"<<endl;
		return -1;
	}

	if (xa==xd && ya==yd)   // A = D
	{
		//cout<<"D = A)"<<endl;
		return -2;
	}

	if (xb==xd && yb==yd)   // B = D
	{
		//cout<<"D = B"<<endl;
		return -3;
	}

	if (ya==yd) // AD || OX
	{
		xc=(xa+xd)/2;
		yc=((xa-xc)*(xa-xc)-(xb-xc)*(xb-xc)+yd*yd-yb*yb)/(2*(yd-yb));
		R=sqrt((xd-xc)*(xd-xc)+(yd-yc)*(yd-yc));
		return 0;
	}
	else if (xa==xd) // AD || OY
		if (xb!=xd)
		{
			yc=(ya+yd)/2;
			if (xb>xa)
			xc=(3*xa*xa-4*xb*xb-2*xa*xd+xd*xd+ya*ya-2*ya*yd+yd*yd-(ya-yd-2*yb)*(ya+yd-2*yb))/(8*(xa-xb));
			else
				xc=(xd*xd-xb*xb+(yc-yd)*(yc-yd)-(yc-yb)*(yc-yb))/(2*(xd-xb));
			R=sqrt((xc-xa)*(xc-xa)+(ya-yb)*(ya-yb));
			return 0;
		}
		else 
		{
			//cout<<"B na AD"<<endl;
			return -1;
		}
	else 
	{
		Kab=(ya-yb)/(xa-xb);
		e=ya-Kab*xa;
		if ((yd-Kab*xd-e)==0)
			return -1;
		else 
		{
			yc=((xb-xd)*(xb*xb+yb*yb-xa*xa-ya*ya)-(xb-xa)*(xb*xb+yb*yb-xd*xd-yd*yd))/(2*((yd-yb)*(xb-xa)-(ya-yb)*(xb-xd)));
			if (xb!=xd)
			{
				xc=(xb*xb+yb*yb-yd*yd-xd*xd+2*yc*(yd-yb))/(2*(xb-xd));
			}
			else 
			{       if (xb!=xa)
				xc=(xb*xb+yb*yb-ya*ya-xa*xa+2*yc*(ya-yb))/(2*(xb-xa));
			}
		}
	R=sqrt((xa-xc)*(xa-xc)+(ya-yc)*(ya-yc));
	}

	// transfer to polar SC 
	// test CW or CCW
	float xap=xa-xc;
	float yap=ya-yc;
	float xbp=xb-xc;
	float ybp=yb-yc;
	float xdp=xd-xc;
	float ydp=yd-yc;


	if (xap==0){
		if (yap>0)
			fia=PI/2;
		else 
			fia=-PI/2;
	}
	else
	{
		fia=atan(yap/xap);
		if (yap<0)
			fia+=PI;
	}

	if (xbp==0){
		if (ybp>0)
			fib=PI/2;
		else 
			fib=-PI/2;
	}
	else
	{
		fib=atan(ybp/xbp);
		if (ybp<0)
			fib+=PI;
	}

	if (xdp==0){
		if (ydp>0)
			fid=PI/2;
		else 
			fid=-PI/2;
	}
	else
	{
		fid=atan(ydp/xdp);
		if (ydp<0)
			fid+=PI;
	}

	if (fia>fid)
		ArcDirection=AD_CLOCKWISE;
	else
		ArcDirection=AD_COUNTERCLOCKWISE;
	x4=xdp;
	y4=ydp;
	x5=xap;
	y5=yap;
	x6=xbp;
	y6=ybp;

return 0;	
}
