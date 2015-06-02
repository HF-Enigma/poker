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

DESTDIR = ./game/target

SOURCES += main.cpp \
    engine/card.cpp \
    engine/pokermath.cpp \
    engine/pokereval.cpp \
    ai/ai.cpp \
    engine/player.cpp \
    engine/game.cpp

HEADERS += \
    engine/card.h \
    engine/pokermath.h \
    engine/pokereval.h \
    ai/ai.h \
    engine/player.h \
    engine/game.h
