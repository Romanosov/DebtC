#include <iostream>

using namespace std;

struct subj {
    int subj_id;
    bool has_data;
    bool complete;
    short points;
    long long complete_time;
};

struct stud_row {
    int student_id;
    int position;
    string name;
    string group;
    subj results[20];
    int color;
};

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}