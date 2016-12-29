//
// Created by rmnsv on 19.09.16.
//

#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <cstring>
#include <iostream>
#include <iconv.h>
#include <vector>

using namespace std;

string json_return;
string student_name;


class IConv {
    iconv_t ic_;
public:
    IConv(const char* to, const char* from)
            : ic_(iconv_open(to,from))    { }
    ~IConv() { iconv_close(ic_); }

    bool convert(char* input, char* output, size_t& out_size) {
        size_t inbufsize = strlen(input)+1;// s-jis string should be null terminated,
        // if s-jis is not null terminated or it has
        // multiple byte chars with null in them this
        // will not work, or to provide in other way
        // input buffer length....
        return iconv(ic_, &input, &inbufsize, &output, &out_size);
    }
};

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c < size*nmemb; c++)
    {
        json_return.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

size_t writeCallback_name(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    char stud_n[size*nmemb];

    IConv ic("UTF8","CP1251");

    size_t osz = size*nmemb;

    ic.convert(buf, stud_n, osz);
    for (int c = 0; c < size * nmemb; c++) {
        student_name.push_back(stud_n[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

pair<string, string> de_data_get(string de_lg, string de_ps) {

    CURL *ch;
    CURLcode res;

    curl_slist *cooki;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    //distributedCDE?Rule=LOGON&LOGIN=191793&PASSWD=fun003&loginbutton=Войти

    /* get a curl handle */
    ch = curl_easy_init();
    string reqe = "https://de.ifmo.ru/servlet/distributedCDE?Rule=LOGON&LOGIN=" + de_lg + "&PASSWD=" + de_ps + "&loginbutton=Войти";
    string reqe2 = "https://de.ifmo.ru/servlet/distributedCDE?Rule=APPLYSECURITYGROUP&PERSON=" + de_lg + "&SECURITYGROUP=8&COMPNAME=";
    string reqe3 = "https://de.ifmo.ru/servlet/distributedCDE?Rule=ShowWorkSpace";

    if(ch) {
        char url[1024];
        char url2[1024];
        char url3[1024];
        strcpy(url, reqe.c_str());
        strcpy(url2, reqe2.c_str());
        strcpy(url3, reqe3.c_str());
        char agent[] = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.0.3705; .NET CLR 1.1.4322)";

        curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);   //TIMEOUT
        curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(ch, CURLOPT_VERBOSE, true);
        curl_easy_setopt(ch, CURLOPT_TRANSFER_ENCODING, true);
        curl_easy_setopt(ch, CURLOPT_COOKIEFILE, "");
        //curl_easy_setopt(ch, CURLOPT_COOKIEJAR, '.');
        //curl_easy_setopt(ch, CURLOPT_COOKIE, "route=add37689fc578b763a27f46a625fec72;JSESSIONID=2E4BE99B2C18B911DB1A195251457935.academicnt1;path=/;Secure");
        curl_easy_setopt(ch, CURLOPT_USERAGENT, agent);
        curl_easy_setopt(ch, CURLOPT_URL, url);


        res = curl_easy_perform(ch);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));


        curl_easy_getinfo(ch, CURLINFO_COOKIELIST, &cooki);

        int i = 1;
        while (cooki) {
            printf("[%d]: %s\n", i, cooki->data);
            cooki = cooki->next;
            i++;
        }

        curl_easy_setopt(ch, CURLOPT_URL, "https://de.ifmo.ru/api/private/eregister/");
        curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, &writeCallback);

        res = curl_easy_perform(ch);


        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_setopt(ch, CURLOPT_URL, url2);
        curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, &writeCallback_name);

        res = curl_easy_perform(ch);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_setopt(ch, CURLOPT_URL, url3);
        curl_easy_setopt(ch, CURLOPT_ENCODING, "Windows-1251");
        curl_easy_setopt(ch, CURLOPT_CONV_FROM_NETWORK_FUNCTION, &writeCallback_name);
        curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, &writeCallback_name);

        res = curl_easy_perform(ch);



        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        size_t name_b, name_e;
        name_b = student_name.find("id=\"fio\"") + 21;
        name_e = student_name.find("<", name_b - 2);
        student_name = student_name.substr(name_b, name_e - name_b);
        cout << "NAME: " << student_name << endl;

        curl_easy_cleanup(ch);
    }
    curl_global_cleanup();


    //freopen("yourDE.txt", "w", stdout);
    return make_pair(json_return, student_name);
}