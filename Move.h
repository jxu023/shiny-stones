#pragma once
#include <string>
enum Value {
	Wall, Empty, Black, White
};

// need to add the ability to pass, and request counting
// do that by having negative coords perhaps, row -1 pass, row -2 count, row -3 resign
struct Move {
	Value v;
	int row;
	int col;
	Move(Value v, int r, int c) : v(v), row(r), col(c) { }
	std::string gtp_vertex() {
		char letter = 'A' + col - 1;
		if (letter >= 'T') {
			letter = letter + 1;
		}
		std::string ret = letter + std::to_string(row);
		return ret;
	}

};
