//
// Created by rmnsv on 19.09.16.
//

#include <string>
#include <iostream>
#include "handler.h"
#include "database.h"
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include "global.h"
#include <ctime>
#include <chrono>
#include <cstring>

using namespace std;

int yyear;

pair<string, string> sem_date[10];

void init_sem_dates() {
    sem_date[0].first = "01.09.2013";
    sem_date[0].second = "31.01.2014";
    sem_date[1].first = "08.02.2014";
    sem_date[1].second = "30.06.2014";
    sem_date[2].first = "01.09.2014";
    sem_date[2].second = "31.01.2015";
    sem_date[3].first = "08.02.2015";
    sem_date[3].second = "02.07.2015";
    sem_date[4].first = "01.09.2015";
    sem_date[4].second = "31.01.2016";
    sem_date[5].first = "08.02.2016";
    sem_date[5].second = "30.06.2016";
    sem_date[6].first = "01.09.2016";
    sem_date[6].second = "29.01.2017";
    sem_date[7].first = "06.02.2017";
    sem_date[7].second = "30.06.2017";
}

long long to_minutes(string data) {
    std::tm tm = {};
    char * ff;
    data += " 18:23:00.000000";
    strcpy(ff, data.c_str());
    const char* snext = ::strptime(ff, "%d.%m.%Y %H:%M:%S", &tm);
    auto time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    long long  duration_ms = time_point.time_since_epoch() / std::chrono::milliseconds(1) + std::atof(snext);
    return duration_ms / 10000;
}

vector<stud_row> parse_default(string &json_data, string student_name) {

    GLOBAL_student_name = student_name;

    init_sem_dates();

    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    vector<stud_row> stud_marks_ready;
    int study_year;
    string buffer_string;
    size_t found = 0;
    int vector_resize_done = false;

    //cout << json_data << endl;

    //cout << json_data[46110] << endl;

    vector<pair<string, int> > terms_data;

    int terms_amount = 20;
    int m = 1;
    int h = 0;

    stud_marks_ready = stud_vector_no_data(stud_marks_ready);

    while (m <= terms_amount / 2) {
        string grupk;
        found = json_data.find("group", found);
        if (found != string::npos) {
            buffer_string = "";
            int i = 0;
            char pl;
            while ((pl = json_data[found + 8 + i]) != '\"') {
                buffer_string += pl;
                i++;
            }
            grupk = buffer_string;
            //cout << "GROUP: " << grupp << endl;
        }
        found = json_data.find("studyyear", found);
        if (found != string::npos) {
            buffer_string = "";
            for (int i = 0; i < 4; i++) {
                buffer_string += json_data[found + 12 + i];
            }
            study_year = stoi(buffer_string);
            //cout << "YEAR: " << study_year << '\n';
            terms_data.push_back(make_pair(grupk, study_year));
            terms_data.push_back(make_pair(grupk, study_year + 1));
            if (!vector_resize_done) {
                int current_year = aTime->tm_year + 1900;
                int current_month = aTime->tm_mon;
                if ((current_month == 0) || (current_month <= 7))
                    terms_amount = (current_year - study_year) * 2;
                else
                    terms_amount = (current_year - study_year + 1) * 2;
                //cout << "TERMS: " << terms_amount << endl;
                stud_marks_ready.resize(terms_amount);
                vector_resize_done = true;
            }
        }
        m++;
    }

    GLOBAL_year = terms_data[0].second;

    found = 0;

    int current_term = 0;
    vector<int> k(terms_amount, 0);

        while (current_term <= terms_amount) {

            string subj_data;
            size_t found_subj_end = 0;
            size_t found_subj = 0;
            size_t found_term = 0;

            found_term = json_data.find("semester", found);
            if (found_term != string::npos) {
                buffer_string = "";
                int i = 0;
                char pl;
                while ((pl = json_data[found_term + 11 + i]) != '\"') {
                    buffer_string += pl;
                    i++;
                }
                int what_term = stoi(buffer_string);
                if (what_term != current_term) {
                    current_term = what_term;
                    stud_marks_ready[current_term - 1].name = student_name;
                    stud_marks_ready[current_term - 1].group = terms_data[current_term - 1].first;
                    stud_marks_ready[current_term - 1].student_id = GLOBAL_students_total + current_term;
                    //cout << endl << terms_data[current_term - 1].first << ", year " << terms_data[current_term - 1].second << "..." << endl;
                    continue;
                }
                stud_marks_ready[current_term - 1].term = current_term;
            }


            //cout << stud_marks_ready[current_term - 1].name << endl;

            ++k[current_term - 1];
            //cout << k[current_term - 1] << ")" << endl;

            found = json_data.find("name", found);
            if (found != string::npos) {
                buffer_string = "";
                int i = 0;
                char pl;
                while ((pl = json_data[found + 7 + i]) != '\"') {
                    buffer_string += pl;
                    i++;
                }
                stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].subj_name = buffer_string;
                stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].subj_id = get_subj_id(buffer_string, current_term);
                //string what_for = stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].subj_name;
                cout.flush();
                cout << "\r" << stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].subj_name << " — семестр " << current_term;

                size_t found_no_data = json_data.find("name", found + 10);

                found_subj_end = json_data.find("value", found);

                if (found_no_data < found_subj_end) {
                    found = found_no_data;
                    //cout << endl;
                    //usleep(1000000);
                    continue;
                }

                found_subj_end += 12;

                if (found_subj_end != string::npos) {
                    subj_data = json_data.substr(found, found_subj_end - found);
                    //cout << subj_data << endl;
                }
            }

                //cout << "TERM: " << current_term << endl;

            found_subj = subj_data.find("markdate", found_subj);
            if (found_subj != string::npos) {
                buffer_string = "";
                int i = 0;
                char pl;
                while ((pl = subj_data[found_subj + 11 + i]) != '\"') {
                    buffer_string += pl;
                    i++;
                }
                if (buffer_string != "") {
                    stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].complete_data = buffer_string;
                    stud_marks_ready[current_term -1].results[k[current_term -1]].complete_time = to_minutes(buffer_string) - to_minutes(sem_date[7 - terms_amount + current_term].first);
                    //cout << "DATE: " << buffer_string << endl;
                }
            }


            found_subj = subj_data.find("worktype", found_subj);
            if (found_subj != string::npos) {
                buffer_string = "";
                int i = 0;
                char pl;
                while ((pl = subj_data[found_subj + 11 + i]) != '\"') {
                    buffer_string += pl;
                    i++;
                }
                if (buffer_string != "") {
                    if (buffer_string == "Зачет")
                        stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].is_exam = false;
                    if (buffer_string == "Экзамен")
                        stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].is_exam = true;
                    //cout << "IS EXAM: " << stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].is_exam << endl;
                }
            }


            found_subj = subj_data.find("value", found_subj);
            string bs2;
            if (found_subj != string::npos) {
                bs2 = "";
                int i = 0;
                char pl;
                while (((pl = subj_data[found_subj + 8 + i]) >= '0' && pl <= '9') || (pl == ','))  {
                    i++;
                    if (pl == ',') {
                        bs2 += '.' + '\0';
                    } else {
                        bs2 += pl + '\0';
                    }
                }

                //cout << bs2 << endl;
                if (bs2 != "") {
                    stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].points = stod(bs2);
                    //cout << "POINTS: " << stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].points << endl;
                    stud_marks_ready[current_term - 1].results[k[current_term - 1] - 1].has_data = true;
                    bs2 = "";
                }
            }


            if (found_subj_end < found)
                break;

            found = found_subj_end;
            //cout << endl;
        }

    //cout << "GOT NAME: " << stud_marks_ready[4].name << endl;

    cout  << "\rGot some data...                                                                                                  " << endl;

    GLOBAL_students_total += terms_amount;

    return stud_marks_ready;
}

