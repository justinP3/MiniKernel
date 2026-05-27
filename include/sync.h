#ifndef SYNC_H
#define SYNC_H
#include <pthread.h>

typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;
typedef pthread_t thread_t;

#endif