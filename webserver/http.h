#define MAX_MSG_LENGTH 200

enum http_method {
    HTTP_GET,
    HTTP_UNSUPPORTED,
};

typedef struct
{
    enum    http_method method;
    int     major_version;
    int     minor_version;
    char    *url;
} http_request;

int parse_http_request(const char *request_line, http_request *request);
void skip_headers(FILE *client);
int compile_regex(void);
