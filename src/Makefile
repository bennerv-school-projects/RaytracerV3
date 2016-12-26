.SUFFIXES: .cpp .o
CC = g++
EXEC = raytracer
CCFLAGS = -g -Wall -std=c++11
OBJS = Main.o Triangle.o RayHit.o Sphere.o

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -lm -o ${EXEC} ${OBJS}

.cpp.o:
	${CC} -c $<

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${EXEC} ${OBJS}

tests:

Main.o : Main.cpp 
Triangle.o : Triangle.cpp
RayHit.o : RayHit.cpp
Sphere.o : Sphere.cpp