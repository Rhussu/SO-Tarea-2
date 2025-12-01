#ifndef BARRERA_H
#define BARRERA_H

#include <pthread.h>

typedef struct {
    int count;
    int N;
    int etapa;
    pthread_mutex_t m;
    pthread_cond_t cv;
} barrera_t;

void barrera_init(barrera_t* b, int N);
void barrera_wait(barrera_t* b);
void barrera_destroy(barrera_t* b);

#endif
