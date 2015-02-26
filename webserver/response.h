void send_status(FILE *client, int code, const char *reason_phrase);
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body);
void send_response_fd(FILE *client, int fd, int code, const char *reason_phrase);
