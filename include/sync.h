#ifndef SYNC_H
#define SYNC_H
#include <pthread.h>

#define Quantum 3
#define Cant_cpus 2
#define tiempo 120
typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;
#endif