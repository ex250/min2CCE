name=main.exe

lib32 = user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib\
 opengl32.lib glu32.lib

srcs=baseWindow.cpp main.cpp toolbardraw.cpp commandLine.cpp transform.cpp model.cpp modelWindow.cpp

objs=$(srcs:.cpp=.obj)

all:$(objs) resource.res
 link -SUBSYSTEM:windows -OUT:$(name) $(objs) resource.res -DEFAULTLIB:$(lib32)

.cpp.obj:
 cl -c -GX $(srcs)

resource.res:resource.rc resource.h
 rc resource.rc

main.obj:baseWindow.cpp headerui.h wndclass.h toolbardraw.cpp commandLine.cpp
baseWindow.obj:baseWindow.cpp headerui.h wndclass.h