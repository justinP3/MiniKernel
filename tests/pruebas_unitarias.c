#include <stdio.h>
#include <assert.h>
#include "../include/queue.h"

int main() {
    printf("pruebas unitarias \n");

    cola_t prueba;
    crear_cola(&prueba);

    pcb_t proceso_prueba;
    proceso_prueba.pid = 33;
    proceso_prueba.burst_time = 5;

    //se invocan las funcion a probar queue y dequeue, y se obtiene el proceso encolado con el dequeue probando las dos funciones al mismo tiempo
    printf("prueba funcion enqueue\n");
    enqueue(&prueba, proceso_prueba);
    printf("prueba funcion dequeue\n");
    pcb_t proceso_recuperado = dequeue(&prueba);

    // se introduce la variable donde se guardo el proceso para evaluar que funciono bien el proceso de enqueue y dequeue apoyandose de la
    // libreriaassert.h 
    assert(proceso_recuperado.pid == 33);
    assert(proceso_recuperado.burst_time == 5);

    printf("prueba de cola ejecutada con exito.\n");

    return 0;
}