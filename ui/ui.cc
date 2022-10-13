#include "ui.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../sqlite/sqlite3.h"
#include <iostream>
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
enum MyItemColumnID
{
    MyItemColumnID_ID,
    MyItemColumnID_Name,
    MyItemColumnID_Action,
    MyItemColumnID_Quantity,
    MyItemColumnID_Description
};
struct MyItem
{
    int         ID;
    const char* Name;
    int         Quantity;

    // We have a problem which is affecting _only this demo_ and should not affect your code:
    // As we don't rely on std:: or other third-party library to compile dear imgui, we only have reliable access to qsort(),
    // however qsort doesn't allow passing user data to comparing function.
    // As a workaround, we are storing the sort specs in a static/global for the comparing function to access.
    // In your own use case you would probably pass the sort specs to your sorting/comparing functions directly and not use a global.
    // We could technically call ImGui::TableGetSortSpecs() in CompareWithSortSpecs(), but considering that this function is called
    // very often by the sorting algorithm it would be a little wasteful.
    static const ImGuiTableSortSpecs* s_current_sort_specs;

    // Compare function to be used by qsort()
    static int IMGUI_CDECL CompareWithSortSpecs(const void* lhs, const void* rhs)
    {
        const MyItem* a = (const MyItem*)lhs;
        const MyItem* b = (const MyItem*)rhs;
        for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
        {
            // Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
            // We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
            const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
            int delta = 0;
            switch (sort_spec->ColumnUserID)
            {
            case MyItemColumnID_ID:             delta = (a->ID - b->ID);                break;
            case MyItemColumnID_Name:           delta = (strcmp(a->Name, b->Name));     break;
            case MyItemColumnID_Quantity:       delta = (a->Quantity - b->Quantity);    break;
            case MyItemColumnID_Description:    delta = (strcmp(a->Name, b->Name));     break;
            default: IM_ASSERT(0); break;
            }
            if (delta > 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
            if (delta < 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
        }

        // qsort() is instable so always return a way to differenciate items.
        // Your own compare function may want to avoid fallback on implicit sort specs e.g. a Name compare if it wasn't already part of the sort specs.
        return (a->ID - b->ID);
    }
};
static int callback_login(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        if (i == 0)
        {
            if (argv[0] == user.u_name && argv[1] == user.u_pass) {
                ui::userPanel();
            }
            // printf("name: %s price: $%s", argv[0], argv[1]);
        }
    }
    printf("\n");
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
    }
    return 0;
}
static int fetchuserUsername(const char* s)
{
    std::string sql = "SELECT cash FROM User WHERE username = '" + user.u_name + "';";
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
static int callback_insert_into_marketplace(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        ImGui::Text("your items:");
        ImGui::SameLine();
        ImGui::Text(argv[0]);
    }
    return 0;
}
static int callback_select_from_marketplace(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
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
    exit = sqlite3_exec(DB, sql.c_str(), callback_insert_into_marketplace, 0, &messageError);

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
void View_Marketplace() {
    if (ImGui::TreeNode("Listed Items")) {
        select_from_marketplace(dir);
        ImGui::TreePop();
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
        //TODO ATM
    }
    ImGui::SameLine();
    if (ImGui::Button("Inventory")) {
        //TODO Inventory
    }
    ImGui::SameLine();
    if (ImGui::Button("Log Out")) {
        //TODO Log out
    }
    ImGui::SameLine();
    if (ImGui::Button("Job Selector")) {
        //TODO JOB

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
        if (globals.marketplace != true) {
            if (globals.userpanel != true) {
                ImGui::InputText("Username", globals.user_name, IM_ARRAYSIZE(globals.user_name));
                ImGui::InputText("Password", globals.pass_word, IM_ARRAYSIZE(globals.pass_word), ImGuiInputTextFlags_Password);
                if (ImGui::Button("Login")) {
                    user.u_name = globals.user_name;
                    user.u_pass = globals.pass_word;
                    globals.userpanel = true;
                    l_login();
                }
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