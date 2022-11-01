#include "ui.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../sqlite/sqlite3.h"
#include <iostream>
#include <sstream>
#include <random>
#include <string>
#include <fstream>
#include <ctime>
void confirm_purchase();
void appareance();
static bool istrue = false;
static int fetch_logins(const char* s);
const char* dir = "c:\\Database.db";
static int check_for_sum(const char* s);
static bool buy_item = false;
bool send_message = false;
bool failed = false;
void TextCentered(std::string text);
static bool confirm_item_from_marketplace = false;
void DBlog(std::string prefix, std::string message) {
    if (user.user_uid == "1") {
        std::ofstream databaseLogs;
        databaseLogs.open("c://database_errors.txt", std::ios::app);
        if (databaseLogs.is_open()) {
            databaseLogs << "\n> error occured in [" << prefix << "] >> " << message << ";";
            databaseLogs.close();
        }
    }
}
void log(std::string message) {
    if (user.user_uid == "1") {
        std::ofstream databaseLogs;
        databaseLogs.open("c://dbincpp_logs.txt", std::ios::app);
        if (databaseLogs.is_open()) {
            databaseLogs << "\ndbincpp >> [" << message << "]";
            databaseLogs.close();
        }
    }
}
void funcLogs(std::string prefix, std::string message) {
    if (user.user_uid == "1") {
        std::ofstream databaseLogs;
        databaseLogs.open("c://dbincpp_function_logs.txt", std::ios::app);
        if (databaseLogs.is_open()) {
            databaseLogs << "\n> [" << prefix << "] >> " << message << ";";
            databaseLogs.close();
        }
    }
}
static int callback_login(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        if (i == 0)
        {

            if (argv[0] == user.u_name && argv[1] == user.u_pass) {
                globals.userpanel = true;
                MessageBeep(MB_OK);
                //  ui::userPanel();
            }
            // printf("name: %s price: $%s", argv[0], argv[1]);
        }
        else
            MessageBeep(MB_ICONERROR);
        return 0;

    }
    // printf("\n");
    return 0;
}
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
static int fetch_credentials(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_login, 0, &messageError);

    if (exit != SQLITE_OK) {
        DBlog("fetch_credentials", messageError);
        sqlite3_free(messageError);
    }
    else
        return(0);
}
static int Register(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) {
        DBlog("Register", messageError);
        sqlite3_free(messageError);
        return 1;
    }
    else
        return(0);
}
static int callback_uid(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        ImGui::Text("UID:");
        ImGui::SameLine();
        ImGui::Text(argv[0]);
        ImGui::SameLine();
        user.user_uid = argv[0];
    }
    return 0;
}
static int fetchuserUID(const char* s)
{
    std::string sql = "SELECT id FROM User WHERE username = '" + user.u_name + "';";
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_uid, 0, &messageError);

    if (exit != SQLITE_OK) {
        DBlog("fetchuserUID", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int callback_CASH(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        ImGui::Text(" CASH:");
        ImGui::SameLine();
        ImGui::Text(argv[0]);
        std::string abc = argv[0];
        user.user_cash = stoi(abc);
    }
    return 0;
}
static int callback_username(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    static float Alpha = 255;
    static bool tick = false;
    static float Speed = 1.0f;
    if (tick || Alpha >= 255) {
        tick = true;
        if (!(Alpha <= 0))
            Alpha -= Speed;
        else if (Alpha <= 0)
            tick ^= 1;


    }
    else if (!tick || Alpha != 255)
    {
        tick = false;
        if (!(Alpha >= 255))
            Alpha += Speed;
        else if (Alpha >= 255)
            tick ^= 1;
    }
    for (i = 0; i < argc; i++) {
        ImGui::Text("Username:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255, (int)Alpha), argv[0]);
        ImGui::SameLine();
        user.s_username = argv[0];
    }
    return 0;
}
static int fetchuserUsername(const char* s)
{
    std::string sql = "SELECT username FROM User WHERE username = '" + user.u_name + "';";
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_username, 0, &messageError);

    if (exit != SQLITE_OK) {
        DBlog("fetchuserUsername", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int callback_select_from_marketplace(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;    
    for (i = 0; i < argc - 2; i++) {
        ImGui::Selectable(argv[0]);
        
       
        /*ImGui::Text("item:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[0]);
        ImGui::SameLine();
        ImGui::Text("price:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[1]);
    */
    }
    if (ImGui::BeginPopupContextItem()) {
        ImGui::Text("this item costs $%s", argv[1]);
        if (ImGui::Button("buy")) {
            std::string abc = argv[1];
            market.listed_item_price = stoi(abc);
            market.listed_item_name = argv[0];
            market.listed_item_owner = argv[2];
            confirm_item_from_marketplace = true;
 
            //  check_for_sum(dir);
            ImGui::CloseCurrentPopup();

        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();

    }
    return 0;
}
static int select_from_marketplace(const char* s)
{
    std::string sql = "SELECT itemName, price, userID FROM Marketplace;";
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_select_from_marketplace, NULL, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("select_from_marketplace", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        return 0;
}

static int insert_into_marketplace(const char* s)
{
    std::string sql = "INSERT INTO Marketplace (userID, itemName, price) VALUES (" + user.user_uid + ", '" + user.marketplace_item_name + "', " + user.marketplace_item_price + "); ";
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    sql = "DELETE FROM Inventory WHERE item = '" + user.marketplace_item_name + "';";
    if (exit != SQLITE_OK) {
        DBlog("insert_into_marketplace", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("insert_into_marketplace", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        funcLogs("insert_into_marketplace", "item added successfully");
        MessageBeep(MB_OK);
        return 0;
}
static int admin_insert_into_marketplace(const char* s)
{
    std::string sql = "INSERT INTO Marketplace (userID, itemName, price) VALUES (" + user.user_uid + ", '" + user.marketplace_item_name + "', " + user.marketplace_item_price + "); ";
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        MessageBeep(MB_ICONERROR);
        DBlog("admin_insert_into_marketplace", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        funcLogs("admin_insert_into_marketplace", "item added successfully");
        MessageBeep(MB_OK);
    return 0;
}

static int fetchuserCASH(const char* s)
{
    std::string sql = "SELECT cash FROM User WHERE username = '" + user.u_name + "';";
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_CASH, 0, &messageError);

    if (exit != SQLITE_OK) {
        DBlog("fetchuserCASH", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        return 0;
}
std::string s_i;
static int insertData(const char* s, std::string sql)
{
    std::ofstream myfile;
    myfile.open("database errors.txt", std::ios::app);
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK || exit == SQLITE_BUSY) {
        DBlog("insertData", messageError);
        MessageBeep(MB_ICONERROR);
        system("pause");
    }
    else
        MessageBeep(MB_OK);
    return 0;
}
static int selectData(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) {
        DBlog("selectData", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);
        return 1;
    }
    else
        return 0;
}

static int add_inventory(const char* s, std::string sql)
{
    sqlite3* DB;

    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK || exit == SQLITE_BUSY) {
        DBlog("add_inventory", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);

    }

    else
        return 0;
}
static int update_user_inventory(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK || exit == SQLITE_BUSY) {
        DBlog("update_user_inventory", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);
        return 1;
    }
    else
        return 0;
}
static int delete_from_marketplace(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK || exit == SQLITE_BUSY) {
        DBlog("delete_from_marketplace", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);
        return 1;
    }
    else
        return 0;
}
static int send_money_to_owner(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK || exit == SQLITE_BUSY) {
        DBlog("send_money_to_owner", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);
        return 1;
    }
    else
        return 0;
}
static int fetch_owner_money_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        std::string temp = argv[0];
        market.owner_money = stoi(temp);      
    }
    return 0;
}
static int fetch_owner_money(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), fetch_owner_money_callback, 0, &messageError);

    if (exit != SQLITE_OK || exit == SQLITE_BUSY) {
        DBlog("fetch_owner_money", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);
        return 1;
    }
    else
        MessageBeep(MB_OK);
        return 0;
}
void messagebox(int type, const char message[100]) {
    MessageBeep(MB_ICONWARNING);
   if (ImGui::Begin("Message From dbincpp", 0, ui::window_flags))
        ImGui::Text(message);
   switch (type) {
   case 1:
       if (ImGui::Button("ok")) {
           confirm_item_from_marketplace = false;
           buy_item = false;
       }
       break;
   }
   ImGui::End();
}

static int add_to_purchases(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK || exit == SQLITE_BUSY) {
        DBlog("add_to_purchases", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);
        return 1;
    }
    else
        MessageBeep(MB_OK);
    return 0;
}
void confirm_purchase() {

    std::string message = "you don't have enough funds";
    std::ostringstream str1;
    if (user.user_cash >= market.listed_item_price) {
        std::ofstream myfile;
        myfile.open("c://" + user.u_name + "_purchaseHis.txt", std::ios::app);
        std::string add_inv = "INSERT INTO Inventory (userID, item, amount) VALUES (" + user.user_uid + ", '" + market.listed_item_name + "', 1)";
        add_inventory(dir, add_inv);;
        std::ostringstream str1;
        int pay_amount = user.user_cash - market.listed_item_price;
        str1 << pay_amount; //stores pay_amount into str1
        std::string i_pay_amount = str1.str(); //converts pay_amount which is stored in str1 into i_pay_amount (string)
        time_t now = time(0);
        char* dt = ctime(&now);
        myfile << "\n" << "date:" << dt << "> money had : " << user.user_cash << "\n> money left : " << i_pay_amount << "\n> item price : " << market.listed_item_price << "\n> item name : " << market.listed_item_name << "\n\n";
        myfile.close();
        std::ofstream data;
        std::string query = "UPDATE User SET cash = " + i_pay_amount + " WHERE id =" + user.user_uid + ";";
        update_user_inventory(dir, query);
        std::string delete_item = "DELETE FROM Marketplace WHERE itemName = '" + market.listed_item_name + "';";
        std::string fetch_cash = "SELECT cash FROM User WHERE id = " + market.listed_item_owner + ";";
        fetch_owner_money(dir, fetch_cash);
        int owner_new_money = market.owner_money + market.listed_item_price;
        std::ostringstream str2;
        str2 << owner_new_money;
        std::string owner_amount = str2.str();
        query = "UPDATE User SET cash = " + owner_amount + " WHERE id = " + market.listed_item_owner + ";";
        update_user_inventory(dir, query);
        delete_from_marketplace(dir, delete_item);
        std::ostringstream str3;
        str3 << market.listed_item_price;
        std::string saved_amount = str3.str();
        funcLogs("confirm_purchase", "purchase was successfull");
        std::string query1 = "INSERT INTO Purchases (listed_by, itemName, price, purchased_by) VALUES ('" + market.listed_item_owner + "', '" + market.listed_item_name + "'," + saved_amount + ", '" + user.u_name + "');";
        add_to_purchases(dir, query1);
        confirm_item_from_marketplace = false;
        buy_item = false;
    }
    else
        funcLogs("confirm_purchase", "purchase was NOT successfull");
        messagebox(1,"you have insufficient funds");
}


