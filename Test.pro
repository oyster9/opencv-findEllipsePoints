QT += core
QT -= gui

TARGET = Test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    imageprocess.cpp

INCLUDEPATH +=  D:\ProgramFiles\OpencvQt\include \
                D:\ProgramFiles\OpencvQt\include\opencv \
                D:\ProgramFiles\OpencvQt\include\opencv2 \

LIBS += D:\ProgramFiles\OpencvQt\lib\libopencv_*.dll.a

HEADERS += \
    imageprocess.h

