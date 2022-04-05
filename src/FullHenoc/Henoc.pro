#
# /usr/include/malloc/ --> directorio necesario para compilar en MacOSX
# en linux esta directamente en /usr/include
#

INCLUDEPATH += ../HenocUniverse/ /usr/include/malloc/
LIBS += -L. -L../HenocUniverse/ -lHenocUniverse
CONFIG += warn_off static release
FORMS += frmHenoc.ui \
	frmPeCaLi.ui \
	frmMundo.ui
SOURCES += main.cpp \
	diagramscene.cpp\
	HObject.cpp\
	HLine.cpp\
	HBox.cpp\
	HBall.cpp\
	glwidget.cpp \
	CfrmPeCaLi.cpp \
	CfrmMundo.cpp \
	CfrmHenoc.cpp
HEADERS += CfrmHenoc.h\
	HObject.h\
	HBox.h\
	HLine.h\
	HBall.h\
	glwidget.h \
	CfrmPeCaLi.h \
	CfrmMundo.h \
	diagramscene.h
RESOURCES += diagramscene.qrc

DESTDIR += ../

QT           += opengl xml
