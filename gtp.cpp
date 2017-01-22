#include <vector>
#include <string>
#include <iostream>
#include "gtp.h"
#include "Board.cpp" // FIX THIS PLEASE, WHY DOESNT VISUAL STUDIO LET ME NOT HAVE THIS

using namespace std;

Board b;

bool success(string s) {
	cout << "= " << s << "\n\n";
	return true;
}

bool gtp_name(vector<string> args) {
	return success("o_o go");
}
bool gtp_version(vector<string> args) {
	return success("0");
}
bool gtp_protocol_version(vector<string> args) {
	return  success("2");
}
bool gtp_list_commands(vector<string> args);
bool gtp_clear_board(vector<string> args) {
	b.init();
	return success("");
}
bool gtp_boardsize(vector<string> args) {
	int n = stoi(args.at(1));
	b.boardsize(n);
	return success("");
}
bool gtp_komi(vector<string> args) {
	double n = stod(args.at(1));
	b.set_komi(n);
	return success("");
}
// this might return false =_=
// compare with gnugo
bool gtp_play(vector<string> args) {
	Value v = Black;
	if (args[1] != "B") {
		v = White;
	}
	int col = 20 - ((int)(args[2][0] - 'A') + 1);
	if (args[2][0] > 'T') {
		col = col - 1;
	}
	int row = stoi(args[2].substr(1, 2));
	b.move(v, row, col);
	return success("");
}
bool gtp_genmove(vector<string> args) {
	Value v = Black;
	if (args[1] != "B") {
		v = White;
	}
	Move m = b.genmove(v);
	b.move(m.v,m.row,m.col);
	return success(m.gtp_vertex());
}
bool gtp_list_commands(vector<string> args) {
	string s = "";
	for (auto a : commands) {
		s = s + a.cmd + '\n';
	}
	s.erase(s.length() - 1, 1);
	return success(s);
}
void gtp_loop() {
	while (true) {
		string line;
		getline(cin, line);
		unsigned int i = 0;
		vector<string> args;
		while (i < line.length()) {
			while (i < line.length() && isspace(line[i])) { ++i; }
			string word = "";
			while (i < line.length() && !isspace(line[i])) {
				word = word + line[i++];
			}
			args.push_back(word);
		}
		for (auto a : commands) {
			if (a.cmd == args.at(0)) {
				(*a.fn)(args);
				break;
			}
		}
	}
}
