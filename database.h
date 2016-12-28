//
// Created by rmnsv on 13.10.16.
//


#ifndef DEBTC_DATABASE_H
#define DEBTC_DATABASE_H


#include "handler.h"

std::string test_query(std::string query);

bool ctdd_connect();

bool ctdd_disconnect();

bool add_student(int st_id, int isu_num, std::string name, int term);

bool add_subject(int term, std::string name, std::string name_short, bool is_exam);

bool add_result(int student_id, int subject_id, double points, long long update_time);

bool add_bot();

void bot_update();

result_cell get_result(int student_id, int subject_id);

stud_row get_all_the_line(int student_id);

#endif //DEBTC_DATABASE_H
