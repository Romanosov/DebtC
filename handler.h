//
// Created by rmnsv on 13.10.16.
//

#ifndef DEBTC_HANDLER_H
#define DEBTC_HANDLER_H

#include <string>
#include <vector>

struct result_cell {
    int subj_id;
    bool has_data;
    std::string subj_name;
    bool is_exam;
    double points;
    long long complete_time;
    std::string complete_data;
};

struct stud_row {
    int student_id;
    int term;
    std::string name;
    std::string group;
    result_cell results[20];
};

int get_year();

void create_bot();

std::vector<stud_row> parse_default(std::string json_data, std::string student_name);

void print_student_marks_default(std::vector<stud_row> stud_marks);

bool marks_login_update(std::vector<stud_row> marks);

bool marks_login_update_test(std::vector<stud_row> marks);

std::vector<stud_row> stud_vector_no_data(std::vector<stud_row> clean_it);

std::pair<int, int> date_to_minutes(std::string date);

std::pair<char, std::string> get_mark(double points, bool is_exam);

std::vector<stud_row> sort_by_passes(std::vector<stud_row> stud_vector, int from, int to);

std::vector<stud_row> sort_by_marks(std::vector<stud_row> stud_vector, int from, int to);

std::vector<stud_row> sort_by_time(std::vector<stud_row> stud_vector, int form, int to);

#endif //DEBTC_HANDLER_H
