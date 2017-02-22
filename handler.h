//
// Created by rmnsv on 13.10.16.
//

#ifndef DEBTC_HANDLER_H
#define DEBTC_HANDLER_H

#include <string>
#include <vector>
#include "structs.h"


void init_sem_dates();

long long to_minutes(std::string data);

vector<stud_row> parse_default(std::string &json_data, std::string student_name);

void print_student_marks_default(std::vector<stud_row> stud_marks);

bool marks_login_update(std::vector<stud_row> marks);

std::vector<stud_row> * stud_vector_no_data(std::vector<stud_row> &clean_it);

std::pair<int, int> date_to_minutes(std::string date);

std::pair<char, std::string> get_mark(double points, bool is_exam);

std::vector<stud_row> sort_by_passes(std::vector<stud_row> stud_vector, int from, int to);

std::vector<stud_row> sort_by_marks(std::vector<stud_row> stud_vector, int from, int to);

std::vector<stud_row> sort_by_time(std::vector<stud_row> stud_vector, int form, int to);

#endif //DEBTC_HANDLER_H
