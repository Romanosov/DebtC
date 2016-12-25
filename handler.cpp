//
// Created by rmnsv on 19.09.16.
//

#include <string>
#include <iostream>
#include "handler.h"
#include <time.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

vector<stud_row> parse_default(string json_data) {
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    vector <stud_row> stud_marks_ready;
    stud_row pushed;
    string buffer_string;
    int study_year;
    size_t found = 0;
    int vector_resize_done = false;
    size_t parse_from = 0;

    cout << json_data << endl;

    //cout << json_data[46110] << endl;

    //while (found <= json_data.size()) {
        found = json_data.find("group", found);
        if (found != string::npos) {
            buffer_string = "";
            int i = 0;
            char pl;
            while ((pl = json_data[found + 8 + i]) != '\"') {
                buffer_string += pl;
                i++;
            }
            pushed.group = buffer_string;
            cout << "GROUP: " << pushed.group << endl;
        }
        found = json_data.find("studyyear", found);
        if (found != string::npos) {
            buffer_string = "";
            for (int i = 0; i < 4; i++) {
                buffer_string += json_data[found + 12 + i];
            }
            study_year = stoi(buffer_string);
            cout << "YEAR: " << study_year << '\n';
            if (!vector_resize_done) {
                int current_year = aTime->tm_year + 1900;
                int terms_amount = (current_year - study_year + 1) * 2;
                cout << "TERMS: " << terms_amount << endl;
                stud_marks_ready.resize(terms_amount);
                vector_resize_done = true;
            }

        }
        int current_term = 1;
        int k = 0;

        while (current_term <= 6) {

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
                    k = 0;
                    //found = parse_from;
                    continue;
                }
                pushed.term = current_term;
            }

            cout << ++k << ")" << endl;

            found = json_data.find("name", found);
            if (found != string::npos) {
                buffer_string = "";
                int i = 0;
                char pl;
                while ((pl = json_data[found + 7 + i]) != '\"') {
                    buffer_string += pl;
                    i++;
                }
                pushed.results[k - 1].subj_name = buffer_string;
                cout << "NAME: " << pushed.results[k - 1].subj_name << '\n';

                size_t found_no_data = json_data.find("name", found + 10);

                found_subj_end = json_data.find("value", found);

                if (found_no_data < found_subj_end) {
                    found = found_no_data;
                    cout << endl;
                    usleep(1000000);
                    continue;
                }

                found_subj_end += 12;

                if (found_subj_end != string::npos) {
                    subj_data = json_data.substr(found, found_subj_end - found);
                    //cout << subj_data << endl;
                }
            }
                cout << "TERM: " << current_term << endl;

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
                    //pushed.results[k - 1].complete_time = to_minutes(buffer_string);
                    cout << "DATE: " << buffer_string << endl;
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
                        pushed.results[k - 1].is_exam = false;
                    if (buffer_string == "Экзамен")
                        pushed.results[k - 1].is_exam = true;
                    cout << "IS EXAM: " << pushed.results[k - 1].is_exam << endl;
                }
            }

            found_subj = subj_data.find("value", found_subj);
            if (found_subj != string::npos) {
                buffer_string = "";
                int i = 0;
                char pl;
                while ((pl = subj_data[found_subj + 8 + i]) != '\"') {
                    buffer_string += pl;
                    i++;
                }
                if (buffer_string != "") {
                    replace(buffer_string.begin(), buffer_string.end(), ',', '.');
                    pushed.results[k - 1].points = stod(buffer_string);
                    cout << "POINTS: " << pushed.results[k - 1].points << endl;
                }
            }

            if (found_subj_end < found)
                break;

            found = found_subj_end;
            cout << endl;
            usleep(1000000);
        }
    //}

    return stud_marks_ready;
}

void print_student_marks_default(std::vector<stud_row> stud_marks) {
    return;
}
