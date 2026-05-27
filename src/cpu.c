#include <stdio.h>
#include <unistd.h>      
#include "../include/queue.h"
#include "../include/metrics.h"

extern cola_t queue_Ready;
extern metrics_t metricas;

void* rutina_cpu(void* arg) {
    int id_cpu = *((int*)arg); 

    while (1) {

        pcb_t proceso_actual = dequeue(&queue_Ready);

        if (proceso_actual.burst_time == proceso_actual.remaining_time) {
            registrar_tiempo_respuesta(&metricas, proceso_actual.arrival_time, (int)time(NULL)); 
        }

        printf("CPU %d inicia ejecución de PID %d. Tiempo restante: %d\n", id_cpu, proceso_actual.pid, proceso_actual.remaining_time);
        int tiempo_ejecucion = (proceso_actual.remaining_time < 3) ? proceso_actual.remaining_time : 3;
        sleep(1);
        proceso_actual.remaining_time -= tiempo_ejecucion;

        if (proceso_actual.remaining_time <= 0) {
            proceso_actual.state = 2;
            printf("CPU %d, proceso PID %d terminado.\n", id_cpu, proceso_actual.pid);
            registrar_proceso_terminado(&metricas);
        } else {
            printf("CPU %d tiempo agotado (Quantum). PID %d vuelve a la cola. Le falta: %d\n", 
                   id_cpu, proceso_actual.pid, proceso_actual.remaining_time);
            enqueue(&queue_Ready, proceso_actual);
        }
    }

    return NULL;
}