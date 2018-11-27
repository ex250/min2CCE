#include "headerui.h"
#include "model.h"
#include "myvector.h"

extern ModelWindow modelWindow;
extern BaseWindow mainWindow; 
extern CommandLine comStr;
extern TextEntities textEntities;
extern Model myModel;
extern Layer* currentLayer;
extern Layer layer[];

enum {LINEAR,BIARC};

float fromFixed(FIXED fixed);

bool lineBez(float,float,float,float,float,float,float,float);

bool arcBez(vec2 B0, vec2 B1, vec2 B2, float tau,float);

bool isRealNumber(char * str);

int getArcDirection(vec2 p1, vec2 p2, vec2 p3);

TextEntities::TextEntities():insX(0.0),insY(0.0),
	angle(0.0),len(-1),numLines(10),tau(0.5),aproxSwitch(LINEAR)
{}

void TextEntities::show(){
	DialogBoxParam(NULL,MAKEINTRESOURCE(IDD_DIALOGTEXT),
			mainWindow.getHWND(),TextDlgProc,
			(LPARAM)this);
}

void TextEntities::setXY(float x,float y){
	insX=x;
	insY=y;
}

void TextEntities::setAngle(float an){
	angle=an;
}

float TextEntities::getInsX(){
	return insX;
}

float TextEntities::getInsY(){
	return insY;
}

float TextEntities::getAngle(){
	return angle;
}

