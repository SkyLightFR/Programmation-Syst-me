#include <semaphore.h>
#define CON 1
#define REQ 2
#define OK 3
#define BADREQ 4
#define FORB 5
#define NOTF 6
#define METHD 7
#define HTTPVERS 8

typedef struct {
    int served_connections;
    int served_requests;
    int ok_200;
    int ko_400;
    int ko_403;
    int ko_404;
    int ko_405;
    int ko_505;
    sem_t semaphore;
} web_stats;

int init_stats(void);
int update_stats(int field);
web_stats *get_stats(void);
