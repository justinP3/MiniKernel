#include "pcb.h"
#include "sync.h"

typedef struct Nodo {
  pcb_t proceso;
  struct Nodo *next;
} nodo_t;

typedef struct Cola {
  nodo_t *front;
  nodo_t *rear;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} cola_t;

extern cola_t queueReady;
void crearCola(cola_t *cola);
int vacio(cola_t *cola);
void enqueue(cola_t *cola, pcb_t proceso);
pcb_t dequeue(cola_t *cola);
