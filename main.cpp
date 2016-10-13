#include <iostream>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdlib.h>
#include "database.h"

using namespace std;
using namespace sql::mysql;

struct subj {
    int subj_id;
    bool has_data;
    bool complete;
    short points;
    long long complete_time;
};

struct stud_row {
    int student_id;
    int position;
    std::string name;
    std::string group;
    subj results[20];
    int color;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    int q_id;
    do {
        cin >> q_id;
        string qu = std::to_string(q_id);
        cout << test_query("SELECT * FROM djournal.subjects WHERE djournal.subjects.id = " + qu + ";") << endl;
    } while (q_id != 0);

    return 0;
}