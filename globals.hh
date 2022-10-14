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
	bool messages = false;
	bool ATM = false;
	bool log_out = false;
	char user_item[100];
	char user_item_price[100];
	char item[100];
	bool inventory = false;
	bool user_list = false;
	char modify_username[100];
	char modify_password[100];
	char modify_cash[100];
	bool modify_user = false;
};
inline c_globals globals;