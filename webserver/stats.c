#include <stdlib.h>
#include <sys/mman.h>
#include "stats.h"

static web_stats *stats;

int init_stats(void) {
    if ((stats = mmap(NULL, sizeof(web_stats), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
        return -1;

    /* No need to initialize stats, mmap already set it to 0 */

    if (sem_init(&(stats->semaphore), 1, 1) == -1)
        exit(-1);

    return 0;
}

int update_stats(int field) {
    sem_wait(&(stats->semaphore));

    switch (field) {
        case CON:
            ++stats->served_connections;
            break;
        case REQ:
            ++stats->served_requests;
            break;
        case OK:
            ++stats->ok_200;
            break;
        case BADREQ:
            ++stats->ko_400;
            break;
        case FORB:
            ++stats->ko_403;
            break;
        case NOTF:
            ++stats->ko_404;
            break;
        case METHD:
            ++stats->ko_405;
            break;
        case HTTPVERS:
            ++stats->ko_505;
            break;
        default:
            return -1;
    }

    sem_post(&(stats->semaphore));

    return 0;
}

web_stats *get_stats(void) {
    return stats;
}
