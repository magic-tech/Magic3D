win32:QMAKE_CXXFLAGS += -std=c++11
win32:RC_FILE = icons.rc

TARGET = game
TEMPLATE = app
DESTDIR = ./game

OBJECTS_DIR = ./build
UI_DIR = ./build
MOC_DIR = ./build

CONFIG -= exceptions

LIBS += -L./game -lmagic3d_contrib
win32:LIBS += -mwindows -lwsock32 -lwinmm -lws2_32 -limm32 -lmingw32 -lopengl32 -lOpenAL32 -lSDL2
unix:!macx:LIBS += -lopenal
macx:LIBS += -framework OpenAL

DEFINES += MAGIC3D_LOG

MAGIC3D = ./
CONTRIB = ./contrib

include($$CONTRIB/Magic3D_Base.pro)
include($$CONTRIB/Magic3D_Base_Include.pro)
include(Magic3D_Engine.pro)

SOURCES += \
    ./main.cpp

DISTFILES += \    
    game/config.xml \