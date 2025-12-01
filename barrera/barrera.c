#include "barrera.h"

void barrera_init(barrera_t* b, int N) {
    b->count = 0;
    b->N = N;
    b->etapa = 0;
    pthread_mutex_init(&b->m, NULL);
    pthread_cond_init(&b->cv, NULL);
}

void barrera_wait(barrera_t* b) {
    pthread_mutex_lock(&b->m);

    int etapa_local = b->etapa;
    b->count++;

    if (b->count < b->N) {
        while (etapa_local == b->etapa)
            pthread_cond_wait(&b->cv, &b->m);
    } else {
        b->etapa++;
        b->count = 0;
        pthread_cond_broadcast(&b->cv);
    }

    pthread_mutex_unlock(&b->m);
}

void barrera_destroy(barrera_t* b) {
    pthread_mutex_destroy(&b->m);
    pthread_cond_destroy(&b->cv);
}
