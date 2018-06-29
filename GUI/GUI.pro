#-------------------------------------------------
#
# Project created by QtCreator 2018-06-19T09:40:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    gantt.cpp \
    ../Algorithm/describeTable.cpp \
    ../Algorithm/gene.cpp \
    ../Algorithm/index.cpp \
    ../Algorithm/mutate.cpp \
    ../Utils/formatOutput.cpp \
    ../Utils/scanFormatInput.cpp \
    ../Algorithm/crossover.cpp \
    ../Algorithm/decode.cpp \
    ../Utils/repair.cpp

HEADERS += \
        mainwindow.h \
    gantt.h \
    ../Utils/formatOutput.h \
    ../Utils/scanFormatInput.h \
    ../Algorithm/crossover.h \
    ../Algorithm/decode.h \
    ../Algorithm/decribeTable.h \
    ../Algorithm/gene.h \
    ../Algorithm/index.h \
    ../Algorithm/mutate.h \
    ../Utils/repair.h

FORMS += \
        mainwindow.ui
