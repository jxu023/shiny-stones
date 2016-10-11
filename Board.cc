#include <stdio.h>
#include <stdlib.h>

const int dr[4] = {-1,0,0,1};
const int dc[4] = {0,1,-1,0};
struct Stack {
	int stk[361];
	int index;
	Stack() {
		index = 0;
	}
	void push(int c) {
		stk[index++] = c;
	}
	int pop() {
		return stk[--index];
	}
	bool empty() {
		return index == 0;
	}
};

// for serious computation, need an efficient encoding
// and a way to manipulate encoding efficiently
// like matrix multiplication or bit operations or something
enum Value {
	Wall, Empty, Black, White
};

class Board {
private:
	Value * board;
	int Rows;
	int Cols;
	bool * checked;
	int ko_location;
	int Bcaptures;
	int Wcaptures;
public:
	Board(int r, int c) : Rows(r+2), Cols(c+2) {
		init();
	}
	Board() {
		Rows = 21; Cols = 21;
		init();
	}
	Board(char * sgf_file) {
		Rows = 21; Cols = 21;
		init();
		FILE * fp;
		if ((fp = fopen(sgf_file,"r")) == NULL) {
			printf("file not found");
			exit(1);
		}
		char c;
		int state = 3;
		Value v;
		int col;
		while((c = getc(fp)) != EOF) {
			switch(state) {
				case 0: // Look for Moves
					if (c == 'B') {
						state = 1;
						v = Black;
					}
					else if (c == 'W') {
						state = 1;
						v = White;
					} break;
				case 1: // Look for x coordinate aka column
					if (c >= 'a' && c < 'z') {
						col = (int)(c - 'a') + 1;
						state = 2;
					} break;
				case 2: // Look for y coordinate aka row
					if (c >= 'a' && c < 'z') {
						int row = (int)(c - 'a') + 1;
						move(v,row,col);
						state = 0;
					} break;
				case 3: // should be looking for '(' to start adding moves
						// and looking for labels at the same time
					if (c == '(') {
						state = 0;
					} else if (c >= 'A' || c <= 'Z') {
						
					}break;
			}
		}
		fclose(fp);
	}
	void init() {
		Bcaptures = Wcaptures = 0;
		ko_location = -1;
		checked = (bool *)calloc(Rows*Cols,sizeof(Value));
		board = (Value *)calloc(Rows*Cols,sizeof(Value));
		for (int i = 1; i < Rows-1; ++i) {
			for (int j = 1; j < Cols-1; ++j) {
				board[i*Cols + j] = Empty;
			}
		}
	}
	// could make this method private and inline
	void clear_checked() {
		for (int i = 1; i < Rows - 1; ++i) {
			for (int j = 1; j < Cols - 1; ++j) {
				checked[i*Cols+j] = false;
			}
		}
	}
	// could make this method private and inline
	char read(Value v) {
		switch (v) {
			case Wall:
				return '|';
			case Empty:
				return '-';
			case Black:
				return 'b';
			case White:
				return 'w';
		}
	}
	// maybe should make use of group
	// apply group, then apply liberts
	// ex : for each coord, check 4 directions for space
	// well separately is probably more optimized
	int liberties(int r, int c) {
		Value v = board[r*Cols + c];
		int breaths = 0;
		clear_checked();
		Stack stk; stk.push(r*Cols+c);
		int current;
		int row, col;
		while (!stk.empty()) {
			current = stk.pop();
			checked[current] = true;
			row = current / Cols;
			col = current % Cols;
			for (int i = 0; i < 4; ++i) {
				int neighbor = (row+dr[i])*Cols + col+dc[i];
				if (!checked[neighbor]) {
					if (board[neighbor] == v) {
						stk.push(neighbor);
					} else if (board[neighbor] == Empty) {
						++breaths;
						checked[neighbor] = true;
					}
				}
			}
		}
		return breaths;
	}
	// could make this method private and inline
	Value other(Value v) {
		return v == Black ? White : Black;
	}
	// could make this method private and inline
	bool player(Value v) {
		return v == Black || v == White;
	}
	void editMove(Value v, int r, int c) {
		board[r*Cols+c] = v;
	}
	// check rules, no suicide, respect ko, implement captures
	// within bounds, correct turn orrder
	// make return bool to express success
	bool move(Value v, int r, int c) {
		int rc_ind = r*Cols+c;
		if (!player(v) || board[rc_ind] != Empty || rc_ind == ko_location) {
			return false;
		}
		Value orig = board[rc_ind];
		board[rc_ind] = v;
		int captures = 0;
		int captureIndex = -1;
		for (int i = 0; i < 4; ++i) {
			if (player(board[(r+dr[i])*Cols+c+dc[i]]) && 0 == liberties(r+dr[i],c+dc[i])) {
				// should maybe just maintain a list of coords isntead
				// that means have both a checked array and a group list?
				for (int row; row < Rows-1; ++row) {
					for (int col; col < Cols-1; ++col) {
						int index = row*Cols+col;
						if (checked[index] && board[index] == other(v))  {
							board[index] = Empty;
							++captures;
							captureIndex = index;
						}
					}
				}
			}
		}
		int temp = ko_location;
		ko_location = -1;
		// is there a way to make this evaluated efficiently?
		// upon finding 1 liberty, terminate the liberty method ...
		// could just create a separate method
		if (captures == 0 && liberties(r,c) == 0) {
			board[r,c] = orig;
			ko_location = temp;
			return false;
		}
		else if (captures == 1 && liberties(r,c) == 1) {
			ko_location = captureIndex;
		}
		if (captures > 0) {
			if (other(v) == White) {
				++Bcaptures;
			}
			else {
				++Wcaptures;
			}
		}
		return true;
	}
	void print() {
		printf("\n");
		for (int i = 0; i < Rows; ++i) {
			for (int j = 0; j < Cols; ++j) {
				printf("%c ",read(board[i*Cols + j]));
			}
			printf("\n");
		}
		printf("\n");
	}
	~Board() {
		free(board);
		free(checked);
	}
};

//todo:
	// load tsumego
		// try solving them
	// score estimate
	// score counting
	// review udacity cs271
		// heuristic and search stuff
		// maybe probablilty things too
	// integration to boardbuddha
	// efficiency optimizations
	// make more modular for easier maintenance and updating?
// combinatorial way to create test cases?
// make tests for:
	// Ko, capture, liberty counting, grouping...

// try out persistent tracking for groups
