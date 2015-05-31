#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T10:24:39
#
#-------------------------------------------------

QT       -= core gui

TARGET = game
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = ./game/target

SOURCES += main.cpp \
    engine/card.cpp \
    engine/pokermath.cpp \
    engine/combination.cpp \
    engine/pokereval.cpp

HEADERS += \
    engine/card.h \
    engine/pokermath.h \
    engine/combination.h \
    engine/pokereval.h
