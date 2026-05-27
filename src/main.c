#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../include/queue.h"
#include "../include/metrics.h"

cola_t queue_Ready;
void* rutina_cpu(void* arg);
metrics_t metricas;

void* generador_procesos(void* arg) {
    (void) arg;
    int pidCounter = 1;
    while(1) { 
        sleep(rand() % 2);
        
        pcb_t newProcess;
        newProcess.pid = pidCounter++;
        newProcess.burst_time = (rand() % 10) + 1;
        newProcess.remaining_time = newProcess.burst_time; 
        newProcess.arrival_time = (int)time(NULL);
        newProcess.state = 0; 
        
        enqueue(&queue_Ready, newProcess);
        printf("Proceso PID %d creado.\n", newProcess.pid);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    crear_cola (&queue_Ready);
    init_metrics(&metricas);
    int cpu_ids[6];

    printf("MiniKernel\n");
    pthread_t cpu_threads[3];

    for (int i = 0; i < 3; i++) {
        cpu_ids[i] = i + 1;
        if (pthread_create(&cpu_threads[i], NULL, rutina_cpu, &cpu_ids[i]) != 0) {
            perror("Error al crear un hilo de CPU");
            return 1;
        }
    }

    pthread_t hilo_generador;
    if (pthread_create(&hilo_generador, NULL, generador_procesos, NULL) != 0) {
        perror("No se pudo crear el hilo generador.\n");
        return 1;
    }
    pthread_t hilo_metrica;
    if (pthread_create(&hilo_metrica, NULL, hilo_metricas, &metricas) != 0) {
        perror("Error al crear el hilo de métricas");   
    }
    pthread_join(hilo_generador, NULL);

    return 0;
}