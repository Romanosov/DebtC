#include <stdio.h>
#include <iostream>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdlib.h>
#include "database.h"
#include "de.h"
#include "handler.h"

using namespace std;
using namespace sql::mysql;

int main(int args_c, char* args[]) {
    cout << "Hello, World!" << endl;
    int q_id = 0;
    string de_lg, de_ps;

    if (args_c == 1) {
        cout << "login:" << endl;
        string de_login;
        cin >> de_login;
        cout << "password:" << endl;
        string de_pass;
        cin >> de_pass;
        de_lg = de_login;
        de_ps = de_pass;
    } else if (args_c == 2) {
        string bot_or_not = args[1];
        if (bot_or_not == "bot") {
            cout << "Creating a bot..." << endl;
            //create_bot();
            return 0;
        } else if (bot_or_not == "test") {
            cout << "This is a test. Get ready." << endl;
            de_lg = "";
            de_ps = "";
        } else {
            cout << "Wrong arg, try \"bot\"." << endl;
            return 0;
        }
    } else if (args_c == 4) {
        de_lg = args[1];
        de_ps = args[2];
        cout << "Got login and password, trying to enter." << endl;
    } else {
        cout << "What are you trying to do?" << endl;
        return 0;
    }

    cout << "1. Getting data from de.ifmo.ru. Wait." << endl;
    //freopen("results.txt", "r", stdin);
    //string json_data;
    //cin >> json_data;
    string json_data = de_data_get(de_lg, de_ps);
    //freopen("results.txt", "w", stdout);
    //cout << json_data << endl;
    cout << "**********************************************" << endl;
    cout << "2. JSON parsing test." << endl;
    print_student_marks_default(parse_default(json_data));
    cout << "**********************************************" << endl;
    cout << "3. DB test. Enter subject ID of 5 term." << endl;

    while (q_id != 0) {
        cin >> q_id;
        string qu = to_string(q_id);
        //string qu = args[3];
        //cout << test_query("SELECT * FROM djournal.subjects WHERE djournal.subjects.id = " + qu + ";") << endl;
        cout << test_query("SELECT * FROM djournal.subjects WHERE djournal.subjects.id >= 500 AND djournal.subjects.id <= " + qu + ";") << endl;
        cout << "You can try again or type \"0\" to exit." << endl;
    }

    return 0;
}