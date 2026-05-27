#ifndef METRICS_H
#define METRICS_H

#include "sync.h"

typedef struct {
  int procesos_terminados;
  int tiempo_respuesta_total;
  int procesos_atendidos;

  pthread_mutex_t metrics_mutex;
} metrics_t;

void init_metrics(metrics_t *m);
void registrar_proceso_terminado(metrics_t *m);
void registrar_tiempo_respuesta(metrics_t *m, int tiempo_llegada,
                                int tiempo_inicio);
void *hilo_metricas(void *arg);

#endif