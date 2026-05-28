#include "../include/queue.h"
#include <stdlib.h>

//funcion para crear o inicializar la cola con valores nulos y evitar problemas de ejecucion
void crear_cola(cola_t* cola) {
    cola->front = NULL;
    cola->rear = NULL;
    //incia el mutex para proteger la cola de la concurrencia
    pthread_mutex_init(&cola->mutex, NULL);
    pthread_cond_init(&cola->cond, NULL);
}
// funcion pra verificar que no este vacia la cola
int vacio(cola_t *cola) {
    if (cola->front == NULL){
        return 1;
    }
    return 0;
}
// funcion para encolar un proceso mediante nodos de manera dinamica en la heap con malloc
void enqueue(cola_t* cola, pcb_t process) {
    nodo_t *nuevo = (nodo_t*)malloc(sizeof(nodo_t));
    nuevo->proceso = process;
    // bloquea la cola para hacer los cambios sin generar problemas 
    pthread_mutex_lock(&cola->mutex);
    //si es el primer proceso hace que el frent y final de la cola apunte a el para luego poder encolar los otros sin problemas
    if (cola->front == NULL){
        cola->front = nuevo;
        cola->rear = nuevo;
        nuevo->next = nuevo;
    }
    // encola normalmente los nuevos nodos enlazandolos al ultimo nodo de la cola y actualizando el nodo final.
    cola->rear->next = nuevo;
    cola->rear = nuevo;
    cola-> rear ->next = cola-> front;
    // lo desbloquea para que siga con la ejecucion los otros hilos
    pthread_cond_signal(&cola->cond);
    pthread_mutex_unlock(&cola->mutex);
}
// funcion para eliminar los nodos de la cola
pcb_t dequeue (cola_t* cola) {
    // lo bloquea 
    pthread_mutex_lock(&cola->mutex);
    //mira esta vacia la cola antes de quitar algo y generar un error , si lo esta mantiene la cola libre para que se haga el enqueue.
    while (cola->front == NULL) {
        pthread_cond_wait(&cola->cond, &cola->mutex);
    }
    // logica del dequeue, crea un puntero temporal hacia el nodo a elimnar para libera memoria lueego, y actualiza los punteros del
    // front, rear de la cola y el next de los nodos importante para el dequeue deslindadolo de la cola.
    nodo_t* borrar = cola->front;
    pcb_t pcb = cola->front->proceso;

    if (cola->front == cola->rear){
        cola->front = cola-> rear = NULL;
    } else {
        cola->front = cola->front->next;
        cola->rear->next = cola->front;
    }
    //libera la memoria par que no quede basura y desbloquea el recurso
    free(borrar);
    pthread_mutex_unlock(&cola->mutex);
    return pcb;
}
// funcion para despertar todos los hilos y cerrar el kernel
void despertar_hilos_cola(cola_t* cola) {
    pthread_cond_broadcast(&cola->cond);
}


