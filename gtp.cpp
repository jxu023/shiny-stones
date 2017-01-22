#include "gtp.h"
#include "Board.cpp" //  =/, why doesn't it work without this?

using namespace std;
Board * myboard = new Board(); // pointer was needed b/c compiler thinks i'm declaring Board again ...

bool success(string s) {
	cout << "= " << s << "\n\n";
	return true;
}
bool failure(string s) {
	cout << "? " << s << "\n\n";
	return false;
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
	myboard->init();
	return success("");
}
bool gtp_boardsize(vector<string> args) {
	int n = stoi(args.at(1));
	myboard->boardsize(n);
	return success("");
}
bool gtp_komi(vector<string> args) {
	double n = stod(args.at(1));
	myboard->set_komi(n);
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
	if (myboard->move(v, row, col))
		return success("");
	return failure("move invalid");
}
bool gtp_genmove(vector<string> args) {
	Value v = Black;
	if (args[1] != "B") {
		v = White;
	}
	Move m = myboard->genmove(v);
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
