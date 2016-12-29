//
// Created by rmnsv on 28.12.16.
//

#ifndef DEBTC_HTML_H
#define DEBTC_HTML_H

#include "handler.h"

void html_print_top(std::vector<stud_row> printed, int term);

void html_print_term(std::string name, std::vector<stud_row> printed, int term);

void html_print_bottom(std::vector<stud_row> printed, int term);

void html_print_page(int year, int term);

void html_create_test(std::string name, std::vector<stud_row> printed, int term);

#endif //DEBTC_HTML_H
