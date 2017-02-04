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
    vector<stud_row> tte;
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
        tte = parse_default(datas.first, datas.second);
    } catch (const exception&e) {
        cout << "Parsing failure. I'm out." << endl;
        exit(0);
    }
    cout << "Parse OK." << endl;
    //new_student(tte, de_lg);
    cout << "Step 4/5. Updating the database." << endl;
    cout << " ____________________________________________________________" << endl;
    cout << "|0%_________________________50%__________________________100%|" << endl << "|";
    size_t tsz = tte.size();
    size_t tasks_left = tsz * 12;
    //cout << tasks_left;

    size_t kc = tasks_left;
    size_t perc = 0;
    size_t new_perc;
    string done_progress = "";
    for (int i = 0; i < tsz; i++) {
        //cout << tte[i].name << endl;
        //cout << tte[i].group << endl;
        if (add_student(stoi(de_lg), tte[i].name, tte[i].group, tte[i].term)) {
            tasks_left--;
            new_perc = (kc - tasks_left) * 60 / kc;
            if (new_perc > perc) {
                perc = new_perc;
            }
            //cout << "TERM " << tte[i].term << endl;
            bool ok = true;
            for (int j = 0; j < 12; j++) {
                //cout << "Are we here?" << endl;
                bool add = tte[i].results[j].has_data;
                int stud_id = tte[i].student_id;
                //cout << "STUD ID: " << stud_id << endl;
                int subj_id = tte[i].results[j].subj_id;
                //cout << "SUBJ ID: "<< subj_id << endl;
                double pnts = tte[i].results[j].points;
                long long int ct = tte[i].results[j].complete_time;
                //cout << add << " " << stud_id << " " << subj_id << " " << pnts << " " << ct << endl;
                //cout << "Has?" << endl;
                if (add) {
                    //cout << "Has data!" << endl;
                    if (!add_result(stud_id, subj_id, pnts, ct)) {
                        ok = false;
                    }
                }
                tasks_left--;
                new_perc = (kc - tasks_left) * 60 / kc;
                if (new_perc > perc) {
                    perc = new_perc;
                    cout << "$";
                    cout.flush();
                }
            }
        }
    }
    cout << "|" << endl << endl << "Done. Disconnecting..." << endl;
    //print_student_marks_default(parse_default(json_data));
    //html_create_test(datas.second, parse_default(json_data, datas.second), 3);
    //html_print_term(datas.second, parse_default(json_data, datas.second), 4);

    ctdd_disconnect();
    cout << "Done?" << endl;
    return 0;
}