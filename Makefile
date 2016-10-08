CC = g++
CFLAGS = -std=c++11

all: main.o System.o
	$(CC) $(CFLAGS) main.o System.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

System.o: System.cpp System.h
	$(CC) $(CFLAGS) -c System.cpp

test: test.cpp
	$(CC) $(CFLAGS) test.cpp -o test

py: pmain.cpp
	g++ -std=c++11 -fPIC -shared pmain.cpp System.cpp -o pmain.so -I/usr/include/python2.7/ -lpython2.7

clean:
	rm *.o main test
