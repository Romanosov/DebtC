//
// Created by rmnsv on 10.01.17.
//

#ifndef DEBTC_STRUCTS_H
#define DEBTC_STRUCTS_H

#include <string>
#include <iostream>
using namespace std;

struct result_cell {
    int subj_id;
    bool has_data;
    std::string subj_name;
    bool is_exam;
    double points;
    long long complete_time;
    std::string complete_data;

    result_cell(){

    }

    ~result_cell() {
        //cout << "CELL" << endl;
    }
};

struct stud_row {
    int student_id;
    int term;
    std::string name;
    std::string group;
    result_cell results[20];

    stud_row() {
    }

    ~stud_row() {
        //cout << "?" << endl;
    }
};

#endif //DEBTC_STRUCTS_H
