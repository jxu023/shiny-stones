a: main.o
	g++ main.o -o main -g
main.o: main.cc
	g++ -c main.cc
clean:
	rm -rf main.o
run:
	./main "games/chb3046(10K)_ihasdcoffe(10K)_201610081428.sgf"
