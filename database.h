//
// Created by rmnsv on 13.10.16.
//


#ifndef DEBTC_DATABASE_H
#define DEBTC_DATABASE_H


#include "handler.h"

std::string test_query(std::string query);

bool ctdd_connect(std::string hostname, int port, std::string db_login, std::string db_password);

bool ctdd_disconnect();

bool add_student(int isu_num, std::string name, std::string group, int term);

bool new_student(std::vector<stud_row> stud_marks, std::string isu_num);

bool add_subject(int term, std::string name, std::string name_short, bool is_exam);

bool add_result(int student_id, int subject_id, double points, long long update_time);

bool add_bot();

void bot_update();

result_cell get_result(int student_id, int subject_id);

stud_row get_all_the_line(int student_id);

std::vector<stud_row> get_all_the_students();

#endif //DEBTC_DATABASE_H