void buy_item_from_marketplace() { // buy item tab

    select_from_marketplace(dir);
    ImGui::SameLine();
    if (ImGui::Button("Back")) {
        confirm_item_from_marketplace = false;
        buy_item = false;
    }

}
void View_Marketplace() {
        select_from_marketplace(dir);
    if (ImGui::Button("Back")) {
        globals.marketplace = false;
    }
}
static int createTable(const char* s) {
    system("title initializing databases");
    int exception;
    sqlite3* db;
    std::string query = "CREATE TABLE IF NOT EXISTS User ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "cash INTEGER NOT NULL DEFAULT 0);";

    char* Error;
    int exit = 0;
    exit = sqlite3_open(s, &db);
    exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
    if (exit != SQLITE_OK) {
        DBlog("User table", Error);
        sqlite3_free(Error);
    }
    else {
        query = "CREATE TABLE IF NOT EXISTS Marketplace ("
            "userID INTEGER,"
            "itemName TEXT,"
            "price INTEGER);";

        exit = sqlite3_open(s, &db);
        exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
        if (exit != SQLITE_OK) {
            DBlog("Marketplace table", Error);
            sqlite3_free(Error);
        }
        else {
            //logs("Marketplace Database table Created Successfully", 1);
            query = "CREATE TABLE IF NOT EXISTS Inventory ("
                "userID INTEGER,"
                "item TEXT,"
                "amount INTEGER);";

            exit = sqlite3_open(s, &db);
            exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
            if (exit != SQLITE_OK) {
                DBlog("Inventory table", Error);
                sqlite3_free(Error);
            }
            else {
                //	logs("Inventory table was created successfully!", 1);
                query = "CREATE TABLE IF NOT EXISTS Messages ("
                    "MessageID	INTEGER UNIQUE,"
                    "senderID INTEGER,"
                    "sender_name TEXT,"
                    "receiverID INTEGER,"
                    "message TEXT,"
                    "PRIMARY KEY(MessageID AUTOINCREMENT));";

                exit = sqlite3_open(s, &db);
                exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);

                if (exit != SQLITE_OK) {
                    DBlog("Messages table", Error);
                    sqlite3_free(Error);
                }
                else {
                    query = "CREATE TABLE IF NOT EXISTS Logins ("
                        "username TEXT NOT NULL,"
                        "date TEXT NOT NULL);";
                    exit = sqlite3_open(s, &db);
                    exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);

                    if (exit != SQLITE_OK) {
                        DBlog("Logins table", Error);
                        sqlite3_free(Error);
                    }
                    else {
                        query = "CREATE TABLE IF NOT EXISTS Purchases ("
                            "listed_by TEXT,"
                            "itemName TEXT,"
                            "price int,"
                            "purchased_by TEXT);";
                        exit = sqlite3_open(s, &db);
                        exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);

                        if (exit != SQLITE_OK) {
                            DBlog("purchases table", Error);
                            sqlite3_free(Error);
                        }
                        
                    }
                }


            }
        }
    }

    sqlite3_close(db);

    return 0;
}
void view_purchase_history() {
    std::string line;
    std::fstream myfile;
    myfile.open("c://" + user.u_name + "_purchaseHis.txt");
    if (!myfile.is_open()) {
        MessageBeep(MB_ICONERROR);
        ImGui::Text("Something went wrong");
    }
    else

        while (getline(myfile, line)) {
            ImGui::Text(line.c_str());
        }
}
void view_atm() {
start:
    static int i;
    std::stringstream cash;
    cash << user.user_cash;
    std::string user_cash = cash.str();
    ImGui::Text("You have $%s", user_cash.c_str());
    if (ImGui::TreeNode("view purchases")) {
        view_purchase_history();
        ImGui::TreePop();
    }
    if (user.user_uid == "1") {
        ImGui::SliderInt("Money", &i, 0, 100000, "%d", ImGuiSliderFlags_Logarithmic);
        std::stringstream ss;
        ss << i;
        s_i = ss.str();
        if (ImGui::Button("Add Money")) {
            user.user_cash = i;
            std::string sql = "UPDATE User SET cash = " + s_i + " WHERE username = '" + globals.user_name + "';";
            funcLogs("view_atm", "money added successfully");
            insertData(dir, sql.c_str());
            goto start;
        }
        ImGui::SameLine();
    }
    
    if (ImGui::Button("Back")) {
        globals.ATM = false;
    }
}
void sell_item() {
    ImGui::InputText("Item Price", globals.user_item_price, IM_ARRAYSIZE(globals.user_item_price));
    user.marketplace_item_price = globals.user_item_price;
    if (ImGui::Button("Add Item")) {
        insert_into_marketplace(dir);
        globals.sell_item = false;
    }
   
}
void add_item() {
    ImGui::InputText("Item Name", globals.user_item, IM_ARRAYSIZE(globals.user_item));
    ImGui::InputText("Item Price", globals.user_item_price, IM_ARRAYSIZE(globals.user_item_price));
    user.marketplace_item_price = globals.user_item_price;
    user.marketplace_item_name = globals.user_item;
    if (ImGui::Button("Generate Item")) {
        admin_insert_into_marketplace(dir);
        globals.generate_item = false;
    }
}
static int check_for_inventory_items_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 1; i++) {
        ImGui::Selectable(argv[0]);


        /*ImGui::Text("item:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[0]);
        ImGui::SameLine();
        ImGui::Text("price:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[1]);
    */
    }
    if (ImGui::BeginPopupContextItem()) {
        ImGui::Text("actions for %s", argv[0]);
        if (ImGui::Button("sell item")) {
            user.marketplace_item_name = argv[0];
            globals.sell_item = true;
            ImGui::CloseCurrentPopup();

        }
        if (ImGui::Button("gift item")) {
            //Todo
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();

    }
    return 0;
    return 0;

}
static int check_for_inventory_items(const char* s) {
    std::string item = globals.item;
    std::string sql_query = "SELECT item, amount FROM Inventory WHERE userID = " + user.user_uid + ";";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(db, sql_query.c_str(), check_for_inventory_items_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("check_for_inventory_items", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int check_for_listed_items_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 1; i++) {
        ImGui::Text("item:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[0]);
        ImGui::SameLine();
        ImGui::Text("price: $");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[1]);
        ImGui::Separator();
    }
    return 0;

}
static int check_for_listed_items(const char* s) {
    std::string item = globals.item;
    std::string sql_query = "SELECT itemName, price FROM Marketplace WHERE userID = " + user.user_uid + ";";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), check_for_listed_items_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("check_for_listed_items", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
void view_inventory() {
    ImGui::Text("Item(s) you have in inventory:");
    check_for_inventory_items(dir);
    ImGui::Text("Item(s) you have listed in marketplace:");
    check_for_listed_items(dir);
    if (ImGui::Button("Back")) {
        globals.inventory = false;
    }

}
static int userlist_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    
    for (i = 0; i < argc - 3; i++) {
       ImGui::Selectable(argv[1]);
    }
    if (ImGui::BeginPopupContextItem()) {
        ImGui::Text("userID: %s", argv[0]);
        ImGui::Text("username: %s", argv[1]);
        ImGui::Text("%s's password: %s", argv[1], argv[2]);
        ImGui::Text("%s's cash: %s",argv[1], argv[3]);
        if (ImGui::Button("modify")) {
            user.user_modify_name = argv[1];
            user.user_modify_cash = argv[3];
            user.user_modify_password = argv[2];
            user.user_modify_uid = argv[0];
            globals.modify_user = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    } 
    return 0;

}
static int find_wanted_usercallback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 3; i++) {
        ImGui::Text("UID:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[0]);
        ImGui::SameLine();
        ImGui::Text("username:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[1]);
        ImGui::SameLine();
        ImGui::Text("password:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[2]);
        ImGui::SameLine();
        ImGui::Text("cash:");
        ImGui::SameLine();
        ImGui::TextColored(ImColor(255, 0, 255), "%s", argv[3]);
        ImGui::SameLine();
        ImGui::Separator();
    }
    return 0;

}
static int find_wanted_user(const char* s) {
    std::string item = globals.item;
    std::string sql_query = "SELECT * FROM User WHERE id = '" + user.user_modify_uid + "';";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), find_wanted_usercallback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("find_wanted_user", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int set_new_values(const char* s, std::string sql) {
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);
    exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("set_new_values", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
void modify_user() {
    TextCentered("dbincpp admin tools (modify user)");
    find_wanted_user(dir);
    if (ImGui::TreeNode("Edit Username")) {
        ImGui::PushItemWidth(150);
        ImGui::InputText("new username", globals.modify_username, IM_ARRAYSIZE(globals.modify_username));
        if (ImGui::Button("send")) {
            std::string newname = globals.modify_username;
            std::string sql = "UPDATE User SET username = '" + newname + "' WHERE username = '" + user.user_modify_name + "';";
            set_new_values(dir, sql);

        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Edit password")) {
        ImGui::PushItemWidth(150);
        ImGui::InputText("new password", globals.modify_password, IM_ARRAYSIZE(globals.modify_password));
        if (ImGui::Button("send")) {
            std::string newpass = globals.modify_password;
            std::string sql = "UPDATE User SET password = '" + newpass + "' WHERE username = '" + user.user_modify_name + "';";
            set_new_values(dir, sql);

        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Edit cash")) {
        ImGui::PushItemWidth(150);
        ImGui::InputText("new amount", globals.modify_cash, IM_ARRAYSIZE(globals.modify_cash));
        if (ImGui::Button("send")) {
            std::string newpass = globals.modify_cash;
            std::string sql = "UPDATE User SET cash = '" + newpass + "' WHERE username = '" + user.user_modify_name + "';";
            set_new_values(dir, sql);
            find_wanted_user(dir);

        }
        ImGui::TreePop();
    }
    if (ImGui::Button("Back")) {
        globals.modify_user = false;
    }
}
static int userlist(const char* s) {
    std::string sql_query = "SELECT * FROM User";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), userlist_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("userlist", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
void TextCentered(std::string text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextColored(ImColor(255,0,255),text.c_str());
    ImGui::Separator();
}
void view_userlist() {
    TextCentered("Dbincpp admin tools (userlist)");
    userlist(dir);
    if (ImGui::Button("Back")) {
        globals.user_list = false;
    }
}
static int fetch_messages_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 1; i++) {
        ImGui::Text("from: %s", argv[0]);
        ImGui::Text("message: %s", argv[1]);
        ImGui::Separator();
    }
    return 0;

}
static int fetch_messages(const char* s) {
    std::string item = globals.item;
    std::string sql_query = "SELECT sender_name, message FROM Messages WHERE receiverID = " + user.user_uid + ";";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), fetch_messages_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("fetch_messages", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int find_user_uid_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {

        user.s_uid = argv[0];
    }
    return 0;

}
static int find_user_uid(const char* s) {
    std::string s_username = globals.user_message_name;
    std::string sql_query = "SELECT id FROM User WHERE username = '" + s_username + "';";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), find_user_uid_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("find_user_uid", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int clearlogins(const char* s) {
    std::string sql_query = "DELETE FROM Logins";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("clearlogins", messageError);
        sqlite3_free(messageError);
        MessageBeep(MB_ICONERROR);
    }
    else
        return 0;
}
void send_user_message() {
    ImGui::InputText("Message", globals.user_message, IM_ARRAYSIZE(globals.user_message));
    if (ImGui::Button("Send")) {
        find_user_uid(dir);
        std::string message = globals.user_message;
        std::string send_message1 = "INSERT INTO Messages (senderID, sender_name, receiverID, message) VALUES (" + user.user_uid + ",'" + user.s_username + "'," + user.s_uid + ",'" + message + "');";
        insertData(dir, send_message1);
        send_message = false;
    }
}
static int get_users_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        ImGui::Selectable(argv[0]);
        
    }
    if (ImGui::BeginPopupContextItem()) {
        ImGui::Text("send message");
        if (ImGui::Button("select")) {
            globals.user_message_name = argv[0];
            send_message = true;
           
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    return 0;

}
static int get_users(const char* s) {
    std::string s_username = globals.user_message_name;
    std::string sql_query = "SELECT username FROM User";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), get_users_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("get_users", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
void view_messages() {
    if (ImGui::TreeNode("View Messages")) {
        fetch_messages (dir);

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Send Message")) {
        get_users(dir);
        ImGui::TreePop();
    }
    if (ImGui::Button("Back")) {
        globals.messages = false;
    }
}

void roulette() { // beta

    std::ostringstream convert;
    TextCentered("russian roulette");
    start:
    std::ostringstream user_cash;
    user_cash << user.user_cash;
    std::string user_cash1 = user_cash.str();
    ImGui::Text("you have $%s", user_cash1.c_str());
    if (ImGui::Button("try")) {
        int pay_amount = rand() % 10 + 50;
        int random = rand() % 6 + 1;
        int hit = rand() % 6 + 1;
        if (random == hit) {

            MessageBeep(MB_ICONERROR);
            pay_amount = user.user_cash - pay_amount;
            convert << pay_amount;
            std::string covert_1 = convert.str();
            std::string sql = "UPDATE User SET cash =" + covert_1 + " WHERE username = '" + user.u_name + "';";
            insertData(dir, sql);
            log("user lost and now has $" + covert_1);
            goto start;
        }
        else
            fetchuserCASH(dir);
        pay_amount = user.user_cash + pay_amount;
        convert << pay_amount;
        std::string covert_1 = convert.str();
        MessageBeep(MB_OK);
        std::string sql = "UPDATE User SET cash =" + covert_1 + " WHERE username = '" + user.u_name + "';";
        insertData(dir, sql);
        log("user won and now has $" + covert_1);
        goto start;
    }
    
    if (ImGui::Button("close")) {
        globals.roulette = false;
    }
}
void fetch_DBlogs() {
    std::fstream database_logs;
    std::string line;
    int number = 0;
    database_logs.open("c://database_errors.txt");
    if (!database_logs.is_open())
    {
        ImGui::Text("Something bad happened");
    }
    else
    while (getline(database_logs, line)) {
        ImGui::Text("(%d) %s", number, line.c_str());
        number++;
    }
}
void fetch_func_logs() {
    std::fstream logs;
    std::string line;
    int number = 0;
    logs.open("c://dbincpp_function_logs.txt");
    if (!logs.is_open())
    {
        ImGui::Text("Something bad happened");
    }
    else
        while (getline(logs, line)) {
            ImGui::Text("(%d) %s", number, line.c_str());
            number++;
        }
}
void fetch_logs() {
    std::fstream logs;
    std::string line;
    int number = 0;
    logs.open("c://dbincpp_logs.txt");
    if (!logs.is_open())
    {
        ImGui::Text("Something bad happened");
    }
    else
        while (getline(logs, line)) {
            ImGui::Text("(%d) %s", number, line.c_str());
            number++;
        }
}
static int get_all_messages_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 4; i++) {
        ImGui::Text("> MessageID: %s", argv[0]);
        ImGui::Text("> SenderID: %s", argv[1]);
        ImGui::Text("> Sender_name: %s", argv[2]);
        ImGui::Text("> ReceiverID: %s", argv[3]);
        ImGui::Text("> Message: %s", argv[4]);
        ImGui::Separator();

    }
   
    
    return 0;

}
static int get_all_messages(const char* s, std::string sql_query) {
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), get_all_messages_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("get_users", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int get_all_purchases_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 4; i++) {
        ImGui::Text("> : %s", argv[0]);
        ImGui::Text("> SenderID: %s", argv[1]);
        ImGui::Text("> Sender_name: %s", argv[2]);
        ImGui::Text("> ReceiverID: %s", argv[3]);
        ImGui::Text("> Message: %s", argv[4]);
        ImGui::Separator();

    }


    return 0;

}
static int get_all_purchases(const char* s, std::string sql_query) {
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), get_all_purchases_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("get_users", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
void view_all_logs() {
    if (ImGui::TreeNode("view all messages")) {
        get_all_messages(dir, "SELECT * FROM Messages");
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("view all purchases")) {
        ImGui::TreePop();
    }
    if (ImGui::Button("Back")) {
        globals.admin_view_all_logs = false;
    }

}
void ui::userPanel() {

    if (!globals.active && user.user_uid != "") return;
    ImGui::SameLine();
    if (ImGui::Button("Marketplace")) {
        log("clicked marketplace");
        globals.marketplace = true;
        View_Marketplace();
        //TODO MARKETPLACE
    }
    ImGui::SameLine();
    if (ImGui::Button("Messages")) {
        log("clicked Messages");
        globals.messages = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("ATM")) {
        log("clicked ATM");
        globals.ATM = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Inventory")) {
        log("clicked Inventory");
        globals.inventory = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Roulette (W.I.P)")) {
        log("clicked Roulette (W.I.P)");
        globals.roulette = true;
    }
    if (ImGui::Button("Appareance")) {
        log("clicked Appareance");
        appareance();
    }
    ImGui::SameLine();
    if (ImGui::Button("Log Out")) {
        log("clicked Log Out");
        //TODO Log out

    }

    fetchuserUsername(dir);
    fetchuserUID(dir);
    fetchuserCASH(dir);
    
    if (user.user_uid == "1") {
        ImGui::Separator();
        ImGui::TextColored(ImColor(255, 0, 255), "Admin Tools");
        if (ImGui::Button("All logs (W.I.P)")) {
            globals.admin_view_all_logs = true;
        }
        if (ImGui::Button("User List")) {
            globals.user_list = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove Item(s)")) {

        }
        ImGui::SameLine();
        if (ImGui::Button("add item(s)")) {
            globals.generate_item = true;
        }
        if (ImGui::TreeNode("Total logins")) {
            if (ImGui::Button("Clear Logins")) {
                clearlogins(dir);
            }
            fetch_logins(dir);
            ImGui::TreePop();
        }
        ImGui::Separator();
        ImGui::Text("for debugging");
        if (ImGui::TreeNode("View function logs")) {
            if (ImGui::Button("Clear Logs")) {
                remove("c://dbincpp_function_logs.txt");
            }
            fetch_func_logs();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("view logs")) {
            if (ImGui::Button("Clear Logs")) {
                remove("c://dbincpp_logs.txt");
            }
            fetch_logs();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("view database logs")) {
            if (ImGui::Button("Clear Logs")) {
                remove("c://database_errors.txt");
            }
            fetch_DBlogs();
            ImGui::TreePop();
        }



    }
    else
        ImGui::Separator();
    //ImGui::Text("Welcome back, " + globals.user_name);
}
static int callback_fetch_username(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        if (i == 0)
        {
            MessageBeep(MB_ICONERROR);
            ImGui::Text("user: %s is already a member", argv[0]);
            failed = true;

        }
        else
        {
            return 0;
        }

    }
    return 0;
}
static int fetch_username(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_fetch_username, 0, &messageError);

    if (exit != SQLITE_OK) {
        DBlog("fetch_username", messageError);
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        return(0);
}
void r_register() {
    std::string check_for_user = "SELECT username FROM user WHERE username = '" + user.u_name + "';";
    fetch_username(dir, check_for_user);
    if (failed != true) {
        std::string sql = "INSERT INTO User (username, password) VALUES ('" + user.u_name + "','" + user.u_pass + "');";
        Register(dir, sql.c_str());
        MessageBeep(MB_OK);
    }

}
static int fetch_logins_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 1; i++) {
        ImGui::Text(argv[0]);
        ImGui::Text(argv[1]);
        ImGui::Separator();
    }
    return 0;

}
static int fetch_logins(const char* s) {

    std::string sql_query = "SELECT * FROM Logins";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), fetch_logins_callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("fetch_logins", messageError);
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int add_logins(const char* s) {
    time_t now = time(0);
    char* dt = ctime(&now);
    std::string sql_query = "INSERT INTO Logins (username, date) VALUES ('" + user.u_name + "', '" + dt + "');";
    sqlite3* db;
    char* messageError;
    int exit = sqlite3_open(s, &db);

    exit = sqlite3_exec(db, sql_query.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        DBlog("add_logins", messageError);
        sqlite3_free(messageError);
        system("pause");
        return 1;
    }
    else
        return 0;
}
void appareance() {
    if (ImGui::BeginChild("appareance", ImVec2(100,100),false, ui::window_flags))
    ImGui::Text("Coming soon.");
    if (ImGui::Button("Back")) {
        globals.appareance = false;
    }
    ImGui::EndChildFrame();
}


void l_login() {

    std::string sql = "SELECT username, password FROM User WHERE username = '" + user.u_name + "' AND password = '" + user.u_pass + "';";
    fetch_credentials(dir, sql.c_str());
    // sql = "SELECT logins FROM Logins";
    add_logins(dir);
    ui::userPanel();

}
void ui::render() {
    if (globals.dbCreated == false) {
        createTable(dir);
        globals.dbCreated = true;
    }
    if (!globals.active) return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.0f);

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
        if (globals.admin_view_all_logs != true) {
            if (globals.generate_item != true) {
                if (globals.sell_item != true) {
                    if (globals.roulette != true) {
                        if (send_message != true) {
                            if (globals.messages != true) {
                                if (globals.modify_user != true) {
                                    if (globals.user_list != true) {
                                        if (globals.inventory != true) {
                                            if (confirm_item_from_marketplace != true) {
                                                if (globals.ATM != true) {
                                                    if (buy_item != true) {
                                                        if (globals.marketplace != true) {
                                                            if (globals.userpanel != true) {
                                                                ImGui::PushItemWidth(150);
                                                                ImGui::InputText("Username", globals.user_name, IM_ARRAYSIZE(globals.user_name));
                                                                ImGui::PushItemWidth(150);
                                                                ImGui::InputText("Password", globals.pass_word, IM_ARRAYSIZE(globals.pass_word), ImGuiInputTextFlags_Password);
                                                                ImGui::SameLine();
                                                                HelpMarker("Forgot your password? Download DB browser or Delete database to recreate database.");
                                                                if (ImGui::Button("Login")) {
                                                                    user.u_name = globals.user_name;
                                                                    user.u_pass = globals.pass_word;
                                                                    if (user.u_name.length() >= 1 && user.u_pass.length() >= 1) {
                                                                        l_login();

                                                                    }
                                                                    else
                                                                        MessageBeep(MB_ICONERROR);
                                                                }
                                                                ImGui::SameLine();

                                                                if (ImGui::Button("Register")) {
                                                                    user.u_name = globals.user_name;
                                                                    user.u_pass = globals.pass_word;
                                                                    r_register();
                                                                }
                                                            }
                                                            else
                                                                userPanel();
                                                        }
                                                        else
                                                            View_Marketplace();
                                                    }
                                                    else
                                                        buy_item_from_marketplace();
                                                }
                                                else
                                                    view_atm();
                                            }
                                            else
                                                confirm_purchase();
                                        }
                                        else
                                            view_inventory();

                                    }
                                    else
                                        view_userlist();
                                }
                                else
                                    modify_user();
                            }
                            else
                                view_messages();
                        }
                        else
                            send_user_message();
                    }
                    else
                        roulette();
                }
                else
                    sell_item();
            }
            else
                add_item();
        }
        else
            view_all_logs();
    }
    ImGui::End();
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;

    // colors
    ImGui::StyleColorsDark();

    if (window_pos.x == 0) {
        RECT screen_rect{};
        GetWindowRect(GetDesktopWindow(), &screen_rect);
        screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
        window_pos = (screen_res - window_size) * 0.5f;

        // init images here
    }
}