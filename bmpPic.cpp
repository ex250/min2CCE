#include "headerui.h"
#include <math.h>

extern ModelWindow modelWindow;
extern BaseWindow mainWindow; 

bool isRealNumber(char * str){
	char *ptr;
	ptr=str;
	while (*ptr!=0){
		if (isdigit(static_cast<int>(*ptr))||*ptr=='.'||*ptr=='-')
			ptr++;
		else 
			return false;
	}
	return true;
}



std::string BmpPic::fileNames;
OPENFILENAME BmpPic::ofn;

BmpPic::BmpPic():curPosX(0),curPosY(0),sizeX(100),sizeY(100),
	scale(1.0),alfa(0.0)
{}

HBITMAP BmpPic::adaptedBMP(const char * pFileName){
/*      
 *     y^
 *      |
 * 	------------
 * 	|          |
 *	|          | 
 * 	|          | H
 * 	|          |
 *     O------------ -> x
 *           W
 */
HBITMAP hBitmap;

/*	float Xs=0;
	float Ys=0;
	float XO;
	float YO;

	alfa*=PI/180;

	hBitmap=(HBITMAP)LoadImage(NULL,
		  pFileName,IMAGE_BITMAP,0,0,
		  LR_LOADFROMFILE|
		  LR_CREATEDIBSECTION);

	if(hBitmap==NULL){
		return hBitmap;//error
	}

	GetObject(hBitmap, sizeof(pv),&pv);

	sizeX=static_cast<float>(pv.bmWidth)*(scale*10);
	sizeY=static_cast<float>(pv.bmHeight)*(scale*10);

	if(alfa!=0)
	if (alfa>0&&alfa<=PI/2){
		cout<<"I chetvert"<<endl;
		HH=H*cos(alfa)+W*sin(alfa);
		WW=W*cos(alfa)+H*sin(alfa);
		XO=Xs-H*sin(alfa);
		YO=Ys+HH;
	}
	else if(alfa>PI/2&&alfa<=PI){
		cout<<"II chetvert"<<endl;
		HH=-H*cos(alfa)+W*sin(alfa);
		WW=-W*cos(alfa)+H*sin(alfa);
		XO=Xs-WW;
		YO=Ys+W*sin(alfa);
	}
	else if(alfa>PI&&alfa<=3*PI/2){
		cout<<"III chetvert"<<endl;
		HH=-H*cos(alfa)-W*sin(alfa);
		WW=-W*cos(alfa)-H*sin(alfa);
		XO=Xs-W*cos(alfa);
		YO=Ys;
	}
	else if(alfa>3*PI/2&&alfa<=2*PI){
		cout<<"IV chetvert"<<endl;
		HH=H*cos(alfa)-W*sin(alfa);
		WW=W*cos(alfa)-H*sin(alfa);
		XO=Xs;
		YO=Ys+H*cos(alfa);
	}

	XFORM rotMatrix;
	XFORM curMatrix;
	HDC hDC=GetDC(modelWindow.getHWND());
	HDC compDC=CreateCompatibleDC(hDC);

	SelectObject(compDC, hBitmap);

	rotMatrix.eM11=(FLOAT)cos(alfa);
	rotMatrix.eM12=(FLOAT)sin(alfa);
	rotMatrix.eM21=(FLOAT)-sin(alfa);
	rotMatrix.eM22=(FLOAT)cos(alfa);
	rotMatrix.eDx=curPosX;
	rotMatrix.eDy=curPosY;

	SetWorldTransform(compDC,&rotMatrix);

	StretchBlt(compDC,0,0,sizeX,-sizeY,
			compDC,0,0,pv.bmWidth,pv.bmHeight,SRCCOPY);
	hBitmap=CreateCompatibleBitmap(compDC,WW,HH);

	DeleteDC(compDC);
	DeleteDC(compDC2);
	ReleaseDC(modelWindow.getHWND(),hDC);
	
*/
	return hBitmap;
}

