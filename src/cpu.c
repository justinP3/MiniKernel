#include <stdio.h>
#include <unistd.h>      
#include "../include/queue.h"
#include "../include/metrics.h"

//se usa exterm para que no cree estas varaible si no que las use del 
extern cola_t queue_Ready;
extern metrics_t metricas;

// funcion de la rutina de cpu aqui va context switching, la logica de round robin, encolado, etc.
void* rutina_cpu(void* arg) {
    //obtiene el id del cpu asignada en el main
    int id_cpu = *((int*)arg); 

    while (1) {
        // comienza a simular la ejecucion del proceso sacandolo de la cola
        pcb_t proceso_actual = dequeue(&queue_Ready);
        // compara el tiempo asignado con el que le falta para terminar y evaluar si ya se ha ejecutado antes para usarlo en las metricas
        if (proceso_actual.burst_time == proceso_actual.remaining_time) {
            registrar_tiempo_respuesta(&metricas, proceso_actual.arrival_time, (int)time(NULL)); 
        }
        printf("CPU %d inicia ejecución de PID %d. Tiempo restante: %d\n", id_cpu, proceso_actual.pid, proceso_actual.remaining_time);
        // empieza el round robin evalua. Evalua tiempo que falta para que no quede un tiempo negtivo, si es menor que el Quantum  resta lo que le queda 
        // para llegar a cero, si es mayor solo resta el tiempo disponbile en cpu que se le da. 
        int tiempo_ejecucion = (proceso_actual.remaining_time < 3) ? proceso_actual.remaining_time : 3;
        sleep(1);
        proceso_actual.remaining_time -= tiempo_ejecucion;
        // mira si el tiempo que queda es cero o menor para ver si lo termina, imprime el mensaje y lo manda a la metrica o solo avisa
        //que termino el tiempo y lo encola de nuevo.
        if (proceso_actual.remaining_time == 0) {
            proceso_actual.state = 2;
            printf("CPU %d, proceso PID %d terminado.\n", id_cpu, proceso_actual.pid);
            registrar_proceso_terminado(&metricas);
        } else {
            printf("CPU %d tiempo agotado. El PID %d vuelve a la cola, le falta: %d\n", 
                   id_cpu, proceso_actual.pid, proceso_actual.remaining_time);
            enqueue(&queue_Ready, proceso_actual);
        }
    }
    return NULL;
}