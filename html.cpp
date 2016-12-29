#include <iostream>
#include <stdio.h>
#include "handler.h"
#include "de.h"
#include "global.h"
#include <string>

//
// Created by rmnsv on 19.09.16.
//

using namespace std;

void html_print_term(string name, std::vector<stud_row> printed, int term) {
    int year = _year;
    string define_filename = "./html/year" + to_string(year) + "_term" + to_string(term) + ".html";
    freopen(define_filename.c_str(), "w", stdout);
    int total = 0;
    printf("<!DOCTYPE html>\n"
                   "<html>\n"
                   "<head>\n"
                   "  <title>DebtC year2014</title>\n"
                   "  <meta charset=\"UTF-8\">\n"
                   "  <link rel=\"stylesheet\" type=\"text/css\" href=\"./style.css\">\n"
                   "</head>\n"
                   "<body>\n"
                   "  <p>DebtC by rmnsv</p>\n"
                   "  <h2><p>%d курс (year%d). %d семестр.</p></h2>\n", term / 2, year, term);
    printf("  <p>В ПРОЦЕССЕ: 162720:00 из 214560:00</p>\n"
                   "  <p>СЕССИЯ: 0:00 из 43200:00</p>\n"
                   "  <p>Последняя успешная сдача: Гончар А. В., Автоматное программирование, 15.01.2016 (NULL), зачёт<br><br></p>\n"
                   "  <table align=\"center\">\n"
                   "  <tbody>\n"
                   "  <tr style=\"border-bottom: 1px solid black;\">\n"
                   "    <th>Место</th>\n"
                   "    <th>Студент</th>\n");
    for (int i = 0; i < 20; i++) {
        if (printed[term - 1].results[i].has_data) {

            printf("<th class=\"cell\">%s</th>\n", printed[term - 1].results[i].subj_name.substr(0, 6).c_str());
            if (printed[term - 1].results[i].points >= 60) {
                total++;
            }
        } else {

        }
    }
    printf( "   <th class=\"eq\">=</th>\n"
                    "    <th class=\"time_col\">Время</th>\n"
                    "  </tr>\n"
                    "  <tr>\n"
                    "    <td>1</td>\n"
                    "    <td class=\"stud\">%s</td>\n", name.c_str());
    for (int i = 0; i < 20; i++) {
        if (printed[term - 1].results[i].has_data) {
            pair<char, string> got_mark = get_mark(printed[term - 1].results[i].points, printed[term - 1].results[i].is_exam);
            string x = "";
            if (got_mark.first == 'F')
                x = "X";
            printf("<td class=\"subj_%s\">%c%s</td>\n", got_mark.second.c_str(), got_mark.first, x.c_str());
        }
    }
    printf("    <td>%d</td>\n"
                   "    <td>0</td>\n"
                   "  </tr>", total);
    printf("  </tbody>\n"
                   "  </table>\n"
                   "</body>\n"
                   "</html>");
    fclose(stdout);
}

void html_create_test(string name, std::vector<stud_row> printed, int term) {
    //string define_filename = "./html/year" + year + "_term" + term + ".html";
    freopen("./html/year.html", "w", stdout);
    int total = 0;
    printf("<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
            "  <title>DebtC year2014</title>\n"
            "  <meta charset=\"UTF-8\">\n"
            "  <link rel=\"stylesheet\" type=\"text/css\" href=\"./style.css\">\n"
            "</head>\n"
            "<body>\n"
            "  <p>DebtC by rmnsv</p>\n"
            "  <h2><p>3 курс (year2014). 5 семестр (нихуя не пятый, на самом деле).</p></h2>\n"
            "  <p>В ПРОЦЕССЕ: 162720:00 из 214560:00</p>\n"
            "  <p>СЕССИЯ: 0:00 из 43200:00</p>\n"
            "  <p>Последняя успешная сдача: Гончар А. В., Автоматное программирование, 15.01.2016 (NULL), зачёт<br><br></p>\n"
            "  <table align=\"center\">\n"
            "  <tbody>\n"
            "  <tr style=\"border-bottom: 1px solid black;\">\n"
            "    <th>Место</th>\n"
            "    <th>Студент</th>\n");
    for (int i = 0; i < 20; i++) {
        if (printed[term - 1].results[i].has_data) {

            printf("<th class=\"cell\">%s</th>\n", printed[term - 1].results[i].subj_name.substr(0, 6).c_str());
            if (printed[term - 1].results[i].points >= 60) {
                total++;
            }
        } else {

        }
    }
    printf( "   <th class=\"eq\">=</th>\n"
            "    <th class=\"time_col\">Время</th>\n"
            "  </tr>\n"
            "  <tr>\n"
            "    <td>1</td>\n"
            "    <td class=\"stud\">%s</td>\n", name.c_str());
    for (int i = 0; i < 20; i++) {
        if (printed[term - 1].results[i].has_data) {
            pair<char, string> got_mark = get_mark(printed[term - 1].results[i].points, printed[term - 1].results[i].is_exam);
            string x = "";
            if (got_mark.first == 'F')
                x = "X";
            printf("<td class=\"subj_%s\">%c%s</td>\n", got_mark.second.c_str(), got_mark.first, x.c_str());
        }
    }
    printf("    <td>%d</td>\n"
                   "    <td>0</td>\n"
                   "  </tr>", total);
    printf("  </tbody>\n"
                   "  </table>\n"
                   "</body>\n"
                   "</html>");
    fclose(stdout);
}