BOOL CALLBACK TextEntities::TextDlgProc(HWND hDlg,UINT uMsg,
		WPARAM wParam,LPARAM lParam)
{
	static TextEntities *pText;
	static char sentence[256];
	static HWND hwndEditText;
	static HWND hwndEditX;
	static HWND hwndEditY;
	static HWND hwndEditAngle;
	static HWND hwndEditLen;
	static HWND hwndEditNumblines;
	static HWND hwndEditTolerance;

	HDC hDC;

	static float xPos;
	static float yPos;
	static float len;
	static float angle;
	static float deviation;
	static int nLines;
	static short aproxSwitch;
 
	bool radioApproxLine=true;
	bool radioApproxArc=false;
	bool flagNumber;

	static LOGFONT tempLf={
	   36,0,0,0,700,
	   true,false,false,
	   DEFAULT_CHARSET,
	   OUT_TT_ONLY_PRECIS,
	   CLIP_DEFAULT_PRECIS,
	   DEFAULT_QUALITY,
	   FF_SWISS|DEFAULT_PITCH,
	   "Times"
   	};

   static LOGFONT lf={
	   36,0,0,0,700,
	   true,false,false,
	   DEFAULT_CHARSET,
	   OUT_TT_ONLY_PRECIS,
	   CLIP_DEFAULT_PRECIS,
	   DEFAULT_QUALITY,
	   FF_SWISS|DEFAULT_PITCH,
	   "Times"
   };

   // font for IDC_FONTNAME
   static LOGFONT lfFont={
	   24,0,0,0,700,
	   false,false,false,
	   DEFAULT_CHARSET,
	   OUT_TT_ONLY_PRECIS,
	   CLIP_DEFAULT_PRECIS,
	   DEFAULT_QUALITY,
	   FF_SWISS|DEFAULT_PITCH,
	   "Arial"
   };

   static HFONT hFont;
   static CHOOSEFONT chf;
   static HFONT hOldFont;

	static char temp[256];

	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
			pText=(TextEntities*)lParam;
			if (!pText){
				MessageBox(hDlg,"Error retrieve pointer to Text","ERROR",MB_OK);
				return -1;
			}

			xPos=pText->insX;
			yPos=pText->insY;
			angle=pText->angle;
			len=pText->len;
			deviation=pText->tau;
			nLines=pText->numLines;
			aproxSwitch=pText->aproxSwitch;

			chf.lStructSize=sizeof(CHOOSEFONT);
			chf.lpLogFont=&lf;
			chf.Flags=CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT;
			chf.nFontType=SIMULATED_FONTTYPE;

			hwndEditX=GetDlgItem(
					hDlg,
					IDC_TEXT_INSPOINTX
					);

			hwndEditY=GetDlgItem(
					hDlg,
					IDC_TEXT_INSPOINTY
					);

			hwndEditAngle=GetDlgItem(
					hDlg,
					IDC_TEXT_ANGLE
					);

			hwndEditLen=GetDlgItem(
					hDlg,
					IDC_TEXT_LEN
					);

			hwndEditNumblines=GetDlgItem(
					hDlg,
					IDC_TEXT_NUMBLINES
					);

			hwndEditTolerance=GetDlgItem(
					hDlg,
					IDC_TEXT_NUMBLINES
					);

			hwndEditText=GetDlgItem(
					hDlg,
					IDC_TEXTINPUT
					);

			if (strlen(sentence)>=1)
				SendDlgItemMessage(
					hDlg,
					IDC_TEXTINPUT,
					WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)sentence
					);

				   SendDlgItemMessage(
					hDlg,
					IDC_TEXTFONTNAME,
					WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)lf.lfFaceName
					);

			hFont=CreateFontIndirect(&tempLf);
			hOldFont=CreateFontIndirect(&lfFont);

			SendDlgItemMessage(
					hDlg,
					IDC_TEXTINPUT,
					WM_SETFONT,
					(WPARAM)hFont,
					(LPARAM)true
					);

			SendDlgItemMessage(
					hDlg,
					IDC_TEXTFONTNAME,
					WM_SETFONT,
					(WPARAM)hOldFont,
					(LPARAM)true
					);

			if (aproxSwitch==LINEAR) {
			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_APPROXLINEAR,
					BM_SETCHECK,
					true,
					(LPARAM)NULL
					);

			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_APPROXBIARC,
					BM_SETCHECK,
					false,
					(LPARAM)NULL
					);
			}
			else
			{
			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_APPROXLINEAR,
					BM_SETCHECK,
					false,
					(LPARAM)NULL
					);

			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_APPROXBIARC,
					BM_SETCHECK,
					true,
					(LPARAM)NULL
					);
			}

			sprintf(temp,"%4.2f",xPos);

			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_INSPOINTX,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);
			
			sprintf(temp,"%4.2f",yPos);
			
			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_INSPOINTY,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);

			if (len<=0)
				strcpy(temp,"default");
			else 
				sprintf(temp,"%4.2f",len);

			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_LEN,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);

			sprintf(temp,"%4.2f",angle);
	
			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_ANGLE,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);

        		for(int i=2;i<=30;i++){
				sprintf(temp,"%d",i);
				SendDlgItemMessage(
					hDlg,
					IDC_TEXT_NUMBLINES,
				       	CB_ADDSTRING,
					(WPARAM)NULL,
					(LPARAM)temp
					);
			}

				SendDlgItemMessage(
					hDlg,
					IDC_TEXT_NUMBLINES,
				       	CB_SETCURSEL,
					(WPARAM)nLines-2,
					(LPARAM)NULL
					);

			sprintf(temp,"%4.2f",deviation);
			SendDlgItemMessage(
					hDlg,
					IDC_TEXT_TOLERANCE,
				       	WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)temp
					);

			SetFocus(hwndEditText);
			}
			
			return FALSE;

		case WM_COMMAND:

			switch(LOWORD(wParam))
			{
				case IDC_TEXT_BUTTON_FONT:
				 if(ChooseFont(&chf)){
				   DeleteObject(hFont);
				   if (chf.lpLogFont->lfHeight<-96){
			           tempLf=*chf.lpLogFont;
				   tempLf.lfHeight=-96;
			  	   hFont=CreateFontIndirect(&tempLf);
				   SendDlgItemMessage(
					hDlg,
					IDC_TEXTINPUT,
					WM_SETFONT,
					(WPARAM)hFont,
					(LPARAM)true
					);
				   }
				   else {
			  	   hFont=CreateFontIndirect(chf.lpLogFont);
				   SendDlgItemMessage(
					hDlg,
					IDC_TEXTINPUT,
					WM_SETFONT,
					(WPARAM)hFont,
					(LPARAM)true
					);
				   }

				   SendDlgItemMessage(
					hDlg,
					IDC_TEXTFONTNAME,
					WM_SETTEXT,
					(WPARAM)NULL,
					(LPARAM)lf.lfFaceName
					);
				 }
				 break;
				case IDC_TEXT_INSPOINTX:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_TEXT_INSPOINTX,
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

				case IDC_TEXT_INSPOINTY:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_TEXT_INSPOINTY,
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

				case IDC_TEXT_ANGLE:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_TEXT_ANGLE,
						WM_GETTEXT,
						(WPARAM)16,
						(LPARAM)temp
					   );
					   flagNumber=isRealNumber(temp);
					   if (flagNumber)
						   angle=atof(temp);
					   else {
						MessageBox(hDlg,
						"Invalid value angle","Error",
						MB_OK);
						SetFocus(hwndEditAngle);
					   }
					}
					break;

				case IDC_TEXT_LEN:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_TEXT_LEN,
						WM_GETTEXT,
						(WPARAM)16,
						(LPARAM)temp
					   );
					   flagNumber=isRealNumber(temp);
					   if (flagNumber)
						   len=atof(temp);
					   else {
						MessageBox(hDlg,
						"Invalid value Lenght","Error",
						MB_OK);
						SetFocus(hwndEditLen);
					   }
					}
					break;

				case IDC_TEXT_NUMBLINES:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   nLines=SendDlgItemMessage(
						hDlg,
						IDC_TEXT_NUMBLINES,
						CB_GETCURSEL,
						(WPARAM)NULL,
						(LPARAM)NULL
					   );

					   sprintf(temp,"%d",nLines);

					   MessageBox(hDlg,
						temp,"Error", MB_OK);

					   if (nLines==CB_ERR){
						MessageBox(hDlg,
						"Invalid value Num Lines","Error",
						MB_OK);
						SetFocus(hwndEditNumblines);
					   }
					}
					break;

				case IDC_TEXT_TOLERANCE:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_TEXT_TOLERANCE,
						WM_GETTEXT,
						(WPARAM)16,
						(LPARAM)temp
					   );
					   flagNumber=isRealNumber(temp);
					   if (flagNumber)
						   deviation=atof(temp);
					   else {
						MessageBox(hDlg,
						"Invalid value Deviation","Error",
						MB_OK);
						SetFocus(hwndEditTolerance);
					   }
					}
					break;

				case IDC_TEXTINPUT:
					if (HIWORD(wParam)==EN_KILLFOCUS){
					   SendDlgItemMessage(
						hDlg,
						IDC_TEXTINPUT,
						WM_GETTEXT,
						(WPARAM)256,
						(LPARAM)sentence
					   );
					}
					break;

				case IDOK:
					pText->insX=xPos;
					pText->insY=yPos;
					pText->angle=angle;
					pText->len=len;
					pText->ptrStr=sentence;

					nLines=SendDlgItemMessage(
						hDlg,
						IDC_TEXT_NUMBLINES,
						CB_GETCURSEL,
						(WPARAM)NULL,
						(LPARAM)NULL
					   );

					   if (nLines==CB_ERR){
						MessageBox(hDlg,
						"Invalid value Num Lines","Error",
						MB_OK);
						nLines=8;
					   }

					pText->numLines=nLines+2;
					pText->tau=deviation;
					pText->aproxSwitch=aproxSwitch;

					pText->makeText(&lf);

				   	DeleteObject(hFont);
				   	DeleteObject(hOldFont);

					EndDialog(hDlg,0);

					return true;

				case IDC_TEXT_APPROXLINEAR:
					aproxSwitch=LINEAR;
					break;

				case IDC_TEXT_APPROXBIARC:
					aproxSwitch=BIARC;
					break;

				case IDC_TEXT_BUTTON_INSPOINT:
				    comStr.setState(STATE_TEXT_INSPOINT);
				   	DeleteObject(hFont);
				   	DeleteObject(hOldFont);
					EndDialog(hDlg,0);
					break;

				case IDC_TEXT_BUTTONANGLE:
				    comStr.setState(STATE_TEXT_ANGLE1);
	   				modelWindow.setROP2(R2_NOTXORPEN);
					EndDialog(hDlg,0);
					break;

				case IDHLPFONT:
					MessageBox(hDlg,"HELP","Select", MB_OK);
					break;

				case IDCANCEL:
				   	DeleteObject(hFont);
				   	DeleteObject(hOldFont);
					EndDialog(hDlg,0);
					return true;
			}
			break;
	}
	return false;
}

