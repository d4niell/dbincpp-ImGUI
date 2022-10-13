#pragma once
#include <unordered_map>

class c_globals {
public:
	bool active = true;
	bool registerwindow = false;
	bool userpanel = false;
	char user_name[100];
	char pass_word[100];
	bool dbCreated = false;
	bool marketplace = false;
};

inline c_globals globals;