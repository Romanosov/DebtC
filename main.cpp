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

using namespace std;
using namespace sql::mysql;

int main() {
    std::cout << "Hello, World!" << std::endl;
    int q_id;

    cout << "DB data get test." << endl << "login:" << endl;
    string de_login;
    cin >> de_login;
    cout << "password:" << endl;
    string de_pass;
    cin >> de_pass;
    cout << de_data_get(de_login, de_pass) << endl;
    cout << "**********************************************" << endl;
    cout << "DB test. Enter subject ID of 5 term." << endl;

    do {
        cin >> q_id;
        string qu = std::to_string(q_id);
        cout << test_query("SELECT * FROM djournal.subjects WHERE djournal.subjects.id = " + qu + ";") << endl;
    } while (q_id != 0);

    return 0;
}