#include "../include/queue.h"
#include <stdlib.h>

void crearCola(cola_t* cola) {
    cola->front = NULL;
    cola->rear = NULL;
    pthread_mutex_init(&cola->mutex, NULL);
    pthread_cond_init(&cola->cond, NULL);
}

int vacio(cola_t *cola) {
    if (cola->front == NULL){
        return 1;
    }
    return 0;
}
void enqueue(cola_t* cola, pcb_t process) {
    nodo_t *nuevo = (nodo_t*)malloc(sizeof(nodo_t));
    nuevo->proceso = process;

    pthread_mutex_lock(&cola->mutex);

    if (cola->front == NULL){
        cola->front = nuevo;
        cola->rear = nuevo;
        nuevo->next = nuevo;
    }

    cola->rear->next = nuevo;
    cola->rear = nuevo;
    cola-> rear ->next = cola-> front;

    pthread_cond_signal(&cola->cond);
    pthread_mutex_unlock(&cola->mutex);
}

pcb_t dequeue (cola_t* cola) {
    pthread_mutex_lock(&cola->mutex);
    while (cola->front == NULL) {
        pthread_cond_wait(&cola->cond, &cola->mutex);
    }

    nodo_t* borrar = cola->front;
    pcb_t pcb = cola->front->proceso;

    if (cola->front == cola->rear){
        cola->front = cola-> rear = NULL;
    } else {
        cola->front = cola->front->next;
        cola->rear->next = cola->front;
    }
    
    free(borrar);
    pthread_mutex_unlock(&cola->mutex);
    return pcb;
}


