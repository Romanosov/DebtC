//
// Created by rmnsv on 19.09.16.
//

#include <iostream>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "structs.h"
#include "handler.h"
#include "global.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql::mysql;

MySQL_Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;
string got_data;
sql::PreparedStatement  *prep_stmt;

string test_query(string query) {
    MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = sql::mysql::get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "");
    //con = driver->connect("localhost:3306", "u18489_public", "ctdd_public");
    cout << "Works!" << endl;
    stmt = con->createStatement();
    //res = stmt->executeQuery("SELECT * FROM djournal.subjects WHERE djournal.subjects.id = 505;");
    res = stmt->executeQuery(query);
    string tgot = "";
    while (res->next()) {
        tgot += "id = " + res->getString("id") + " | ";
        tgot += "name = '" + res->getString("subj_name") + "' | ";
        tgot += "short = '" + res->getString("subj_short") + "' | ";
        tgot += "is_exam = " + res->getString("is_exam") + "\n";
    }

    delete stmt;
    delete con;
    delete res;

    return tgot;
}

bool ctdd_connect(string hostname, int port, string db_login, string db_password) {
    driver = sql::mysql::get_driver_instance();
    //con = driver->connect("tcp://127.0.0.1:3306", "root", "");
    con = driver->connect(hostname + ":" + to_string(port), db_login, db_password);
    //con = driver->connect("localhost:3306", "u18489_public", "ctdd_public");
    return con != nullptr;
}

bool ctdd_disconnect() {
    delete con;
    delete res;
    return true;
}


bool add_student(int isu_num, std::string name, string group, int term) {
    int kk;
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM " + GLOBAL_db_used + ".students;");
    if (res->next())
        kk = stoi(res->getString("COUNT(*)"));
    else
        return false;
    //cout << "Count OK." << endl;
    kk++;
    prep_stmt = con->prepareStatement("INSERT INTO " + GLOBAL_db_used + ".students VALUES (?, ?, ?, ?, ?, ?);");
    //string le_query = "INSERT INTO djournal.students VALUES ('" + to_string(kk) + "', '" + to_string(isu_num) + "', '1', '" + to_string(term) + "', \"" + name + "\", \"" + group +"\");";
    prep_stmt->setInt(1, kk);
    prep_stmt->setInt(2, isu_num);
    prep_stmt->setBoolean(3, 1);
    prep_stmt->setInt(4, term);
    prep_stmt->setString(5, name);
    prep_stmt->setString(6, group);
    //cout << le_query << endl;
    prep_stmt->execute();
    delete prep_stmt;
    delete stmt;
    //cout << "Insert student OK." << endl;
    return true;
}

bool new_student(vector<stud_row> stud_marks, string isu_num) {
    cout << "I'M A FUNCKJSLKF" << endl;
     for (int i = 0; i < stud_marks.size(); i++) {
        cout << isu_num << endl;
        cout << stud_marks[i].name << endl;
        cout << stud_marks[i].group << endl;
        if (add_student(stoi(isu_num), stud_marks[i].name, stud_marks[i].group, i + 1)) {
           // marks_login_update_test(stud_marks, 2);
        }
    }
    return true;
}

bool add_subject(int term, std::string name, std::string name_short, bool is_exam) {
    res = stmt->executeQuery("");
    return true;
}

bool add_result(int student_id, int subject_id, double points, long long update_time) {
    int kk;
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM " + GLOBAL_db_used + ".result_cells;");
    res->next();
    kk = stoi(res->getString("COUNT(*)"));
    kk++;
    //res = stmt->executeQuery("select id from djournal.subjects where subj_name = \"Введение в программирование и ЭВМ\" and term_id = 1;");
    string le_query = "INSERT INTO " + GLOBAL_db_used + ".result_cells VALUES ('" + to_string(kk) + "', '" + to_string(student_id) + "', '" + to_string(subject_id) + "', '" + to_string(points) + "', '" + to_string(update_time) + "');";
    //cout << le_query << endl;
    stmt->execute(le_query);
    delete stmt;
    //cout << "Result add OK." << endl;
    return true;
}
int how_many_students() {
    int kk;
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM " + GLOBAL_db_used + ".students;");
    res->next();
    kk = stoi(res->getString("COUNT(*)"));
    return kk;
}

int get_subj_id(string subj_name, int term) {
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT id FROM " + GLOBAL_db_used + ".subjects WHERE subj_name = \"" + subj_name + "\" AND term_id = '" + to_string(term) + "';");
    res->next();
    return stoi(res->getString("id"));
};


bool add_bot() {

    return true;
}

void bot_update() {

    return;
}

result_cell get_result(int student_id, int subject_id) {
    result_cell r_retuned;

    return r_retuned;
}

stud_row get_all_the_line(int student_id) {
    stud_row r_returned;

    return r_returned;
}

std::vector<stud_row> get_all_the_students();