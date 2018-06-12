#Makefile for OpenGL 4 Projects

OBJS += build/main.o
OBJS += build/Shader.o
OBJS += build/Camera.o
OBJS += build/Model.o

SRCS += src/Shader.cpp
SRCS += src/Camera.cpp
SRCS += src/Model.cpp
SRCS += src/main.cpp

LDFLAGS += -lGL -lglfw -lGLEW -lassimp
CFLAGS += -std=c++11

main : objects
	g++ $(OBJS) $(LDFLAGS) -o $@

objects: $(SRCS)
	g++ -c $(SRCS) $(CFLAGS)
	mv *.o build/
