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

bool new_student(vector<stud_row> &stud_marks, string isu_num) {
    cout << " ____________________________________________________________" << endl;
    cout << "|0%_________________________50%__________________________100%|" << endl << "|";
    size_t tsz = stud_marks.size();
    size_t tasks_left = tsz * 12;
    //cout << tasks_left;

    size_t kc = tasks_left;
    size_t perc = 0;
    size_t new_perc;
    string done_progress = "";
    for (int i = 0; i < tsz; i++) {
        //cout << tte[i].name << endl;
        //cout << tte[i].group << endl;
        if (add_student(stoi(isu_num), stud_marks[i].name, stud_marks[i].group, stud_marks[i].term)) {
            tasks_left--;
            new_perc = (kc - tasks_left) * 60 / kc;
            if (new_perc > perc) {
                perc = new_perc;
            }
            //cout << "TERM " << tte[i].term << endl;
            for (int j = 0; j < 12; j++) {
                //cout << "Are we here?" << endl;
                bool add = stud_marks[i].results[j].has_data;
                int stud_id = stud_marks[i].student_id;
                //cout << "STUD ID: " << stud_id << endl;
                int subj_id = stud_marks[i].results[j].subj_id;
                //cout << "SUBJ ID: "<< subj_id << endl;
                double pnts = stud_marks[i].results[j].points;
                long long int ct = stud_marks[i].results[j].complete_time;
                //cout << add << " " << stud_id << " " << subj_id << " " << pnts << " " << ct << endl;
                //cout << "Has?" << endl;
                if (add) {
                    //cout << "Has data!" << endl;
                    if (!add_result(stud_id, subj_id, pnts, ct)) {
                        return false;
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
    cout << "|" << endl;
    return true;
}

bool add_subject(int term, std::string name, std::string name_short, bool is_exam) {
    res = stmt->executeQuery("");
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

int add_bot(int bot_num, int term) {
    int kk, bb;
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
    prep_stmt->setInt(2, 0);
    prep_stmt->setBoolean(3, 0);
    prep_stmt->setInt(4, term);
    prep_stmt->setString(5, "Bot #" + to_string(bot_num));
    prep_stmt->setString(6, "N/A");
    //cout << le_query << endl;
    prep_stmt->execute();
    delete prep_stmt;
    delete stmt;
    return kk;
}

void create_bot() {
    stmt = con->createStatement();
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> distr_term(2, 6);
    int terms = distr_term(gen);
    int bb = 0;
    string qr = "SELECT COUNT(*) FROM " + GLOBAL_db_used + ".students WHERE " + GLOBAL_db_used + ".students.is_real = '0';";
    res = stmt->executeQuery(qr);
    if (res->next())
        bb = stoi(res->getString("COUNT(*)"));
    else
        return;
    bb++;
    int kk;
    res = stmt->executeQuery("SELECT COUNT(*) FROM " + GLOBAL_db_used + ".result_cells;");
    res->next();
    kk = stoi(res->getString("COUNT(*)"));

    for (int i = 1; i <= terms; i++) {
        int id = add_bot(bb, i);
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT id FROM " + GLOBAL_db_used + ".subjects WHERE subjects.id < " + to_string((i + 1) * 100) + " AND subjects.id >= " + to_string(i * 100) + ";");
        while (res->next()) {
            kk++;
            std::uniform_int_distribution<int> distr_points(0, 100);
            //res = stmt->executeQuery("select id from djournal.subjects where subj_name = \"Введение в программирование и ЭВМ\" and term_id = 1;");
            string le_query = "INSERT INTO " + GLOBAL_db_used + ".result_cells VALUES ('" + to_string(kk) + "', '" + to_string(id) + "', '" + res->getString("id") + "', '" + to_string(distr_points(gen)) + "', '0');";
            //cout << le_query << endl;
            stmt->execute(le_query);
        }
    }
    delete stmt;
    return;

}

void bot_update() {

    return;
}

vector <tuple<string, int, long long> > get_term_results(int term, string group) {
    vector<tuple<string, int, long long> > term_res;
    stmt = con->createStatement();
    res = stmt->executeQuery("select students.full_name, students.group, subjects.subj_name, subjects.subj_short, result_cells.points, result_cells.change_time from " + GLOBAL_db_used + "subjects inner join " + GLOBAL_db_used + ".result_cells on subjects.id = result_cells.subject_id inner join " + GLOBAL_db_used + ".students on students.id = result_cells.student_id where subject_id >= " + to_string(term) + "00 and subject_id < " + to_string(term + 1) + "00 order by students.id, subjects.id;");
    res->next();

    return term_res;
}

vector <pair <string, string> > get_term_subjects(int term) {
    vector <pair <string, string> > ts;
    stmt = con->createStatement();
    res = stmt->executeQuery("select subjects.subj_name, subjects.subj_short from " + GLOBAL_db_used + ".subjects where subjects.id >= " + to_string(term) + "00 and subjects.id < " + to_string(term + 1) + "00;");
    while (res->next()) {
        string fff = res->getString("subj_short");
        string sss = res->getString("subj_name");
        ts.push_back(make_pair(fff, sss));
    }
    return ts;
}

tuple<string, string, string, long long, string> get_last_success(int term) {
    stmt = con->createStatement();
    res = stmt->executeQuery("select students.full_name, students.group, subjects.subj_name, result_cells.points, subjects.is_exam, result_cells.change_time from " + GLOBAL_db_used + ".subjects inner join " + GLOBAL_db_used + ".result_cells on subjects.id = result_cells.subject_id inner join " + GLOBAL_db_used + ".students on students.id = result_cells.student_id where (subject_id >= " + to_string(term) + "00 and subject_id < " + to_string(term + 1) + "00 and result_cells.points >= 60 and result_cells.change_time > 1000000) order by result_cells.change_time desc limit 1;");
    res->next();
    string mrk = get_mark(res->getDouble("points"), res->getBoolean("is_exam")).second;
    if (mrk == "A")
        mrk = "отл. A";
    if (mrk == "B" || mrk == "C")
        mrk = "хор. " + mrk;
    if (mrk == "D" || mrk == "E")
        mrk = "удовл. " + mrk;
    if (mrk == "ok")
        mrk = "зачёт";
    tuple<string, string, string, long long, string> tp = make_tuple(res->getString("full_name"), res->getString("group"), res->getString("subj_name"), res->getInt("change_time"), mrk);
    return tp;
}

std::vector<stud_row> get_all_the_students();