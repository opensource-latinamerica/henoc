TEMPLATE = lib
CONFIG += staticlib
QT -= gui core

HEADERS += \
    henocUniverse.h \
    vector.h \
    shapes.h \
    intersection.h \
    enums.h \
    aabb.h

SOURCES += \
    source/henocUniverse.cpp \
    source/vector.cpp \
    source/shapes.cpp \
    source/intersection.cpp \
    source/circle-circle.cpp \
    source/quad-quad.cpp \
    source/quad-circle.cpp \
    source/terrain-all.cpp \
    source/composite-all.cpp

# Link against the pre-built ODE library
INCLUDEPATH += ode_install/include
LIBS += -L$$PWD/ode_install/lib -lode

# Set the destination for object files
OBJECTS_DIR = .obj
MOC_DIR = .obj
