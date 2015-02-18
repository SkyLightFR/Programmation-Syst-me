#include <string.h>

/* Removes the query part of the URL */
char *rewrite_url(char *url) {
    char formatted_url[2000];
    unsigned int i;

    /* Keep only one '/' at the beginning of the URL */
    for (i = 1 ; i < strlen(url) && url[i] == '/' ; ++i);
    if (i > 1) {
        strncpy(&formatted_url[1], &url[i], strlen(url));
        strcpy(url, formatted_url);
    } else
        strcpy(url, &url[1]);

    /* Delete every occurence of ../ at the beginning of the URL */
    i = 1;
    while (!strncmp(&url[i], "../", 3))
        i+=3;
    if (!strncmp(&url[i], "..", 2))
        i+=2;
    strcat(&formatted_url[1], &url[i]);

    /* Remove everything after the '?' symbol */
    for (i = 0 ; i < strlen(url) && url[i] != '?' ; ++i);
    if (i != strlen(url)) {
        strncpy(formatted_url, url, i);
        strcpy(url, formatted_url);
    }

    return url;
}
