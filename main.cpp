#include <stdio.h>
#include <iostream>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdlib.h>
#include <malloc.h>
#include "database.h"
#include "de.h"
#include "handler.h"
#include "html.h"
#include "structs.h"
#include "global.h"

using namespace std;
using namespace sql::mysql;

bool marks_login_update_test(vector<stud_row> &marks, int till) {
    bool ok = true;
    for (int i = 0; i < till; i++) {
        for (int j = 0; j < 20; j++) {
            if (marks[i].results[j].has_data) {
                if (!add_result(marks[i].student_id, marks[i].results[j].subj_id, marks[i].results[j].points, marks[i].results[j].complete_time))
                    ok = false;
            }
        }
    }
    return ok;
}

int main(int args_c, char* args[]) {
    vector<stud_row> * tte = new vector<stud_row>();
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
            ctdd_connect("95.213.143.187", 3306, "u18489_rmnsv", "2A2rtQxbWYnU");
            GLOBAL_db_used = "u18489_ctdd";
            create_bot();
            ctdd_disconnect();
            return 0;
        } else if (bot_or_not == "test") {
            cout << "This is a test. Get ready." << endl;
            //

        } else {
            cout << "Wrong arg, try \"bot\"." << endl;
            return 0;
        }
    } else if (args_c == 3) {
        de_lg = args[1];
        de_ps = args[2];
        cout << "Got login and password, trying to login as " << de_lg << "." << endl;
    } else {
        cout << "What are you trying to do?" << endl;
        return 0;
    }

    cout << "Step 1/5. Getting data from de.ifmo.ru... Wait." << endl;
    //freopen("results.txt", "r", stdin);
    //string json_data;
    //cin >> json_data;
    pair<string, string> datas;
    try {
        datas = de_data_get(de_lg, de_ps);
    } catch (const std::exception& e) {
        cout << "Something is wrong. Check your Internets and make sure that your login and password are correct." << endl;
        exit(0);
    }

    cout << "This time a new student will be added. Idk, why I say this." << endl;
    //freopen("results.txt", "w", stdout);
    //cout << json_data << endl;
    cout << "Step 2/5. Connecting to database at 95.213.143.187... Wait." << endl;
    try {
        ctdd_connect("95.213.143.187", 3306, "u18489_rmnsv", "2A2rtQxbWYnU");
        cout << "Connect OK." << endl;
        GLOBAL_db_used = "u18489_ctdd";
    } catch (const exception& e) {
        cout << "Database connect error. That's all I know. Bye." << endl;
        exit(0);
    }

    //ctdd_connect("localhost", 3306, "root", "")
    GLOBAL_students_total = how_many_students();
    cout << "Step 3/5. JSON parsing. I hate this job." << endl;
    try {
        *tte = parse_default(datas.first, datas.second);
    } catch (const exception&e) {
        cout << "Parsing failure. I'm out." << endl;
        exit(0);
    }
    cout << "Parse OK." << endl;
    cout << "Step 4/5. Updating the database." << endl;
    try {
        new_student(* tte, de_lg);
    } catch (const exception& e) {
        cout << "Failed. Sorry." << endl;
    }
    cout << endl << "Done! Disconnecting..." << endl;
    //print_student_marks_default(parse_default(json_data));
    //html_create_test(datas.second, parse_default(json_data, datas.second), 3);
    //html_print_term(datas.second, parse_default(json_data, datas.second), 4);

    ctdd_disconnect();
    return 0;
}