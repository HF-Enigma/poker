#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T10:24:39
#
#-------------------------------------------------

QT       -= core gui

TARGET = game
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = ../game/works/target

SOURCES += main.cpp \
    ai/ai.cpp \
    engine/card.cpp \
    engine/pokermath.cpp \
    engine/pokereval.cpp \
    engine/player.cpp \
    engine/game.cpp

HEADERS += \
    ai/ai.h \
    engine/card.h \
    engine/pokermath.h \
    engine/pokereval.h \
    engine/player.h \
    engine/game.h
