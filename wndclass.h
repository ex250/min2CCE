#ifndef WNDCLASSH
#define WNDCLASSH
#include "resource.h"

long WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
long WINAPI cmdProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



// определение класса главного окна
WNDCLASS OFwin = {
      CS_HREDRAW | CS_VREDRAW ,
      WindowProc,
      0,          // No extra bytes after the window class
      0,          // structure or the window instance
      NULL,   // Application instance handle
      LoadIcon(0, IDI_APPLICATION),
      LoadCursor(0, IDC_ARROW),
      static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)),
      MAKEINTRESOURCE(MYMENU),           // No menu, so no menu resource name
      "OFWin"  // Set class name
};

// определение класса дочерних окон
WNDCLASS TBwin = {
      CS_HREDRAW | CS_VREDRAW,
      WindowProc,
      0,          // No extra bytes after the window class
      0,          // structure or the window instance
      NULL,   // Application instance handle
      LoadIcon(0, IDI_APPLICATION),
      LoadCursor(0, IDC_ARROW),
      static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
      0,           // No menu, so no menu resource name
      "toolbarClass"  // Set class name
};

// определение класса окна отображения модели
WNDCLASS modelWin = {
      CS_HREDRAW | CS_VREDRAW|CS_OWNDC,
      WindowProc,
      0,          // No extra bytes after the window class
      0,          // structure or the window instance
      NULL,   // Application instance handle
      LoadIcon(0, IDI_APPLICATION),
      LoadCursor(0, IDC_CROSS),
      static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
      0,           // No menu, so no menu resource name
      "modelWindowClass"  // Set class name
};


//константы инструментальных панелей
const int numTbDraw=11;
const int numTbModify=12;
const int numTbStd=5;
const int buttonStyles=TBSTYLE_BUTTON;

//структуры инструментальных панелей
TBBUTTON TBDRAW[numTbDraw] = 
    {
        { 0, IDM_LINE, TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON,
	      {0}, 0, 0 },
        { 1, IDM_ARCSTARTENDRADIUS, TBSTATE_ENABLED|TBSTATE_WRAP,
	      TBSTYLE_BUTTON, {0}, 0, 0 },
        { 2, IDM_ARC3POINTS,        TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 3, IDM_ARCCENTREANG1ANG2, TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 4, IDM_CIRCCENTRRAD,      TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 5, IDM_CIRCCENTRDIAM,     TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 6, IDM_CIRC3POINTS,       TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 7, IDM_CONTUR,TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 8, IDM_RECTANGLE, TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 9, IDM_POLYGON, TBSTATE_ENABLED|TBSTATE_WRAP,
	     TBSTYLE_BUTTON, {0}, 0, 0 },
        { 10, IDM_TEXT, TBSTATE_ENABLED|TBSTATE_WRAP,
	      TBSTYLE_BUTTON, {0}, 0, 0},
};

TBBUTTON TBMODIFY[numTbModify] = 
{
  { 0, IDM_ERASE,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 1, IDM_PCOPY,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 2, IDM_MIRROR, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 3, IDM_OFFSET, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 4, IDM_ARRAY,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 5, IDM_MOVE,   TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 6, IDM_ROTATE, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 7, IDM_SCALE,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 8, IDM_TRIM,   TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 9, IDM_LENGTH, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 10,IDM_CHAMFER,TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 },
  { 11,IDM_FILLET, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, 0 }
};

TBBUTTON TBSTD[numTbStd] = 
{
        { STD_FILENEW, IDM_NEW,   TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { STD_FILEOPEN, IDM_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { STD_FILESAVE, IDM_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { SEPARATOR_WIDTH,   ID_SEP,   TBSTATE_ENABLED, TBSTYLE_SEP, {0}, 0, 0 },
        { COMBO_SPACE_WIDTH, ID_SEP,   TBSTATE_ENABLED, TBSTYLE_SEP, {0}, 0, 0 }
};
#endif
