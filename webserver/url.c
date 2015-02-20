#include <string.h>
#include <strings.h>

/* Analyzes and returns the URL in a valid format or NULL */
char *rewrite_url(char *url) {
    int length = strlen(url);
    char *pos = "";

    printf("url = %s\n", url);
    /* Do nothing if URL is empty or not absolute or contains "/.." or "//" */
    if (length < 1 || url[0] != '/' || strstr(url, "/..") || strstr(url, "//"))
        return NULL;

    /* Remove '/' at the beginning of the URL */
    url = &url[1];

    /* Ignore everything starting at the '?' character */
    if ((pos = index(url, '?'))  != NULL)
        *pos = '\0';

    length = strlen(url);

    /* Remove '/' at the end of the URL */
    if (url[length] == '/')
        url[length] = '\0';

    return url;
}
