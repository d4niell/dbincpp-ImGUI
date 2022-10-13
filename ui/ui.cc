#include "ui.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../sqlite/sqlite3.h"
#include <iostream>
#include <sstream>
#include <fstream>
static bool istrue = false;
const char* dir = "c:\\Database.db";
struct {
public:


    std::string u_name;
    std::string u_pass;
    std::string marketplace_item_name;
    std::string marketplace_item_price;
    std::string user_uid;

}user;
static bool buy_item = false;
static int callback_login(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        if (i == 0)
        {
            if (argv[0] == user.u_name && argv[1] == user.u_pass) {
                globals.userpanel = true;
              //  ui::userPanel();
            }
            else {
                ImGui::Text("Invalid Username/password");
            }
            // printf("name: %s price: $%s", argv[0], argv[1]);
        }
    }
   // printf("\n");
    return 0;
}
static int fetch_credentials(const char* s, std::string sql)
{
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_login, 0, &messageError);

    if (exit != SQLITE_OK) {      
        // std::cerr << "Error in selectData function." << std::endl;
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
        sqlite3_free(messageError);
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
    }
    return 0;
}
static int callback_username(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        ImGui::Text("Username:");
        ImGui::SameLine();
        ImGui::Text(argv[0]);
        ImGui::SameLine();
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
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        return 0;
}
static int callback_select_from_marketplace(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc - 1; i++) {
        ImGui::Text("item:");
        ImGui::SameLine();
        ImGui::Text(argv[0]);
        ImGui::SameLine();
        ImGui::Text("price:");
        ImGui::SameLine();
        ImGui::Text(argv[1]);
    }
    return 0;
}
static int select_from_marketplace(const char* s)
{
    std::string sql = "SELECT itemName, price FROM Marketplace;";
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, sql.c_str(), callback_select_from_marketplace, 0, &messageError);

    if (exit != SQLITE_OK) {
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

    if (exit != SQLITE_OK) {
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
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
        std::cerr << "Error in selectData function." << std::endl;
        sqlite3_free(messageError);
    }
    else
        return 0;
}
std::string s_i;
static int insertData(const char* s, std::string sql)
{
    std::ofstream data;
    data.open("data.txt", std::ios::app);
    sqlite3* DB;
    char* messageError;
    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in insertData function." << std::endl;
        data << "\n\n" << s_i << ":" << globals.user_name;
        data.close();
        sqlite3_free(messageError);
    }
    else
        return 0;
}

void buy_item_from_marketplace() {
    select_from_marketplace(dir);
 
    ImGui::InputText("Item name", globals.item, IM_ARRAYSIZE(globals.item));
    if (ImGui::Button("Buy Item")) {
        std::string item = globals.item;
        std::string sql = "INSERT INTO Inventory (userID, item, amount) VALUES (" + user.user_uid + ", '" + item + "', 1)";
        insertData(dir, sql);
    }

}
void View_Marketplace() {
    if (ImGui::TreeNode("Listed Items")) {
        select_from_marketplace(dir);
        ImGui::TreePop();
        if (ImGui::Button("Buy Item")) {
            buy_item = true;
        }
    }     
    if (ImGui::TreeNode("Add Item")) {
        ImGui::InputText("Item name", globals.user_item, IM_ARRAYSIZE(globals.user_item));
        ImGui::InputText("Item Price", globals.user_item_price, IM_ARRAYSIZE(globals.user_item_price));
        user.marketplace_item_name = globals.user_item;
        user.marketplace_item_price = globals.user_item_price;
        if (ImGui::Button("Add Item")) {
            insert_into_marketplace(dir);
        }

        ImGui::TreePop();
    }
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
                    sqlite3_free(Error);
                }


            }
        }
    }

    sqlite3_close(db);

    return 0;
}
void view_atm() {
        static int i;
        ImGui::SliderInt("Money", &i, 0, 100000, "%d", ImGuiSliderFlags_Logarithmic);
        std::stringstream ss;
        ss << i;
        s_i = ss.str();
        if (ImGui::Button("Add Money")) {
            std::string sql = "UPDATE User SET cash = " + s_i + " WHERE username = '" + globals.user_name + "';";
            insertData(dir, sql.c_str());
        }
        if (ImGui::Button("Back")) {
            globals.ATM = false;
        }
}
void ui::userPanel() {
    if (!globals.active) return;
    ImGui::SameLine();
    if (ImGui::Button("Marketplace")) {
        globals.marketplace = true;
        View_Marketplace();
        //TODO MARKETPLACE
    }
    ImGui::SameLine();
    if (ImGui::Button("Messages")) {
        //TODO Messages
    }
    ImGui::SameLine();
    if (ImGui::Button("ATM")) {
        globals.ATM = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Inventory")) {
        //TODO Inventory
    }
    ImGui::SameLine();
    if (ImGui::Button("Job Selector")) {
        //TODO Job Selector
    }
    ImGui::SameLine();
    if (ImGui::Button("Log Out")) {
        //TODO Log out

    }
    fetchuserUsername(dir);
    fetchuserUID(dir);
    fetchuserCASH(dir);

    //ImGui::Text("Welcome back, " + globals.user_name);
}
void r_register() {

    std::string sql = "INSERT INTO User (username, password) VALUES ('" + user.u_name + "','" + user.u_pass + "');";
    Register(dir, sql.c_str());

}
void l_login() {
    std::string sql = "SELECT username, password FROM User WHERE username = '" + user.u_name + "' AND password = '" + user.u_pass + "';";
    fetch_credentials(dir, sql.c_str());
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
        if (globals.ATM != true) {
            if (buy_item != true) {
                if (globals.marketplace != true) {
                    if (globals.userpanel != true) {
                        ImGui::InputText("Username", globals.user_name, IM_ARRAYSIZE(globals.user_name));
                        ImGui::InputText("Password", globals.pass_word, IM_ARRAYSIZE(globals.pass_word), ImGuiInputTextFlags_Password);
                        if (ImGui::Button("Login")) {
                            user.u_name = globals.user_name;
                            user.u_pass = globals.pass_word;

                            l_login();
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