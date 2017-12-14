#include "headerui.h"
#include <objbase.h>
#include "model.h"
#include "math.h"
#include <stdio.h>

UINT CHANGEWINDOWPROC=NULL;
extern BaseWindow mainWindow;
extern ModelWindow modelWindow;
extern Model myModel; 

CommandLine::CommandLine():BaseWindow()
{
	pStrCmd="Command";
	EnType=tLine;
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
	char *ptrChar=buffer;
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
		else if(!strcmp(buffer,"contur")){
		       contur(0,0);
		     }
		else {
		  strcat(buffer,"-unknown command");
		  addTextToHistory(buffer);
	        }
	  break;
	  case STATE_LINE_POINT1:
	  	ptrChar=strtok(ptrChar,",");
		if (ptrChar!=NULL)
			x1=atof(ptrChar);
		ptrChar=NULL;
	  	ptrChar=strtok(ptrChar,"\0");
		if (ptrChar!=NULL)
			y1=atof(ptrChar);
		segLine(x1,y1);
	  break;
	  case STATE_LINE_POINT2:
	  	ptrChar=strtok(ptrChar,",");
		if (ptrChar!=NULL)
			x2=atof(ptrChar);
		ptrChar=NULL;
	  	ptrChar=strtok(ptrChar,"\0");
		if (ptrChar!=NULL)
			y2=atof(ptrChar);
		segLine(x2,y2);
		myModel.showModel();
	  break;
	  case STATE_ARC_POINT1:
	  	ptrChar=strtok(ptrChar,",");
		if (ptrChar!=NULL)
			x1=atof(ptrChar);
		ptrChar=NULL;
	  	ptrChar=strtok(ptrChar,"\0");
		if (ptrChar!=NULL)
			y1=atof(ptrChar);
		segArc(x1,y1);
	  break;
	  case STATE_ARC_POINT2:
	  	ptrChar=strtok(ptrChar,",");
		if (ptrChar!=NULL)
			x2=atof(ptrChar);
		ptrChar=NULL;
	  	ptrChar=strtok(ptrChar,"\0");
		if (ptrChar!=NULL)
			y2=atof(ptrChar);
		segArc(x2,y2);
	  break;
	  case STATE_ARC_POINT3:
	  	ptrChar=strtok(ptrChar,",");
		if (ptrChar!=NULL)
			x3=atof(ptrChar);
		ptrChar=NULL;
	  	ptrChar=strtok(ptrChar,"\0");
		if (ptrChar!=NULL)
			y3=atof(ptrChar);
		segArc(x3,y3);
		myModel.showModel();
	  break;
	}
	
	SetWindowText(hwndC,"");

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
int  CommandLine::addTextToHistory(const char * pStr){
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

int CommandLine::setEnType(int entype){
	EnType=entype;
	return EnType;
}

int CommandLine::getEnType(){
	return EnType;
}
int CommandLine::enumEnType(){
	if (EnType==tLine)
		EnType=tArc;
	else
		EnType=tLine;
}

bool CommandLine::segLine(float x,float y){
	bool result;
	RECT aRect={0,40,180,80}; 	

	switch (state){
		case STATE_WAIT_COMMAND:
           pStrCmd="координаты x,y: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   addTextToHistory("Line");
	   modelWindow.setROP2(R2_NOTXORPEN);
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
            pStrCmd="команда: ";
	    InvalidateRect(hWnd,&aRect,TRUE);	
	    modelWindow.setROP2(R2_COPYPEN);
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
           pStrCmd="координаты x,y: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   addTextToHistory("Arc 3 points");
	   modelWindow.setROP2(R2_NOTXORPEN);
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
	    modelWindow.setROP2(R2_COPYPEN);
            pStrCmd="команда: ";
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

bool CommandLine::contur(float x, float y)
{
	bool result;
	RECT aRect={0,40,180,80}; 	
        char str[256];
	Point start,end;
	switch (state){
		case STATE_WAIT_COMMAND:
           pStrCmd="координаты x,y: ";
	   InvalidateRect(hWnd,&aRect,TRUE);
	   switch(EnType){
		   case tLine:
	   		addTextToHistory("Line");
	   		state=STATE_CONTUR_LINE1;
		   break;
		   case tArc:
	   		addTextToHistory("Arc");
	   		state=STATE_CONTUR_ARC1;
		   break;
	   }
	   modelWindow.setROP2(R2_NOTXORPEN);
	  break;
	  	case STATE_CONTUR_LINE1:
	  	x1=x;
	    	y1=y;
	    	addCoordToHistory(x,y,1);
		if (EnType==tLine)
	    		state=STATE_CONTUR_LINE2;
		else 
	    		state=STATE_CONTUR_ARC2;
	  break;

		case STATE_CONTUR_LINE2:
		if (EnType==tLine){
	    		start.setXY(x1,y1);
	    		end.setXY(x,y);
	    		myModel.appendLine(&start,&end);
	    		addCoordToHistory(x,y,2);
            		pStrCmd="команда: ";
	    		InvalidateRect(hWnd,&aRect,TRUE);	
			x1=x;
			y1=y;
	    		state=STATE_CONTUR_LINE2;
		}
		else {
	    		state=STATE_CONTUR_ARC2;
			contur(x,y);
		}

		break;

	  	case STATE_CONTUR_ARC1:
	  	x1=x;
	    	y1=y;
	    	addCoordToHistory(x,y,1);
		if (EnType==tLine)
	    		state=STATE_CONTUR_LINE2;
		else 
	    		state=STATE_CONTUR_ARC2;
	  	break;

	  	case STATE_CONTUR_ARC2:
		if (EnType==tArc){
	  	x2=x;
	    	y2=y;
	    	addCoordToHistory(x,y,2);
	    	state=STATE_CONTUR_ARC3;
		}
		else{
			state=STATE_CONTUR_LINE2;
			contur(x,y);
		}
      		break;
	  	case STATE_CONTUR_ARC3:
		if (EnType==tArc){
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

	    addTextToHistory(str);
            pStrCmd="команда: ";
	    InvalidateRect(hWnd,&aRect,TRUE);	
		x1=x3;
		y1=y3;
			state=STATE_CONTUR_ARC2;
		}
		else{
			state=STATE_CONTUR_LINE2;
			contur(x,y);
		}
	  break;
	  
	}
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

int CommandLine::getRC(float xd, float yd)
{
//            
//            xxx
//          x     B             A(xa,ya)
//         x       x            B(xb,yb)
//         A_______D            D(xd,yd) 
//          x     x             R - radius
//            xxx               C(xc,yc) - center
//
//

	x3=xd;
	y3=yd;
        float xa=x1, ya=y1;
 	float xb=x2, yb=y2;
	float e,Kab;
	if (xa==xb && ya==yb)   // A = B
	{
		return -1;
	}

	if (xa==xd && ya==yd)   // A = D
	{
		return -2;
	}

	if (xb==xd && yb==yd)   // B = D
	{
		return -3;
	}

	if (ya==yd) // AD || OX
	{
		xc=(xa+xd)/2;
		yc=((xa-xc)*(xa-xc)-(xb-xc)*(xb-xc)+yd*yd-yb*yb)/(2*(yd-yb));
		R=sqrt((xd-xc)*(xd-xc)+(yd-yc)*(yd-yc));
	}
	else if (xa==xd) // AD || OY
		if (xb!=xd)
		{
			yc=(ya+yd)/2;
			xc=(3*xa*xa-4*xb*xb-2*xa*xd+xd*xd+ya*ya-2*ya*yd+yd*yd-(ya-yd-2*yb)*(ya+yd-2*yb))/(8*(xa-xb));
			R=sqrt((xc-xa)*(xc-xa)+(ya-yb)*(ya-yb));
		}
		else 
		{
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
			if (xb==xa)
			{
				xc=(xb*xb+yb*yb-yd*yd-xd*xd+2*yc*(yd-yb))/(2*(xb-xd));
			}
			else 
			{   
				xc=(xb*xb+yb*yb-ya*ya-xa*xa+2*yc*(ya-yb))/(2*(xb-xa));
			}
			}
	R=sqrt((xa-xc)*(xa-xc)+(ya-yc)*(ya-yc));
		}
	float xap=xa-xc;
	float yap=ya-yc;
	float xbp=xb-xc;
	float ybp=yb-yc;
	float xdp=xd-xc;
	float ydp=yd-yc;

	float fia,fib,fid;

	if (xap==0){
		if (yap>0)
			fia=PI/2;
		else 
			fia=-PI/2;
	}
	else
	{
		fia=atan(yap/xap);
		if (xap<0)
			fia+=PI;
		else if (xap>0&&yap<0)
			fia+=2*PI;
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
		if (xbp<0)
			fib+=PI;
		else if (xbp>0&&ybp<0)
			fib+=2*PI;
		
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
		if (xdp<0)
			fid+=PI;
		else if (xdp>0&&ydp<0)
			fid+=2*PI;
	}

	if (fia>fid)
		if (fib>fid&&fib<fia)
		ArcDirection=AD_COUNTERCLOCKWISE;
		else
		ArcDirection=AD_CLOCKWISE;
	else 
		if (fib>fia&&fib<fid)
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
