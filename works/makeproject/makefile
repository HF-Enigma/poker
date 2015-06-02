#############################################################################
# Makefile for building: ../target/game
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
CFLAGS        = -pipe -O2 -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong --param=ssp-buffer-size=4 -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong --param=ssp-buffer-size=4 -Wall -W -fPIC $(DEFINES)
INCPATH       = -I../source -I. -I/usr/include
LINK          = g++
LFLAGS        = -Wl,-O1 -Wl,-O1,--sort-common,--as-needed,-z,relro
LIBS          = $(SUBLIBS) -lpthread 

####### Output directory

OBJECTS_DIR   = ./object

####### Files

SOURCES       = ../source/main.cpp \
		../source/ai/ai.cpp \
		../source/engine/card.cpp \
		../source/engine/pokermath.cpp \
		../source/engine/pokereval.cpp \
		../source/engine/player.cpp \
		../source/engine/game.cpp 
OBJECTS       = main.o \
		ai.o \
		card.o \
		pokermath.o \
		pokereval.o \
		player.o \
		game.o

DESTDIR       = ../target/#avoid trailing-slash linebreak
TARGET        = ../target/game


####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET):  $(OBJECTS)  
	@test -d ../target/ || mkdir -p ../target/
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

####### Compile

main.o: ../source/main.cpp ../source/engine/game.h \
		../source/engine/player.h \
		../source/engine/card.h \
		../source/engine/pokereval.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o ../source/main.cpp

ai.o: ../source/ai/ai.cpp ../source/ai/ai.h \
		../source/engine/player.h \
		../source/engine/card.h \
		../source/engine/game.h \
		../source/engine/pokermath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ai.o ../source/ai/ai.cpp

card.o: ../source/engine/card.cpp ../source/engine/card.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o card.o ../source/engine/card.cpp

pokermath.o: ../source/engine/pokermath.cpp ../source/engine/pokermath.h \
		../source/engine/card.h \
		../source/engine/pokereval.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o pokermath.o ../source/engine/pokermath.cpp

pokereval.o: ../source/engine/pokereval.cpp ../source/engine/pokereval.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o pokereval.o ../source/engine/pokereval.cpp

player.o: ../source/engine/player.cpp ../source/engine/player.h \
		../source/engine/card.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o player.o ../source/engine/player.cpp

game.o: ../source/engine/game.cpp ../source/engine/game.h \
		../source/engine/player.h \
		../source/engine/card.h \
		../source/ai/ai.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o game.o ../source/engine/game.cpp

.PHONY : clean
clean :
	-rm -f $(OBJECTS)