bool TextEntities::makeText(LOGFONT* pLf){
   HDC hDC;                       // Display context handle

   static char buffer[128];
   static char bufHeader[128];
   static char bufCurve[128];
   static MAT2 matrix={{0,1},{0,0},{0,0},{0,1}};
   static GLYPHMETRICS glyphMetrics;
   static HFONT hFont;
   static HFONT hOldFont;
   static char *ptrToBuffer;
   unsigned char symbol=(unsigned char)*ptrStr;
   float x0,y0;
   float x1,y1;
   float x2,y2;
   float xs,ys;
   static Point p1,p2;
   static float startPos=0;
   float charWidth;
   int intCharWidth;
   static ABCFLOAT abc;
   static LPABCFLOAT lpabc;
   lpabc=&abc;
   
         hDC = GetDC(comStr.getHWND());

	   hFont=CreateFontIndirect(pLf);
	   hOldFont=(HFONT)SelectObject(hDC,hFont);

	   //---------------

	   while (*ptrStr!='\0'){

	   DWORD bufferSize=GetGlyphOutline(
			   hDC,symbol,GGO_NATIVE,
			   &glyphMetrics,0,0,&matrix);
	   if (bufferSize==GDI_ERROR){
		   MessageBox(modelWindow.getHWND(),"invalid font","Error",
				   MB_OK);
		   return false;
	   }

	   ptrToBuffer=new char[bufferSize];

	   if (!ptrToBuffer){
		   MessageBox(modelWindow.getHWND(),"can't create array char","Error",
				   MB_OK);
		   exit(-1);
	   }

	   DWORD error=GetGlyphOutline(
			   hDC,symbol,GGO_NATIVE,
			   &glyphMetrics,
			   bufferSize,
			   ptrToBuffer,
			   &matrix);

	   TTPOLYGONHEADER* outlinesData=(TTPOLYGONHEADER*)(ptrToBuffer);

	   TTPOLYCURVE* curveData=(TTPOLYCURVE*)(outlinesData+1);

	   LPTTPOLYGONHEADER dataStart=outlinesData;

	   error=false;


	   while ((DWORD)outlinesData<(DWORD)(((LPSTR)dataStart)+bufferSize)){
	   //comStr.addTextToHistory("FONT:NEW CONTUR");
		   
	   x0=fromFixed(outlinesData->pfxStart.x);
	   y0=fromFixed(outlinesData->pfxStart.y);
	   xs=x0;
	   ys=y0;

	   TTPOLYCURVE* curveData=(TTPOLYCURVE*)(outlinesData+1);

	   while((DWORD)curveData<(DWORD)(((LPSTR)outlinesData)+
				   outlinesData->cb)){

	   if (curveData->wType==TT_PRIM_LINE){

	   float x;
	   float y;

		   for(WORD i=0;i<curveData->cpfx;i++){
			   x=fromFixed(curveData->apfx[i].x);
			   y=fromFixed(curveData->apfx[i].y);
			   p1.setXY(x0,y0);
			   p2.setXY(x,y);
			   p1.move(startPos,0);
			   p2.move(startPos,0);
	   		   p1.rotate(angle);
	   		   p2.rotate(angle);
	   		   p1.move(insX,insY);
	   		   p2.move(insX,insY);
			   myModel.appendLine(&p1,&p2);
		   	   x0=x;
		           y0=y;
		   }
		   //-------END PRIM LINE-----------
	   }	   
	   else if (curveData->wType==TT_PRIM_QSPLINE)
	   {
		   for(WORD i=0;i<curveData->cpfx;){
			   //second point
			   x1=fromFixed(curveData->apfx[i].x);
			   y1=fromFixed(curveData->apfx[i].y);
			   i++;
			   //third point
			   if (i==(curveData->cpfx-1))
			   {
			   x2=fromFixed(curveData->apfx[i].x);
			   y2=fromFixed(curveData->apfx[i].y);
			   i++;
			   }
			   else 
			   {
			   //third control point is between current and next
			   x2=(fromFixed(curveData->apfx[i-1].x)+fromFixed(curveData->apfx[i].x))/2;
			   y2=(fromFixed(curveData->apfx[i-1].y)+fromFixed(curveData->apfx[i].y))/2;
			   }

	   vec2 B0(x0,y0);
	   vec2 B1(x1,y1);
	   vec2 B2(x2,y2);

	   if (aproxSwitch==LINEAR)
	      lineBez(x0,y0,x1,y1,x2,y2,startPos,1/((float)numLines));
	   else
	      arcBez(B0,B1,B2,0.5,startPos);

	   //myModel.showModel();

			   x0=x2;
			   y0=y2;
		   }//end apfx
		   
	// -------END BEZIER QUAD-----------

	   }
	   else if (curveData->wType==TT_PRIM_CSPLINE){
		   MessageBox(modelWindow.getHWND(),"Cubic spline","info",MB_OK);
		   error=true;
	   }
	   else{
		   MessageBox(modelWindow.getHWND(),"unknown type","info",MB_OK);
		   error=true;
	   }
	   curveData=(LPTTPOLYCURVE)&(curveData->apfx[curveData->cpfx]);
	   }// end of TTPOLYCURVE
	   if(error){
	   DeleteObject(SelectObject(hDC,hOldFont));
		   break;
	   }
	   //#############    END POLYGON    ############
	   

	   p1.setXY(x0,y0);
	   p2.setXY(xs,ys);
	   p1.move(startPos,0);
	   p2.move(startPos,0);
	   p1.rotate(angle);
	   p2.rotate(angle);
	   p1.move(insX,insY);
	   p2.move(insX,insY);
	   myModel.appendLine(&p1,&p2);

	   outlinesData=(LPTTPOLYGONHEADER)(((LPSTR)outlinesData)+outlinesData->cb);
	   }//end of TTPOLYGONHEADER

	   if(!GetCharABCWidthsFloat(hDC,(UINT)symbol,(UINT)symbol,lpabc))
	   	MessageBox(modelWindow.getHWND(),"ERROR:char width ","info",MB_OK);

           //sprintf(bufCurve,"width=%3.3f",lpabc->abcfA+lpabc->abcfB+lpabc->abcfC);
	   //MessageBox(modelWindow.getHWND(),bufCurve,"info",MB_OK);

    	   symbol=*(++ptrStr);
	   startPos+=(lpabc->abcfA+lpabc->abcfB+lpabc->abcfC);
	   }
	   startPos=0;

	   myModel.showModel();
	   DeleteObject(SelectObject(hDC,hOldFont));
	   ReleaseDC(comStr.getHWND(),hDC);
	   comStr.addTextToHistory("Text");

       return true;
}       

