CFLAGS = -pedantic -Wall -g -std=c++11
a: main.o
	g++ main.o -o main $(CFLAGS)
main.o: main.cpp Board.cpp
	g++ -c main.cpp -c Board.cpp $(CFLAGS)
clean:
	rm -rf *.o
run:
	./main "games/chb3046(10K)_ihasdcoffe(10K)_201610081428.sgf"
gtp: gtp.cpp
	g++ gtp.cpp -o gtp
gtprun:
	./gtp.exe
