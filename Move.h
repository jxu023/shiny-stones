#pragma once

#include <string>
enum Value {
	Wall, Empty, Black, White
};

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