float fromFixed(FIXED fixed){
	return float(fixed.value)+float(fixed.fract)/USHRT_MAX;
}

bool lineBez(float x0,float y0,
		float x1,float y1,
		float x2,float y2,
		float startPos,
		float step)
{
	float u;
	float a,b,c;
	float x,y;
   	static Point p1;
   	static Point p2;
	for (u=0;u<1;)
	{
		a=(1-u)*(1-u);
		b=2*u*(1-u);
		c=u*u;
		x=x0*a+x1*b+x2*c;
		y=y0*a+y1*b+y2*c;
	   	p1.setXY(x,y);
		u+=step;

		a=(1-u)*(1-u);
		b=2*u*(1-u);
		c=u*u;
		x=x0*a+x1*b+x2*c;
		y=y0*a+y1*b+y2*c;
	   	p2.setXY(x,y);

	 	p1.move(startPos,0);
	   	p2.move(startPos,0);
	   	p1.rotate(textEntities.getAngle());
	        p2.rotate(textEntities.getAngle());
		p1.move(textEntities.getInsX(),textEntities.getInsY());
		p2.move(textEntities.getInsX(),textEntities.getInsY());

	   	myModel.appendLine(&p1,&p2);
	   	//myModel.showModel();
		//p1=p2;
	}
	return true;
}

