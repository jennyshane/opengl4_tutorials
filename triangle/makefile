OBJS=triangle.o
CC=g++
CFLAGS=-std=c++0x -c
LFLAGS=-lGL -lGLEW -lSDL

triangle : $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -otriangle

triangle.o : triangle.cpp
	$(CC) $(CFLAGS) triangle.cpp
