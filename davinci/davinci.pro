QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    circle.cpp \
    convexpolygon.cpp \
    distance.cpp \
    hexagon.cpp \
    main.cpp \
    paint.cpp \
    pentagon.cpp \
    rectangle.cpp \
    rotate.cpp \
    shape.cpp \
    triangle.cpp

HEADERS += \
    circle.h \
    convexpolygon.h \
    distance.h \
    hexagon.h \
    paint.h \
    pentagon.h \
    rectangle.h \
    rotate.h \
    shape.h \
    triangle.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