void BmpPic::show(){
	XFORM rotMatrix;
	XFORM curMatrix;
	HDC hDC=GetDC(modelWindow.getHWND());
	HDC compDC=CreateCompatibleDC(hDC);
	SelectObject(compDC, hBitmap);
	SaveDC(hDC);

	GetWorldTransform(hDC,&curMatrix);

	rotMatrix.eM11=(FLOAT)cos(alfa);
	rotMatrix.eM12=(FLOAT)sin(alfa);
	rotMatrix.eM21=(FLOAT)-sin(alfa);
	rotMatrix.eM22=(FLOAT)cos(alfa);
	rotMatrix.eDx=curPosX;
	rotMatrix.eDy=curPosY;

	SetWorldTransform(hDC,&rotMatrix);
	ModifyWorldTransform(hDC,&curMatrix,MWT_RIGHTMULTIPLY);

	StretchBlt(hDC,0,0,sizeX,sizeY,
			compDC,0,0,pv.bmWidth,pv.bmHeight,SRCCOPY);

	RestoreDC(hDC,-1);
	DeleteDC(compDC);
	ReleaseDC(modelWindow.getHWND(),hDC);
}

bool BmpPic::open(){
  	BOOL success;
   	char szFile[MAXFILENAME];
	char strFilter[]="BMP file\0*.bmp\0";
	char *pStrFilter=strFilter;

	if (hBitmap)
		DeleteObject((HBITMAP)hBitmap);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = mainWindow.getHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter=strFilter;

	success = GetOpenFileName(&ofn);
	if (success){
		fileNames+=szFile;
		fileNames+=";";

	        DialogBoxParam(NULL,MAKEINTRESOURCE(IDD_PICDIALOG1),
			mainWindow.getHWND(),PicDlgProc,
			(LPARAM)this);
	}
        else
        {
				MessageBox(mainWindow.getHWND(),
				"GetOpenFileName",
				"Отказ от выбора или ошибка",
			       	MB_ICONWARNING);
				return false;
        }


	return true;
}

int BmpPic::getWidth(){
	return 0;
}

int BmpPic::getHeight(){
	return 0;
}

bool BmpPic::setVisible(bool v){
	visible=v;
	return visible;
}


