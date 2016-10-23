//
// Created by rmnsv on 13.10.16.
//

#ifndef DEBTC_HANDLER_H
#define DEBTC_HANDLER_H

#include <string>

struct result_cell {
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
    result_cell results[20];
    int color;
};

#endif //DEBTC_HANDLER_H
