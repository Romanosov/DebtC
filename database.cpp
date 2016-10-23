//
// Created by rmnsv on 19.09.16.
//

#include <iostream>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "handler.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdlib.h>

using namespace std;
using namespace sql::mysql;

string test_query(string query) {
    MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = sql::mysql::get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "");
    cout << "Works!" << endl;
    stmt = con->createStatement();
    //res = stmt->executeQuery("SELECT * FROM djournal.subjects WHERE djournal.subjects.id = 505;");
    res = stmt->executeQuery(query);
    string tgot = "";
    while (res->next()) {
        tgot += "id = " + res->getString("id") + " | ";
        tgot += "name = '" + res->getString("subj_name") + "' | ";
        tgot += "short = '" + res->getString("subj_short") + "' | ";
        tgot += "is_exam = " + res->getString("is_exam");
    }

    delete stmt;
    delete con;
    delete res;

    return tgot;
};

bool ctdd_connect() {

    return true;
};

bool ctdd_disconnect() {

    return true;
};

bool add_student(int st_id, int isu_num, std::string name, int term) {

    return true;
};

bool add_subject(int term, std::string name, std::string name_short, bool is_exam) {

    return true;
};

bool add_result(int student_id, int subject_id, short points, long long update_time) {

    return true;
};

bool add_bot() {

    return true;
};

void bot_update() {

    return;
};

result_cell get_result(int student_id, int subject_id) {
    result_cell r_retuned;

    return r_retuned;
};

stud_row get_all_the_line(int student_id) {
    stud_row r_returned;

    return r_returned;
};