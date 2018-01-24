#include "headerui.h"

extern BaseWindow mainWindow; 

std::string BmpPic::fileNames;

BmpPic::BmpPic():curPosX(0),curPosY(0)
{}

void BmpPic::show(){
}

bool BmpPic::open(){
  	BOOL success;
   	static OPENFILENAME ofn;
   	static char szFile[MAX_PATH];
	static char strFilter[]="BMP file\0*.bmp\0";
	static char *pStrFilter=strFilter;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = mainWindow.getHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter=strFilter;

	success = GetOpenFileName(&ofn);
	if (success){
		fileNames+=szFile;
		fileNames+=";";

	   DialogBox(NULL,MAKEINTRESOURCE(IDD_PICDIALOG1),
			mainWindow.getHWND(),PicDlgProc);
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
	static HWND hwndEditX;
	static HWND hwndEditY;
	static HWND hwndEditAngle;
	static HWND hwndEditScale;
	BITMAP pv;
	HBITMAP hBitmap;
	static float xPos=0.0;
	static float yPos=0.0;
	static float scale=1.0;
	static float angle=0.0;
   	int dX, dY;
   	int ws, hs, wd, hd;
	int picIndex=0;
 
  	BOOL success;
   	static OPENFILENAME ofn;
   	static char szFile[MAX_PATH];
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
					if (HIWORD(wParam)==EN_KILLFOCUS)
					MessageBox(hDlg,"Edit X","Changed",
							MB_OK);
					break;

				case IDC_PICEDITY:
					if (HIWORD(wParam)==EN_KILLFOCUS)
					MessageBox(hDlg,"Edit Y","Changed",
							MB_OK);
					break;

				case IDC_PICEDITSCALE:
					if (HIWORD(wParam)==EN_KILLFOCUS)
					MessageBox(hDlg,"Edit Scale","Changed",
							MB_OK);
					break;

				case IDC_PICEDITANGLE:
					if (HIWORD(wParam)==EN_KILLFOCUS)
					MessageBox(hDlg,"Edit Angle","Changed",
							MB_OK);
					break;

				case IDOK:

					MessageBox(hDlg,"OK","Select",
							MB_OK);
					break;
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
						0x58012000
						);
					}
					else{
					SetWindowLong(
						hwndEditScale,
						GWL_STYLE,
						0x50012000
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
						0x58012000
						);
					}
					else{
					SetWindowLong(
						hwndEditAngle,
						GWL_STYLE,
						0x50012000
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

