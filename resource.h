#if !defined(RESOURCE)
#define RESOURCE

#define ID_SEP			 100
#define IDC_TBCOMBO_LAYER    	 200
#define IDC_TBCOMBO_COLOR    	 201
#define IDC_TBCOMBO_TYPE    	 202
#define IDC_TBCOMBO_WIDTH	 203

//-----меню ФАЙЛ-----------------------
#define MYMENU			1000
#define IDM_NEW 		1010
#define IDM_OPEN 		1020
#define IDM_SAVE 		1030
#define IDM_SAVEAS		1040
#define IDM_EXIT		1090
#define IDM_TEST		1900
//-----меню ПРАВКА-----------------------
#define IDM_UNDO                2000
#define IDM_REDO                2010
#define IDM_CUT                 2030
#define IDM_COPY                2040
#define IDM_PASTE               2050
#define IDM_SELECTALL           2070
#define IDM_DELETE              2080
//-----меню ВИД--------------------------
#define IDM_REGEN		3000
#define IDM_REDRAW              3010
#define IDM_TBDRAW              3020
#define IDM_TBTRANSFORM         3030
#define IDM_STATUSBAR           3040
#define IDM_WNDPROPERTIES       3050
#define IDM_WNDCOMMAND          3060
#define IDM_SCALEINC            3070
#define IDM_SCALEDEC            3080
#define IDM_SCALEMOUSE          3090
#define IDM_ZOOMALL             3100
#define IDM_PAN                 3110
#define IDM_UCSONOFF	        3120
//-----меню ВСТАВИТЬ--------------------------
#define IDM_INSERTBMP		4000
#define IDM_SUBSTRATE		4010
//-----меню ПРИМИТИВ--------------------------
#define IDM_LINE  			5000
#define IDM_ARC3POINTS			5010
#define IDM_ARCSTARTENDRADIUS		5020
#define IDM_ARCCENTREANG1ANG2          5030
#define IDM_CIRCCENTRRAD               5040
#define IDM_CIRCCENTRDIAM              5050
#define IDM_CIRC3POINTS                5060
#define IDM_CONTUR			5061
#define IDM_RECTANGLE			5062
#define IDM_POLYGON                    5070
#define IDM_TEXT                       5080
//-----меню ПРЕОБРАЗОВАТЬ----------------------
#define IDM_ERASE			6000
#define IDM_PCOPY                       6010
#define IDM_MIRROR                      6020
#define IDM_OFFSET                      6030
#define IDM_ARRAY                       6040
#define IDM_MOVE                        6050
#define IDM_ROTATE                     	6060
#define IDM_SCALE              	     	6070
#define IDM_LENGTH     	             	6080
#define IDM_TRIM                    	6090
#define IDM_CHAMFER                     6100
#define IDM_FILLET                      6110
//-----меню ФОРМАТ----------------------------
#define IDM_LAYER			7000
#define IDM_LINECOLOR                   7010
#define IDM_LINEWEIGHT                  7020
#define IDM_LINETYPE                    7030

#define IDM_HELPABOUT			7040
#define IDM_HELPINFO			7050
//-----меню НАСТРОЙКИ--------------------------
#define IDM_BACKGROUND			8000
#define IDM_UCS                         8010

#define IDR_TOOLBAR1	9000
#define IDR_TOOLBAR2	9100
#define IDS_NEW		10000
#define IDS_NEW2	10100

#define EXAMPLEDIALOG		11000
#define IDC_CHECKBOX		11100
#define IDC_RADIO1		11200
#define IDC_RADIO2		11300

//----курсор и иконка------------------
#define IDC_CROSSPTR		11400
#define IDI_ICONMIN2C		11500
#define IDI_ICONMIN3C		11600


//----------─Диалоговое окно Растровое изображение ---------
#define IDD_PICDIALOG1                  12000
#define IDC_PICCOMBOPATH                12100
#define IDC_PICBUTTONPATH               12200
#define IDC_PICBUTTONHELP               12300
#define IDC_PICNAME                     12400
#define IDC_PICSTATICINSERTGROUP        12500
#define IDC_PICSTATICSCALEGROUP         12600
#define IDC_PICSTATICROTATEGROUP        12700
#define IDC_PICCHECKSCALE               12800
#define IDC_PICCHECKINSPOINT            12900
#define IDC_PICCHECKROTATE              13000
#define IDC_PICEDITX                    13100
#define IDC_PICEDITY                    13200
#define IDC_PICEDITSCALE                13300
#define IDC_PICSTATICX                  13400
#define IDC_PICSTATICY                  13500
#define IDC_PICEDITANGLE                13600
#define IDC_PICSTATICPATH               13700

//----------─Диалоговое окно Помощь ---------
#define IDD_HELPDIALOG1                 14000
#define IDC_HELPABOUTINFO               14100
#define IDC_HELPABOUT2                  14200
#define IDC_HELPABOUT3                  14300

//----------Диалоговое окно Ввод текста------
#define IDHLPFONT                       15010
#define IDD_DIALOGTEXT                  15000
#define IDC_TEXTINPUT                   15020
#define IDC_TEXT_INSPOINTX              15030
#define IDC_TEXT_INSPOINTY              15040
#define IDC_TEXT_ANGLE                  15050
#define IDC_TEXT_BUTTON_FONT            15060
#define IDC_TEXT_BUTTONLENSTR           15070
#define IDC_TEXT_BUTTON_INSPOINT        15080
#define IDC_TEXT_LEN                    15090
#define IDC_TEXT_BUTTONANGLE            15100
#define IDC_TEXT_APPROXLINEAR           15110
#define IDC_TEXT_APPROXBIARC            15120
#define IDC_TEXT_NUMBLINES              15130
#define IDC_TEXT_TOLERANCE              15140
#define IDC_TEXTFONTNAME                15150
#define IDC_TEXT_INSPOINTGROUP          15160
#define IDC_TEXT_CAPINSPOINTX           15170
#define IDC_TEXT_CAPINSPOINTY           15180
#define IDC_TEXT_DIRANGLEGROUP          15190
#define IDC_TEXT_CAPANGLE               15200
#define IDC_TEXT_LENSTRGROUP            15210
#define IDC_TEXT_LENCAP                 15220
#define IDC_TEXT_APPROXGROUP            15230
#define IDC_TEXT_NUMLINECAP             15240
#define IDC_TEXT_TOLERANCECAP           15250


#endif

