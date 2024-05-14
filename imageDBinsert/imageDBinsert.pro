
QT       += core
QT       += gui
QT       += sql
 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
 
TARGET = DataBase
TEMPLATE = app
 
 
SOURCES += main.cpp\
        mainwindow.cpp \
        database.cpp
 
HEADERS  += mainwindow.h \
    database.h
 
FORMS    += mainwindow.ui
