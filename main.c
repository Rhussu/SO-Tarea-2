#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "barrera.h"

typedef struct {
    int tid;
    int E;
    barrera_t* barrera;
} hilo_info_t;

void* rutina(void* arg) {
    hilo_info_t* info = (hilo_info_t*) arg;

    for (int e = 0; e < info->E; e++) {

        usleep(1000 * (rand() % 300));

        printf("[T%d] esperando en etapa %d\n", info->tid, e);
        fflush(stdout);

        barrera_wait(info->barrera);

        printf("[T%d] paso barrera en etapa %d\n", info->tid, e);
        fflush(stdout);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    int N = 5;
    int E = 4;

    if (argc >= 2) N = atoi(argv[1]);
    if (argc >= 3) E = atoi(argv[2]);

    pthread_t threads[N];
    hilo_info_t infos[N];

    barrera_t barrera;
    barrera_init(&barrera, N);

    for (int i = 0; i < N; i++) {
        infos[i].tid = i;
        infos[i].E = E;
        infos[i].barrera = &barrera;
        pthread_create(&threads[i], NULL, rutina, &infos[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    barrera_destroy(&barrera);

    return 0;
}
