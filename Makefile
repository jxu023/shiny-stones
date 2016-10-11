a: main.o
	g++ main.o -o main
main.o: main.cc Board.cc
	g++ -c main.cc -c Board.cc
clean:
	rm -rf *.o
run:
	./main "games/chb3046(10K)_ihasdcoffe(10K)_201610081428.sgf"
