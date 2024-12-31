CC=g++
CFLAGS=-std=c++23 -O3 -Wall -Wextra -Wpedantic
APP=x

all:	x

x:	x.cc 
	${CC} x.cc -o ${APP} ${CFLAGS}