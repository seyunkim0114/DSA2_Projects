main.exe: main.o graph.o heap.o hash.o
	g++ -std=c++11 -o main.exe main.o graph.o heap.o hash.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

graph.o: graph.cpp graph.h
	g++ -std=c++11 -c graph.cpp

heap.o: heap.cpp heap.h
	g++ -std=c++11 -c heap.cpp

hash.o: hash.cpp hash.h
	g++ -std=c++11 -c hash.cpp

debug:
	g++ -std=c++11 -g -o mainDebug.exe main.cpp graph.cpp heap.cpp hash.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
