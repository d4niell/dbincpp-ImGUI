#pragma once
#include <unordered_map>

class c_globals {
public:
	bool active = true;
	bool registerwindow = false;
	bool userpanel = false;
	char user_name[100] = "admin";
	char pass_word[100] = "pass";
	bool dbCreated = false;
	bool marketplace = false;
	bool messages = false;
	bool ATM = false;
	bool log_out = false;
	char user_item[100];
	char user_item_price[100];
	std::string item;
	bool inventory = false;
	bool user_list = false;
	char modify_username[100];
	char modify_password[100];
	bool appareance = false;
	char modify_cash[100];
	bool modify_user = false;
	char user_message[100];
	bool messageActive = false;
	bool generate_item = false;
	bool sell_item = false;
	std::string user_message_name;
	bool roulette = false;
};
struct {
public:


	std::string u_name;
	std::string u_pass;
	std::string marketplace_item_name;
	std::string marketplace_item_price;
	std::string user_uid;
	std::string user_modify_name;
	std::string user_modify_cash;
	std::string user_modify_uid;
	std::string user_modify_password;
	std::string s_username;
	int total_logins;
	std::string s_uid;
	int user_cash;
}user;
struct {
public:
	int listed_item_price;
	std::string listed_item_name;
	std::string listed_item_owner;
	int owner_money;
}market;
inline c_globals globals;