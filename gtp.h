#pragma once
#include "Board.h"
#include <vector>

using namespace std;

struct gtp_command {
	string cmd;
	bool(*fn)(vector<string> args);
};
bool success(string s);
bool failure(string s);
bool gtp_name(vector<string> args);
bool gtp_version(vector<string> args);
bool gtp_protocol_version(vector<string> args);
bool gtp_list_commands(vector<string> args);
bool gtp_clear_board(vector<string> args);
bool gtp_boardsize(vector<string> args);
bool gtp_komi(vector<string> args);
bool gtp_play(vector<string> args);
bool gtp_genmove(vector<string> args);
bool gtp_list_commands(vector<string> args);
void gtp_loop();
static struct gtp_command commands[] = {
	{"name", gtp_name},
	{"version", gtp_version},
	{"protocol_version", gtp_protocol_version},
	{"clear_board", gtp_clear_board},
	{"komi", gtp_komi},
	{"play", gtp_play},
	{"genmove", gtp_genmove},
	{"list_commands", gtp_list_commands},
	{"boardsize", gtp_boardsize},
};
