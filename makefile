name=main.exe

CC            = gcc
CXX           = g++
DEFINES       = -DUNICODE -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB -DQT_HAVE_MMX -DQT_HAVE_3DNOW -DQT_HAVE_SSE -DQT_HAVE_MMXEXT -DQT_HAVE_SSE2 -DQT_THREAD_SUPPORT
CFLAGS        = -pipe -fno-keep-inline-dllexport -g -Wall -Wextra $(DEFINES)
CXXFLAGS      = -pipe -fno-keep-inline-dllexport -g -frtti -fexceptions -mthreads -Wall -Wextra $(DEFINES)
INCPATH       = -I'../../../qt-4.8.7/include/QtCore' -I'../../../qt-4.8.7/include/QtGui' -I'../../../qt-4.8.7/include' -I'../../../qt-4.8.7/include/ActiveQt' -I'debug' -I'../../../qt-4.8.7/mkspecs/win32-g++-4.6'
LINK        =        g++
LFLAGS        =        -mthreads -Wl,-subsystem,windows
LIBS        = -lglu32 -lopengl32 -lwinmm -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -lcomctl32


srcs=baseWindow.cpp main.cpp toolbardraw.cpp commandLine.cpp transform.cpp model.cpp modelWindow.cpp kdib.cpp

objs=$(srcs:.cpp=.o)

all:$(objs) resource.res.o
	g++ -g $(LFLAGS) -o $(name) $(objs) resource.res.o $(LIBS) 

.cpp.o:
	g++ -c -g  $(srcs)

resource.res.o:resource.res
	windres -J res -O coff -i resource.res -o resource.res.o 

resource.res:resource.rc resource.h
	windres -O res -i resource.rc -o resource.res 

main.o:main.cpp headerui.h wndclass.h kdib.h kdib.cpp
baseWindow.o:baseWindow.cpp headerui.h wndclass.h
toolbardraw.o:toolbardraw.cpp
commandLine.o:commandLine.cpp
transform.o:transform.cpp  
model.o:model.cpp  
modelWindow.o:modelWindow.cpp
kdib.cpp:kdib.h
