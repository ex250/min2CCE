#include "headerui.h"

toolbarDraw::toolbarDraw():BaseWindow(),ImageListID(0)
{
	dwStyle=WS_CHILD|WS_BORDER;
	cx=cy=16;
	flag=ILC_MASK|ILC_COLOR16;
	cGrow=(int)NULL;
}

void toolbarDraw::setStyle(DWORD style)
{
	dwStyle=style;
}

void toolbarDraw::setButtonSize(int sx,int sy)
{
	cx=sx;
	cy=sy;
}


int toolbarDraw::init(  HWND hWndParent,
			LPCTSTR windowName,
			const int cInitial,
			HBITMAP hBmpToolbar,
			TBBUTTON *tbButtons
			)
{
   

   hWnd = CreateWindow(
          TOOLBARCLASSNAME,    // the window class name
          windowName,   		// The window title
          dwStyle,          // Window style as overlapped
          xPos,         // Default screen position of upper left
          yPos,         // corner of our window as x,y...
          width,         // Default window size
          height,         // .... 
          hWndParent,                     // No parent window
          NULL,                     // No menu
          NULL,	         // Program Instance handle
          0                      // No window creation data
        );

    if (hWnd == NULL)
        return 0;


    hImageList = ImageList_Create(cx, cy,flag, cInitial, cGrow);

    // Set the image list.
    SendMessage(hWnd, TB_SETIMAGELIST, 
                (WPARAM)ImageListID, 
                (LPARAM)hImageList);

    
    ImageList_Add(
		hImageList,
		hBmpToolbar,
		NULL
    );


    // Add buttons.
    SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWnd, TB_ADDBUTTONS,(WPARAM)cInitial,(LPARAM)tbButtons);

    // Resize the toolbar, and then show it.

    SendMessage(hWnd, TB_AUTOSIZE, 0, 0); 
    ShowWindow(hWnd,  TRUE);
    
    return TRUE;

}

int toolbarDraw::initSTD(  HWND hWndParent,
			LPCTSTR windowName,
			const int cInitial,
			TBBUTTON *tbButtons
			)
{
   

   hWnd = CreateWindow(
          TOOLBARCLASSNAME,    // the window class name
          windowName,   		// The window title
          dwStyle,          // Window style as overlapped
          xPos,         // Default screen position of upper left
          yPos,         // corner of our window as x,y...
          width,         // Default window size
          height,         // .... 
          hWndParent,                     // No parent window
          NULL,                     // No menu
          NULL,	         // Program Instance handle
          0                      // No window creation data
        );

    if (hWnd == NULL)
        return 0;


    hImageList = ImageList_Create(cx, cy,flag, cInitial, cGrow);

    // Set the image list.
    SendMessage(hWnd, TB_SETIMAGELIST, 
                (WPARAM)ImageListID, 
                (LPARAM)hImageList);

    SendMessage(hWnd, TB_LOADIMAGES, (WPARAM)IDB_STD_LARGE_COLOR, (LPARAM)HINST_COMMCTRL);    


    // Add buttons.
    SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWnd, TB_ADDBUTTONS,(WPARAM)cInitial,(LPARAM)tbButtons);

    // Resize the toolbar, and then show it.

    SendMessage(hWnd, TB_AUTOSIZE, 0, 0); 
    ShowWindow(hWnd,  TRUE);

    return TRUE;

}
