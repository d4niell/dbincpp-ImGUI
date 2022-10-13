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
};
static const char* template_items_names[] =
{
	"Banana", "Apple", "Cherry", "Watermelon", "Grapefruit", "Strawberry", "Mango",
	"Kiwi", "Orange", "Pineapple", "Blueberry", "Plum", "Coconut", "Pear", "Apricot"
};
inline c_globals globals;