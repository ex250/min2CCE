#include "resource.h"

long WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
long WINAPI cmdProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

const int HSTBAR=35; 	//высота строки состояния
const int WTBDRAW=70;   //ширина инстр. панели DRAW
const int HTBSTD=35;    //высота стандартной инстр. панели
const int WTBMODIFY=70;
const int HTBVERT=500;
const int XPOSWINDOW=50;
const int YPOSWINDOW=20;
const int HCOMSTR=70;
const int COMBO_SPACE_WIDTH=100;
const int COMBO_SPACE_HEIGHT=300;
const int SEPARATOR_WIDTH=20;


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
      LoadCursor(0, IDC_ARROW),
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
        { 0, IDM_LINE,              TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╦шэш " },
        { 1, IDM_ARCSTARTENDRADIUS, TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"─єур2ЄR" },
        { 2, IDM_ARC3POINTS,        TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"─єур3Є" },
        { 3, IDM_ARCCENTREANG1ANG2, TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"─єур╓R╙" },
        { 4, IDM_CIRCCENTRRAD,      TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╬ъЁєцэ╓R" },
        { 5, IDM_CIRCCENTRDIAM,     TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╬ъЁєцэ╓D" },
        { 6, IDM_CIRC3POINTS,       TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╬ъЁєцэ3Є" },
        { 7, IDM_CONTUR,            TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╩юэЄєЁ" },
        { 8, IDM_RECTANGLE,         TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╧Ё ьюєу." },
        { 9, IDM_POLYGON,           TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╧юышуюэ" },
        { 10, IDM_TEXT,             TBSTATE_ENABLED|TBSTATE_WRAP, TBSTYLE_BUTTON, {0}, 0, (INT_PTR)"╥хъёЄ"},
};

TBBUTTON TBMODIFY[numTbModify] = 
    {
        { 0, IDM_ERASE,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╬ўшёЄшЄ№" },
        { 1, IDM_PCOPY,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╩юяш " },
        { 2, IDM_MIRROR, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╟хЁърыю" },
        { 3, IDM_OFFSET, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"▌ътшфшёЄ." },
        { 4, IDM_ARRAY,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╠рёёшт" },
        { 5, IDM_MOVE,   TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╧хЁхьхёЄшЄ№" },
        { 6, IDM_ROTATE, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╧ютюЁюЄ" },
        { 7, IDM_SCALE,  TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╠рё°Єрс" },
        { 8, IDM_TRIM,   TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╬сЁхчрЄ№" },
        { 9, IDM_LENGTH, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╧ЁюфышЄ№" },
        { 10,IDM_CHAMFER,TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╘рёър" },
        { 11,IDM_FILLET, TBSTATE_ENABLED|TBSTATE_WRAP, buttonStyles, {0}, 0, (INT_PTR)"╤ъЁєуышЄ№" }

};

TBBUTTON TBSTD[numTbStd] = 
{
        { STD_FILENEW, IDM_NEW,   TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { STD_FILEOPEN, IDM_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { STD_FILESAVE, IDM_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, {0}, 0, 0 },
        { SEPARATOR_WIDTH,   ID_SEP,   TBSTATE_ENABLED, TBSTYLE_SEP, {0}, 0, 0 },
        { COMBO_SPACE_WIDTH, ID_SEP,   TBSTATE_ENABLED, TBSTYLE_SEP, {0}, 0, 0 }
};
