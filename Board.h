#pragma once

#include "Move.h"

class Board {
private:
	Value * board;
	int Rows;
	int Cols;
	bool * checked;
	int ko_location;
	int Bcaptures;
	int Wcaptures;
	double komi;
public:
	Board(int r, int c);
	Board();
	Board(char * sgf_file);
	void init();
	void boardsize(int n);
	void set_komi(double k);
	Move genmove(Value v);
	// could make this method private and inline
	void clear_checked();
	// could make this method private and inline
	char read(Value v);
	// maybe should make use of group
	// apply group, then apply liberts
	// ex : for each coord, check 4 directions for space
	// well separately is probably more optimized
	int liberties(int r, int c);
	// could make this method private and inline
	Value other(Value v);
	// could make this method private and inline
	bool player(Value v);
	// check rules, no suicide, respect ko, implement captures
	// within bounds, correct turn orrder
	// make return bool to express success
	bool move(Value v, int r, int c);
	bool move(Move);
	void print();
	bool valid(Move);
	~Board();
};