BOOL CALLBACK BmpPic::PicDlgProc(HWND hDlg,UINT uMsg,
		WPARAM wParam,LPARAM lParam)
{
	static BmpPic *bmp;
	static HWND hwndEditX;
	static HWND hwndEditY;
	static HWND hwndEditAngle;
	static HWND hwndEditScale;
	HDC hDC;
	static float xPos=0.0;
	static float yPos=0.0;
	static float scale=1.0;
	static float angle=0.0;
	int picIndex=0;
	bool flagNumber;
 
  	BOOL success;
   	static char szFile[MAXFILENAME];
	static const char strFilter[]="BMP file\0*.bmp\0";
	static const char *pStrFilter=strFilter;
	bool checkInsPoint=true;
	bool checkScale=true;
	bool checkRotate=true;

   	BOOL isFileLoaded;
   	int substrate;
	char *ptrFileNameBuffer;
	char temp[256];

	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
			bmp=(BmpPic*)lParam;
			if (!bmp){
				MessageBox(hDlg,"Error retrieve pointer to BMP","ERROR",MB_OK);
				return -1;
			}


			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = mainWindow.getHWND();
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter=strFilter;

			int size=fileNames.size();
			char *ptrFile;
			char *ptrLastFile;
			ptrFileNameBuffer=new char[size];
			ptrFile=ptrFileNameBuffer;
			fileNames.copy(ptrFileNameBuffer,size,0);
			*(ptrFileNameBuffer+size)=0;

			ptrFile=strtok(ptrFile,";");
			while(ptrFile){
			   SendDlgItemMessage(
					hDlg,
					IDC_PICCOMBOPATH,
					CB_ADDSTRING,
					(WPARAM)NULL,
					(LPARAM)ptrFile
					);
			   ptrLastFile=ptrFile;
			   ptrFile=NULL;
			   ptrFile=strtok(ptrFile,";");
			}

			picIndex=SendDlgItemMessage(
					hDlg,
					IDC_PICCOMBOPATH,
					CB_FINDSTRING,
					(WPARAM)-1,
					(LPARAM)ptrLastFile
					);

			if (picIndex!=CB_ERR){
			SendDlgItemMessage(
					hDlg,
					IDC_PICCOMBOPATH,
					CB_SETCURSEL,
					(WPARAM)picIndex,
					(LPARAM)NULL
					);
			}

			hwndEditX=GetDlgItem(
					hDlg,
					IDC_PICEDITX
					);

			hwndEditY=GetDlgItem(
					hDlg,
					IDC_PICEDITY
					);

			hwndEditAngle=GetDlgItem(
					hDlg,
					IDC_PICEDITANGLE
					);

			hwndEditScale=GetDlgItem(
					hDlg,
					IDC_PICEDITSCALE
					);

			SendDlgItemMessage(
					hDlg,
					IDC_PICCHECKINSPOINT,
					BM_SETCHECK,
					checkInsPoint,
					(LPARAM)NULL
					);

			SendDlgItemMessage(
					hDlg,
					IDC_PICCHECKSCALE,
					BM_SETCHECK,
					checkScale,
					(LPARAM)NULL
					);

			SendDlgItemMessage(
					hDlg,
					IDC_PICCHECKROTATE,
					BM_SETCHECK,
					checkRotate,
					(LPARAM)NULL
					);

			sprintf(temp,"%4.2f",xPos);

			SendDlgItemMessage(
					hDlg,
					IDC_PICEDITX,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);
			
			sprintf(temp,"%4.2f",yPos);
			
			SendDlgItemMessage(
					hDlg,
					IDC_PICEDITY,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);

			sprintf(temp,"%4.2f",scale);

			SendDlgItemMessage(
					hDlg,
					IDC_PICEDITSCALE,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);

			sprintf(temp,"%4.2f",angle);
	
			SendDlgItemMessage(
					hDlg,
					IDC_PICEDITANGLE,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);
			}

			return TRUE;
		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				case IDC_PICEDITX:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_PICEDITX,
						WM_GETTEXT,
						(WPARAM)16,
						(LPARAM)temp
					   );
					   flagNumber=isRealNumber(temp);
					   if (flagNumber)
						   xPos=atof(temp);
					   else {
						MessageBox(hDlg,
						"Invalid value X","Error",
						MB_OK);
						SetFocus(hwndEditX);
					   }
					}
					break;

				case IDC_PICEDITY:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_PICEDITY,
						WM_GETTEXT,
						(WPARAM)16,
						(LPARAM)temp
					   );
					   flagNumber=isRealNumber(temp);
					   if (flagNumber)
						   yPos=atof(temp);
					   else {
						MessageBox(hDlg,
						"Invalid value Y","Error",
						MB_OK);
						SetFocus(hwndEditY);
					   }
					}
					break;

				case IDC_PICEDITSCALE:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_PICEDITSCALE,
						WM_GETTEXT,
						(WPARAM)16,
						(LPARAM)temp
					   );
					   flagNumber=isRealNumber(temp);
					   if (flagNumber)
						   scale=atof(temp);
					   else {
						MessageBox(hDlg,
						"Invalid value Scale","Error",
						MB_OK);
						SetFocus(hwndEditScale);
					   }
					}
					break;

				case IDC_PICEDITANGLE:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_PICEDITANGLE,
						WM_GETTEXT,
						(WPARAM)16,
						(LPARAM)temp
					   );
					   flagNumber=isRealNumber(temp);
					   if (flagNumber)
						   angle=atof(temp);
					   else {
						MessageBox(hDlg,
						"Invalid value Angle","Error",
						MB_OK);
						SetFocus(hwndEditAngle);
					   }
					}
					break;

				case IDOK:

					bmp->curPosX=xPos*100;
					bmp->curPosY=yPos*100;

					picIndex=SendDlgItemMessage(
						hDlg,
						IDC_PICCOMBOPATH,
						CB_GETCURSEL,
						(WPARAM)NULL,
						(LPARAM)NULL
					);

					bmp->index=picIndex;

					SendDlgItemMessage(
						hDlg,
						IDC_PICCOMBOPATH,
						CB_GETLBTEXT,
						(WPARAM)picIndex,
						(LPARAM)temp
					);

				//bmp->adaptedBMP(temp);


					

				//	bmp->show();

					EndDialog(hDlg,0);

					return true;

				case IDC_PICBUTTONHELP:
					MessageBox(hDlg,"HELP","Select",
							MB_OK);
					break;
				case IDC_PICBUTTONPATH:
	       success = GetOpenFileName(&ofn);
	       if (success){
			SendDlgItemMessage(
					hDlg,
					IDC_PICCOMBOPATH,
					CB_INSERTSTRING,
					(WPARAM)NULL,
					(LPARAM)ofn.lpstrFile
					);
			   }
	       else
				MessageBox(hDlg, "GetOpenFileName",
				"Отказ от выбора или ошибка", MB_ICONWARNING);

	       			break;
				case IDC_PICCHECKINSPOINT:
					if(SendDlgItemMessage(
						hDlg,
						IDC_PICCHECKINSPOINT,
						BM_GETCHECK,
						(WPARAM)NULL,
						(LPARAM)NULL
					)==BST_CHECKED)
					{
						checkInsPoint=FALSE;
						SetWindowLong(
						hwndEditX,
						GWL_STYLE,
						0x58010000
						);

						SetWindowLong(
						hwndEditY,
						GWL_STYLE,
						0x58010000
						);
					}
					else{
					SetWindowLong(
						hwndEditX,
						GWL_STYLE,
						0x50010000
						);
					SetWindowLong(
						hwndEditY,
						GWL_STYLE,
						0x50010000
						);
						checkRotate=TRUE;
					}

					SendDlgItemMessage(
						hDlg,
						IDC_PICEDITX,
						WM_ENABLE,
						(WPARAM)checkInsPoint,
						(LPARAM)NULL
					);

					SendDlgItemMessage(
						hDlg,
						IDC_PICEDITY,
						WM_ENABLE,
						(WPARAM)checkInsPoint,
						(LPARAM)NULL
					);
				break;

				case IDC_PICCHECKSCALE:
					if(SendDlgItemMessage(
						hDlg,
						IDC_PICCHECKSCALE,
						BM_GETCHECK,
						(WPARAM)NULL,
						(LPARAM)NULL
					)==BST_CHECKED)
					{
						checkScale=FALSE;
						SetWindowLong(
						hwndEditScale,
						GWL_STYLE,
						0x58010000
						);
					}
					else{
					SetWindowLong(
						hwndEditScale,
						GWL_STYLE,
						0x50010000
						);
						checkRotate=TRUE;
					}

					SendDlgItemMessage(
						hDlg,
						IDC_PICEDITSCALE,
						WM_ENABLE,
						(WPARAM)checkScale,
						(LPARAM)NULL
					);

				break;
				case IDC_PICCHECKROTATE:
					if(SendDlgItemMessage(
						hDlg,
						IDC_PICCHECKROTATE,
						BM_GETCHECK,
						(WPARAM)NULL,
						(LPARAM)NULL
					)==BST_CHECKED){
						checkRotate=FALSE;
					SetWindowLong(
						hwndEditAngle,
						GWL_STYLE,
						0x58010000
						);
					}
					else{
					SetWindowLong(
						hwndEditAngle,
						GWL_STYLE,
						0x50010000
						);
						checkRotate=TRUE;
					}

					SendDlgItemMessage(
						hDlg,
						IDC_PICEDITANGLE,
						WM_ENABLE,
						(WPARAM)checkRotate,
						(LPARAM)NULL
					);

				break;
				case IDCANCEL:
					EndDialog(hDlg,0);
					return true;
			}
			break;
	}
	return false;
}

