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
#include "html.h"
#include "structs.h"
#include "global.h"

using namespace std;
using namespace sql::mysql;

vector<stud_row> tte;

bool marks_login_update_test(vector<stud_row> marks, int till) {
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
            //

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
    pair<string, string> datas = de_data_get(de_lg, de_ps);
    cout << "1.1. Refreshing the database." << endl;
    //freopen("results.txt", "w", stdout);
    //cout << json_data << endl;
    cout << "**********************************************" << endl;
    if (ctdd_connect("localhost", 3306, "root", ""))
        cout << "Connect OK." << endl;
    GLOBAL_students_total = how_many_students();
    cout << "2. JSON parsing test." << endl;
    tte = parse_default(datas.first, datas.second);
    cout << "Parse OK." << endl;
    //new_student(tte, de_lg);
    for (int i = 0; i < tte.size(); i++) {
        //cout << tte[i].name << endl;
        //cout << tte[i].group << endl;
        if (add_student(stoi(de_lg), tte[i].name, tte[i].group, tte[i].term)) {
            cout << "TERM " << tte[i].term << endl;
            bool ok = true;
            for (int j = 0; j < 12; j++) {
                //cout << "Are we here?" << endl;
                bool add = tte[i].results[j].has_data;
                int stud_id = tte[i].student_id;
                cout << "STUD ID: " << stud_id << endl;
                int subj_id = tte[i].results[j].subj_id;
                cout << "SUBJ ID: "<< subj_id << endl;
                double pnts = tte[i].results[j].points;
                long long int ct = tte[i].results[j].complete_time;
                //cout << add << " " << stud_id << " " << subj_id << " " << pnts << " " << ct << endl;
                //cout << "Has?" << endl;
                if (add) {
                    cout << "Has data!" << endl;
                    if (!add_result(stud_id, subj_id, pnts, ct))
                        ok = false;
                }
            }
        }
    }
    //print_student_marks_default(parse_default(json_data));
    //html_create_test(datas.second, parse_default(json_data, datas.second), 3);
    //html_print_term(datas.second, parse_default(json_data, datas.second), 4);
    cout << "**********************************************" << endl;
    cout << "3. DB test. Enter subject ID of 5 term." << endl;


    //if (ctdd_connect("localhost", 3306, "root", ""))
       // new_student(parse_default(json_data, datas.second), de_lg);

    while (q_id != 0) {
        cin >> q_id;
        string qu = to_string(q_id);
        //string qu = args[3];
        //cout << test_query("SELECT * FROM djournal.subjects WHERE djournal.subjects.id = " + qu + ";") << endl;
        cout << test_query("SELECT * FROM djournal.subjects WHERE djournal.subjects.id >= 500 AND djournal.subjects.id <= " + qu + ";") << endl;
        cout << "You can try again or type \"0\" to exit." << endl;
    }

    ctdd_disconnect();

    return 0;
}