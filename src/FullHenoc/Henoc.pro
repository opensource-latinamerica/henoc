#
# /usr/include/malloc/ --> directorio necesario para compilar en MacOSX
# en linux esta directamente en /usr/include
#

DEFINES += dNODEBUG
INCLUDEPATH += ../HenocUniverse/ ../HenocUniverse/ode_install/include /usr/include/malloc/
LIBS += -L../HenocUniverse -lHenocUniverse -L../HenocUniverse/ode_install/lib -lode
CONFIG += warn_off static release c++11
FORMS += frmHenoc.ui \
	frmPeCaLi.ui \
	frmMundo.ui
SOURCES += main.cpp \
	diagramscene.cpp\
	HObject.cpp\
	HLine.cpp\
	HBox.cpp\
	HBall.cpp \
	CfrmPeCaLi.cpp \
	CfrmMundo.cpp \
	CfrmHenoc.cpp
HEADERS += CfrmHenoc.h\
	HObject.h\
	HBox.h\
	HLine.h\
	HBall.h\
	CfrmPeCaLi.h \
	CfrmMundo.h \
	diagramscene.h \
    henocUniverseI.h
RESOURCES += diagramscene.qrc

DESTDIR += ../
OBJECTS_DIR = .obj
MOC_DIR = .obj

QT           += opengl widgets xml
