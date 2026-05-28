#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/metrics.h"

// ffuncion para iniciar la mestricas en cero todo y el mutez para evitar problemas de concurrencia
void init_metrics(metrics_t* m) {
    m->procesos_terminados = 0;
    m->tiempo_respuesta_total = 0;
    m->procesos_atendidos = 0;
    pthread_mutex_init(&m->metrics_mutex, NULL);
}
// funcion para registrar cada vez que termina un proceso
void registrar_proceso_terminado(metrics_t* m) {
    pthread_mutex_lock(&m->metrics_mutex);
    m->procesos_terminados++;
    pthread_mutex_unlock(&m->metrics_mutex);
}
//funcion para calcula el tiempo que un proceso estuvo en cola restando el tiempo creado con el que entro a la rutina de la cpu y lo registra
void registrar_tiempo_respuesta(metrics_t* m, int tiempo_llegada, int tiempo_inicio) {
    int tiempo_espera = tiempo_inicio - tiempo_llegada;

    pthread_mutex_lock(&m->metrics_mutex);
    m->tiempo_respuesta_total += tiempo_espera;
    m->procesos_atendidos++;
    pthread_mutex_unlock(&m->metrics_mutex);
}

// funcion donde ocurre todo el calculo de las metricas 
void* hilo_metricas(void* arg) {
    // revisa que se abra el archivo correctamente, y lo hace en modo escritura 
    FILE *archivo = fopen("tests/metricas.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return NULL; 
    }
    metrics_t* m = (metrics_t*)arg;
    int intervalo = 5;

    while (1) {
        // ejecuta el proceso cada 5 segundos 
        sleep(intervalo); 
        pthread_mutex_lock(&m->metrics_mutex);
        // calcula el promedio de tiempo de respuesta diviendo el tiempo de respuesta total y la cantidad de procesos atendidos por primera vez
        double promedio_respuesta = 0.0;
        if (m->procesos_atendidos > 0) {
            promedio_respuesta = (double)m->tiempo_respuesta_total / m->procesos_atendidos;
        }
        // calcula el trhoughtput diviendo los precos ejecutados entre el tiempo que recopilo datos la metrica.
        double throughput = (double)m->procesos_terminados / intervalo;
        // imprime las mestricas en un rchivos separadao para no saturar las impreciones de la ejecucion del codigo y revisarlas
        fprintf(archivo,"\n----------------------------------------------------------------\n");
        fprintf(archivo,"Reporte metricas, ultimos %d segundos\n", intervalo);
        fprintf(archivo,"Procesos completados: %d\n", m->procesos_terminados);
        fprintf(archivo,"Throughput: %.2f\n", throughput);
        fprintf(archivo,"Tiempo de respuesta promedio: %.2f segundos\n", promedio_respuesta);
        // vuelve las mestricas a cero para el siguiente calculo
        m->procesos_terminados = 0;
        m->tiempo_respuesta_total = 0;
        m->procesos_atendidos = 0;

        pthread_mutex_unlock(&m->metrics_mutex);
        fflush(archivo);
    }
    fclose(archivo);
    return NULL;
}