//************************************************************************

bool arcBez(vec2 B0, vec2 B1, vec2 B2, 
		float tau,
		float startPos
		)
{
	vec2 b1b0=B1-B0;
	vec2 b2b1=B2-B1;
	vec2 b2b0=B2-B0;
	Point ps(B0);
	Point pe(B2);
	Point pcenter;
	vec2 offsetX(startPos,0);
	//currentLayer=&layer[0];

	float a=b1b0.length();
	float b=b2b1.length();
	float d=b2b0.length();

	if (a==0||b==0||d==0){
		//currentLayer=&layer[3];
	 	ps.move(startPos,0);
	   	pe.move(startPos,0);
	   	ps.rotate(textEntities.getAngle());
	        pe.rotate(textEntities.getAngle());
		ps.move(textEntities.getInsX(),textEntities.getInsY());
		pe.move(textEntities.getInsX(),textEntities.getInsY());
		myModel.appendLine(&ps,&pe);
		//currentLayer=&layer[1];
		return true;
	}

	vec2 T0=b1b0/a;
	vec2 T1=b2b1/b;
	vec2 T=b2b0/d;
	float cos_theta=dot(T0,T1);
	int arcDir;

// test collinear
	if (cos_theta>=0.99999||cos_theta<=-0.99999){
		//currentLayer=&layer[3];
	 	ps.move(startPos,0);
	   	pe.move(startPos,0);
	   	ps.rotate(textEntities.getAngle());
	        pe.rotate(textEntities.getAngle());
		ps.move(textEntities.getInsX(),textEntities.getInsY());
		pe.move(textEntities.getInsX(),textEntities.getInsY());
		myModel.appendLine(&ps,&pe);
		//MessageBox(comStr.getHWND(),"angle=0","INFO 1",MB_OK);
		return true;
	}

	float A=(1-cos_theta);
	float B=dot(T1,T0.ort())*dot(T,T0)/dot(T,T0.ort())+
		dot(T0,T1.ort())*dot(T,T1)/dot(T,T1.ort());
	float C=-0.5*dot(T0,T1.ort())*dot(T1,T0.ort())/(dot(T,T1.ort())*dot(T,T0.ort()));
	float D=B*B-4*A*C;
	float s1=(-B+sqrt(D))/(2*A);
	float s2=(-B-sqrt(D))/(2*A);
	float s;
	char buff[256];
	
	//***************************************

	if (s1>=0)
	{
		if (s1<=1)
			s=s1;
	}
	else if (s2>=0)
	{
		if (s2<=1)
			s=s2;
	}
	else
	{
		//sprintf(buff,"s1=%3.3f s2=%3.3f a=%3.3f",s1,s2,a);
		sprintf(buff,"s1=%3.3f s2=%3.3f B0(%3.3f,%3.3f)\
				B1(%3.3f,%3.3f),B2(%3.3f,%3.3f) a=%3.3f"
				,s1,s2,B0.x,B0.y,B1.x,B1.y,B2.x,B2.y
				,cos_theta);
		MessageBox(comStr.getHWND(),buff,"INFO 1",MB_OK);
	}
	
	//***************************************
	
	float lam=s*d*dot(T,T1.ort())/dot(T0,T1.ort());
	float mu=s*d*dot(T,T0.ort())/dot(T1,T0.ort());

	vec2 G=B0+lam*(T0+T);
	A=d-2*a*dot(T0,T);
	B=2*a*dot(T0,T);
	C=-lam*(1+dot(T0,T));
	D=B*B-4*A*C;
	s1=(-B+sqrt(D))/(2*A);
	s2=(-B-sqrt(D))/(2*A);

	//***************************************
	if (s1>=0)
	{
		if (s1<=1)
			s=s1;
	}
	else if (s2>=0)
	{
		if (s2<=1)
			s=s2;
	}
	else
	{
		sprintf(buff,"s1=%3.3f s2=%3.3f B0(%3.3f,%3.3f)\
				B1(%3.3f,%3.3f),B2(%3.3f,%3.3f) a=%3.3f"
				,s1,s2,B0.x,B0.y,B1.x,B1.y,B2.x,B2.y
				,cos_theta);
		
	//	sprintf(buff,"s1=%3.3f s2=%3.3f a=%3.3f",s1,s2,a);
		MessageBox(comStr.getHWND(),buff,"INFO 2",MB_OK);
		//return true;
	}
	//***************************************

	float k=(2*a*s*(1-s)-lam)*sqrt(1-dot(T0,T)*dot(T0,T));
	float u=s;

	if (k>tau)
	{
		//currentLayer=&layer[0];
		vec2 L0=B0;
		float AA=B0.y-B2.y;
		float BB=B2.x-B0.x;
		float CC=B2.y*B0.x-B0.y*B2.x;
		vec2 L2=B0*(1-u)*(1-u)+B1*2*u*(1-u)+B2*u*u;
		float distance=fabs((AA*L2.x+BB*L2.y+CC)/sqrt(AA*AA+BB*BB));
		float dist=distance/(sqrt(1-dot(T,T0)*dot(T,T0)));
		vec2 L1=B0+T0*dist;

		arcBez(L0,L1,L2,tau,startPos);

		//------R I G H T --------------------
		//currentLayer=&layer[1];
		distance=fabs((AA*L2.x+BB*L2.y+CC)/sqrt(AA*AA+BB*BB));
		dist=distance/(sqrt(1-dot(T,T1)*dot(T,T1)));
		vec2 R0=L2;
		vec2 R1=B2-dist*T1;
		vec2 R2=B2;
		arcBez(R0,R1,R2,tau,startPos);
	}
	else 
	{
	//-----construct BIARC-------------------------
	//
	//  LEFT ARC    RIGHT ARC
	//
	//           B1    
	//            *
	//
	//  L1*-----**G**------R1 
	//    |  *         *  |
	//    |*  \R1   R2/  *|
	//  B0*    \     /    *B2 R2   
	//          c1 c2
	//  
	//  construct norm N1_0 for B1-B0
	//  construct norm N1_G for L1-G
	//  calculate center as point intersect 
	//currentLayer=&layer[4];
	
	vec2 c1b0=b1b0.ort();
	vec2 c1G=b2b0.ort();
	vec2 b0G=G-B0;
	vec2 c1;
	float R;
	float k0;
	float kG;
	float xC,yC;
	float b0,b1;

	if (c1b0.x==0){ //C-b0 || OY
		
			xC=B0.x;
			kG=c1G.y/c1G.x;
			b1=G.y-kG*G.x;
			yC=kG*xC+b1;
			c1.x=xC;
			c1.y=yC;
			c1-=B0;
			R=c1.length();
			

	}
	else if (c1G.x==0){
		
			xC=G.x;
			k0=c1b0.y/c1b0.x;
			b0=B0.y-k0*B0.x;
			yC=k0*xC+b0;
			c1.x=xC;
			c1.y=yC;
			c1-=B0;
			R=c1.length();
			
	}
	else {
		k0=c1b0.y/c1b0.x;
		kG=c1G.y/c1G.x;
		if (kG==k0){
			R=b0G.length()/2;
			xC= (B0.x<G.x)?(B0.x+R):(B0.x-R);
			yC= (B0.y<G.y)?(B0.y+R):(B0.y-R);
		}
		else {
			b0=B0.y-k0*B0.x;
			b1=G.y-kG*G.x;
			xC=(b0-b1)/(kG-k0);
			yC=kG*xC+b1;
			c1.x=xC;
			c1.y=yC;
			c1-=B0;
			R=c1.length();
		}
	}

	arcDir=getArcDirection(B0,B1,B2);

	if (arcDir==ERROR)
	{
		Point pt1(B0.x,B0.y);
		Point pt3(B2.x,B2.y);
		myModel.appendLine(&pt1,&pt3);
		return true;
	}

/*
	Point pt1(B0.x,B0.y);
	Point pt2(B1.x,B1.y);
	Point pt3(B2.x,B2.y);
	currentLayer=&layer[0];
	myModel.appendLine(&pt1,&pt2);
	currentLayer=&layer[1];
	myModel.appendLine(&pt2,&pt3);
	currentLayer=&layer[3];
	myModel.appendLine(&pt1,&pt3);
	char buffer[256];

	sprintf(buffer,"B0[%3.3f,%3.3f],B1[%3.3f,%3.3f],B2[%3.3f,%3.3f] ",
			B0.x,B0.y,B1.x,B1.y,B2.x,B2.y);
	if (arcDir==AD_CLOCKWISE)
	{
		strcat(buffer,"CLOCKWISE");
	    comStr.addTextToHistory(buffer);
	}
	else if (arcDir==AD_COUNTERCLOCKWISE)
	{
		strcat(buffer,"COUNTERCLOCKWISE");
	    comStr.addTextToHistory(buffer);
	}
	else 
	    comStr.addTextToHistory("Error calc ROTATION");
*/

	pe=G;
	pcenter.setXY(xC,yC);

	ps.move(startPos,0);
	pe.move(startPos,0);
	pcenter.move(startPos,0);
	ps.rotate(textEntities.getAngle());
	pe.rotate(textEntities.getAngle());
	pcenter.rotate(textEntities.getAngle());
	ps.move(textEntities.getInsX(),textEntities.getInsY());
	pe.move(textEntities.getInsX(),textEntities.getInsY());
	pcenter.move(textEntities.getInsX(),textEntities.getInsY());
	
	myModel.appendArc(ps.getX(),ps.getY(),pe.getX(),pe.getY(),
			pcenter.getX(),pcenter.getY(),R,arcDir);

	//myModel.showModel();

//=============RIGHT PART BI ARC=====================
	//  LEFT ARC    RIGHT ARC
	//
	//           B1    
	//            *
	//
	//  L1*-----**G**------R1 
	//    |  *         *  |
	//    |*  \R1   R2/  *|
	//  B0*    \     /    *B2 R2   
	//          c1 c2
	//  
	//  construct norm N1_0 for B1-B0
	//  construct norm N1_G for L1-G
	//  calculate center as point intersect 

        vec2 c2b2=b2b1.ort();
	vec2 c2G=c1G;
	vec2 b2G=G-B2;
	vec2 c2;

	if (c2b2.x==0){
		
			xC=B2.x;
			kG=c1G.y/c1G.x;
			b1=G.y-kG*G.x;
			yC=kG*xC+b1;
			c1.x=xC;
			c1.y=yC;
			c1-=B2;
			R=c1.length();
		
	}
	else if (c1G.x==0){
		
			xC=G.x;
			k0=c1b0.y/c1b0.x;
			b0=B0.y-k0*B0.x;
			yC=k0*xC+b0;
			c1.x=xC;
			c1.y=yC;
			c1-=B2;
			R=c1.length();
		
	}
	else {
		k0=c2b2.y/c2b2.x;
		kG=c1G.y/c1G.x;
		if (kG==k0){
			R=b2G.length()/2;
			xC= (B2.x<G.x)?(B2.x+R):(B2.x-R);
			yC= (B2.y<G.y)?(B2.y+R):(B2.y-R);
		}
		else {
			b0=B2.y-k0*B2.x;
			b1=G.y-kG*G.x;
			xC=(b0-b1)/(kG-k0);
			yC=kG*xC+b1;
			c1.x=xC;
			c1.y=yC;
			c1-=B2;
			R=c1.length();
		}
	}


	//currentLayer=&layer[1];

	ps=G;
	pe=B2;
	pcenter.setXY(xC,yC);

	ps.move(startPos,0);
	pe.move(startPos,0);
	pcenter.move(startPos,0);
	ps.rotate(textEntities.getAngle());
	pe.rotate(textEntities.getAngle());
	pcenter.rotate(textEntities.getAngle());
	ps.move(textEntities.getInsX(),textEntities.getInsY());
	pe.move(textEntities.getInsX(),textEntities.getInsY());
	pcenter.move(textEntities.getInsX(),textEntities.getInsY());

	
	myModel.appendArc(ps.getX(),ps.getY(),pe.getX(),pe.getY(),
			pcenter.getX(),pcenter.getY(),R,arcDir);
	//myModel.showModel();

	}

	return true;
}

