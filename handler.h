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
};

struct stud_row {
    int student_id;
    int position;
    int term;
    std::string name;
    std::string group;
    result_cell results[20];
};

void create_bot();

std::vector<stud_row> parse_default(std::string json_data);

void print_student_marks_default(std::vector<stud_row> stud_marks);

#endif //DEBTC_HANDLER_H