int get_year() {
    return yyear;
}

bool marks_login_update(vector<stud_row> marks) {
    bool ok = true;
    if (!ctdd_connect("localhost", 3306, "root", ""))
        return false;
    for (int i = 0; i < marks.size(); i++) {
        for (int j = 0; j < 20; j++) {
            if (marks[i].results[j].has_data) {
                if (!add_result(marks[i].student_id, marks[i].results[j].subj_id, marks[i].results[j].points, marks[i].results[j].complete_time))
                    ok = false;
            }
        }
    }
    return ok;
}


vector<stud_row> stud_vector_no_data(vector<stud_row> clean_it) {
    for (int i = 0; i < clean_it.size(); i++) {
        for (int j = 0; j < 20; j++) {
            clean_it[i].results[j].has_data = false;
        }
    }
    return clean_it;
}

void print_student_marks_default(std::vector<stud_row> stud_marks) {
    for (int i = 0; i < stud_marks.size(); i++) {
        cout << "" << stud_marks[i].term << ", " << stud_marks[i].group << ":" << endl << endl;
        int k = 0;
        for (int j = 0; j < 20; j++) {
            if (stud_marks[i].results[j].has_data) {
                k++;
                cout << k << ") " << stud_marks[i].results[j].subj_name << endl;
                cout << "is_exam: " << stud_marks[i].results[j].is_exam << endl;
                cout << stud_marks[i].results[j].points << " points" << endl;
                cout << stud_marks[i].results[j].complete_time << endl << endl;
                usleep(1000000);
            }
        }
    }
    return;
}

pair<char, string> get_mark(double points, bool is_exam) {
    if (is_exam) {
        if (points < 60) {
            return make_pair('F', "FX");
        }
        if ((points >= 60) && (points < 68)) {
            return make_pair('E', "E");
        }
        if ((points >= 68) && (points < 75)) {
            return make_pair('D', "D");
        }
        if ((points >= 75) && (points < 84)) {
            return make_pair('C', "C");
        }
        if ((points >= 84) && (points < 91)) {
            return make_pair('B', "B");
        }
        if ((points >= 91) && (points <= 100)) {
            return make_pair('A', "A");
        }
        if ((points == NULL) || (points > 100)) {
            return make_pair('?', "un");
        }
    } else {
        if (points < 60) {
            return make_pair('-', "fail");
        }
        if (points >= 60) {
            return make_pair('+', "ok");
        }
        if ((points == NULL) || (points > 100)) {
            return make_pair('?', "un");
        }
    }
}

vector<stud_row> sort_by_passes(vector<stud_row> stud_vector, int from, int to) {

}

vector<stud_row> sort_by_marks(vector<stud_row> stud_vector, int from, int to) {

}

vector<stud_row> sort_by_time(vector<stud_row> stud_vector, int from, int to) {

}