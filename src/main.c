#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../include/queue.h"
#include "../include/metrics.h"
// se incializan los datos funciones y estrucuturas que se usaran globalmente incluso por lo otros archivos.c
cola_t queue_Ready;
void* rutina_cpu(void* arg);
metrics_t metricas;
// funcion para generar los procesos indefinidamente y asignar los valores aleatorios correspondientes con hilos
void* generador_procesos(void* arg) {
    (void) arg;
    int pidCounter = 1;
    while(1) {
        // duerme entre 0 y 3 segundo aleatoriamente para crear los procesos
        sleep(rand() % 3);
        // se crear y rellenan los datos el pcb del proceso
        pcb_t newProcess;
        newProcess.pid = pidCounter++;
        newProcess.burst_time = (rand() % 10) + 1;
        newProcess.remaining_time = newProcess.burst_time; 
        newProcess.arrival_time = (int)time(NULL);
        newProcess.state = 0; 
        //se lo mete a la cola de ready a esperar su ejecucion
        enqueue(&queue_Ready, newProcess);
        printf("Proceso PID %d creado.\n", newProcess.pid);
    }
    return NULL;
}

int main() {
    // se crea la semilla para que siempre sea aleatorio los numeros generados
    srand(time(NULL));
    // se llaman a la funuciones para crear la cola y las mestricas necesarias
    crear_cola (&queue_Ready);
    init_metrics(&metricas);
    int cpu_ids[Cant_cpus];

    printf("Ejecucion del Mini Kernel\n");
    pthread_t cpu_threads[Cant_cpus];
    // se crean los hilos que simulara las cpu del minikernel
    for (int i = 0; i < Cant_cpus; i++) {
        cpu_ids[i] = i + 1;
        if (pthread_create(&cpu_threads[i], NULL, rutina_cpu, &cpu_ids[i]) != 0) {
            perror("Error al crear un hilo de CPU");
            return 1;
        }
    }
    // se crean los hilos que se encarganran de sus respectivos procesos, el hilo_generador para generar procesos meterlos a la cola y luego mandarlo 
    // a la rituna del cpu, y el hilo_metrica para calcular las metricas
    pthread_t hilo_generador;
    if (pthread_create(&hilo_generador, NULL, generador_procesos, NULL) != 0) {
        perror("No se pudo crear el hilo generador.\n");
        return 1;
    }
    pthread_t hilo_metrica;
    if (pthread_create(&hilo_metrica, NULL, hilo_metricas, &metricas) != 0) {
        perror("Error al crear el hilo de métricas");   
    }
    sleep(tiempo);
    return 0;
}