int getArcDirection(vec2 p1, vec2 p2, vec2 p3){

	int ArcDirection;
	vec2 p1p3=p3-p1;
	vec2 p1p2=p2-p1;
	float angle;
	float cosA;
	float cosB;
	cosA=p1p3.x/p1p3.length();
	cosB=p1p2.x/p1p2.length();

	if (cosA==cosB)
	{
		ArcDirection=ERROR;
		MessageBox(comStr.getHWND(),"ERROR:Point on one line","ArcDirection",MB_OK);
		return ArcDirection;
	}

	angle=acos(cosA);

	if (cosA==0) // p1p3||OY
	{
		if (p1.y>p3.y){
			if (p2.x>p1.x)
			{
				ArcDirection=AD_COUNTERCLOCKWISE;
			}
			else 
			{
				ArcDirection=AD_CLOCKWISE;
			}
		}
		else{
			if (p2.x>p1.x)
			{
				ArcDirection=AD_CLOCKWISE;
			}
			else 
			{
				ArcDirection=AD_COUNTERCLOCKWISE;
			}
		}
	return ArcDirection;
	}


	if (p1p3.y<0&&p1p3.x<0)
		angle=2*PI-angle;
	else if (p1p3.y<0&&p1p3.x>0)
		angle=2*PI-angle;

	p2-=p1;
	p2.rotate(-angle);

	if (p2.y<0)
	{
		ArcDirection=AD_CLOCKWISE;
	}
	else if (p2.y==0){
		ArcDirection=ERROR;
		MessageBox(comStr.getHWND(),"ERROR:Y=0","ArcDirection",MB_OK);
	}
	else
	{
		ArcDirection=AD_COUNTERCLOCKWISE;
	}
	return ArcDirection;
}
