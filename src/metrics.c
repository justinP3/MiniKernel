#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/metrics.h"


void init_metrics(metrics_t* m) {
    m->procesos_terminados = 0;
    m->tiempo_respuesta_total = 0;
    m->procesos_atendidos = 0;
    pthread_mutex_init(&m->metrics_mutex, NULL);
}

void registrar_proceso_terminado(metrics_t* m) {
    pthread_mutex_lock(&m->metrics_mutex);
    m->procesos_terminados++;
    pthread_mutex_unlock(&m->metrics_mutex);
}

void registrar_tiempo_respuesta(metrics_t* m, int tiempo_llegada, int tiempo_inicio) {
    int tiempo_espera = tiempo_inicio - tiempo_llegada;

    pthread_mutex_lock(&m->metrics_mutex);
    m->tiempo_respuesta_total += tiempo_espera;
    m->procesos_atendidos++;
    pthread_mutex_unlock(&m->metrics_mutex);
}

void* hilo_metricas(void* arg) {
    FILE *archivo = fopen("tests/metricas.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
    return NULL; 
}

    
    metrics_t* m = (metrics_t*)arg;
    int intervalo = 5;

    while (1) {
        sleep(intervalo); 
        pthread_mutex_lock(&m->metrics_mutex);

        double promedio_respuesta = 0.0;
        if (m->procesos_atendidos > 0) {
            promedio_respuesta = (double)m->tiempo_respuesta_total / m->procesos_atendidos;
        }

        double throughput = (double)m->procesos_terminados / intervalo;

        printf("\n----------------------------------------------------------------\n");
        printf("Reporte metricas, ultimos %d segundos\n", intervalo);
        printf("Procesos completados: %d\n", m->procesos_terminados);
        printf("Throughput: %.2f\n", throughput);
        printf("Tiempo de respuesta promedio: %.2f segundos\n", promedio_respuesta);
        printf("\n----------------------------------------------------------------\n");

        fprintf(archivo,"\n----------------------------------------------------------------\n");
        fprintf(archivo,"Reporte metricas, ultimos %d segundos\n", intervalo);
        fprintf(archivo,"Procesos completados: %d\n", m->procesos_terminados);
        fprintf(archivo,"Throughput: %.2f\n", throughput);
        fprintf(archivo,"Tiempo de respuesta promedio: %.2f segundos\n", promedio_respuesta);
        m->procesos_terminados = 0;
        m->tiempo_respuesta_total = 0;
        m->procesos_atendidos = 0;

        pthread_mutex_unlock(&m->metrics_mutex);
        fflush(archivo);
    }
    fclose(archivo);
    return NULL;
}