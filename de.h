//
// Created by rmnsv on 23.10.16.
//

#include <stdio.h>
#include <string>

#ifndef DEBTC_DE_H
#define DEBTC_DE_H

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);

std::string de_data_get(std::string de_lg, std::string de_ps);

#endif //DEBTC_DE_H
