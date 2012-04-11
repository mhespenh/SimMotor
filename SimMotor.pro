# -------------------------------------------------
# Project created by QtCreator 2012-03-21T18:12:32
# -------------------------------------------------
QT += core
QT -= gui
TARGET = SimMotor
CONFIG += console
CONFIG += qdbus
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    Motor.cpp \
    aiobject.cpp
HEADERS += Motor.h \
    aiobject.h
OTHER_FILES += README.txt \
    README
