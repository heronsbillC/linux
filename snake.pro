######################################################################
# Automatically generated by qmake (3.1) Mon Dec 27 22:54:26 2021
######################################################################

TEMPLATE = app
TARGET = snake
INCLUDEPATH += .

QT          += core gui
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
CONFIG      += c++11

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += snake.h
FORMS += snake.ui
SOURCES += main.cpp snake.cpp
RESOURCES += images.qrc myImage.qrc img/bg.qrc
