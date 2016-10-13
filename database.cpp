//
// Created by rmnsv on 19.09.16.
//

#include <iostream>
#include "mysql_driver.h"
#include "mysql_connection.h"
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
        tgot += "is_exam = '" + res->getString("is_exam");
    }

    delete stmt;
    delete con;
    delete res;

    return tgot;
};