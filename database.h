//
// Created by rmnsv on 13.10.16.
//


#ifndef DEBTC_DATABASE_H
#define DEBTC_DATABASE_H


#include <tuple>
#include "handler.h"

std::string test_query(std::string query);

bool ctdd_connect(std::string hostname, int port, std::string db_login, std::string db_password);

bool ctdd_disconnect();

bool add_student(int isu_num, std::string name, std::string group, int term);

bool add_result(int student_id, int subject_id, double points, long long update_time);

bool new_student(std::vector<stud_row> &stud_marks, std::string isu_num);

bool add_subject(int term, std::string name, std::string name_short, bool is_exam);

int how_many_students();

int get_subj_id(std::string subj_name, int term);

int add_bot(int bot_num, int term);

void create_bot();

void bot_update();

vector <tuple<string, int, double, long long> > get_term_results(int term);

vector <pair <string, string> > get_term_subjects(int term);

tuple<string, string, string, long long, string> get_last_success(int term);

result_cell get_result(int student_id, int subject_id);

stud_row get_all_the_line(int student_id);

std::vector<stud_row> get_all_the_students();

#endif //DEBTC_DATABASE_H
