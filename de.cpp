//
// Created by rmnsv on 19.09.16.
//

#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <cstring>

using namespace std;

string json_return;

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

string de_data_get(string de_lg, string de_ps) {


    CURL *ch;
    CURLcode res;

    curl_slist *cooki;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    //distributedCDE?Rule=LOGON&LOGIN=191793&PASSWD=fun003&loginbutton=Войти

    /* get a curl handle */
    ch = curl_easy_init();
    string reqe = "https://de.ifmo.ru/servlet/distributedCDE?Rule=LOGON&LOGIN=" + de_lg + "&PASSWD=" + de_ps + "&loginbutton=Войти";


    if(ch) {
        char url[1024];
        strcpy(url, reqe.c_str());
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

        curl_easy_cleanup(ch);
    }
    curl_global_cleanup();

    //freopen("yourDE.txt", "w", stdout);
    return json_return;
}