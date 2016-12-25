.SUFFIXES: .c .o
CC = g++
EXEC = raytracer
CCFLAGS = -g -Wall
OBJS = Main.o Triangle.o RayHit.o Sphere.o

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -lm -o ${EXEC} ${OBJS}

.c.o:
	${CC} -c $<

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${EXEC} ${OBJS}

Main.o: Main.cpp 
Triangle.o: Triangle.cpp
RayHit.o : RayHit.cpp
Sphere.o : Sphere.cpp