#ifndef PORT_H
#define PORT_H


#include <stdbool.h>

#include "defines.h"
#include "vector.h"


typedef struct gate gate_t;


typedef enum PortType {
  PortType_INPUT,
  PortType_OUTPUT
} PortType_t;



typedef struct port {
  char *name;
  bool state;
  PortType_t type;

  gate_t *gate;
  vector_t connections;
} port_t;


bool port_update_state(port_t *port);
bool port_set_state(port_t *port, bool state);
bool port_get_state(port_t *port);


void port_print(port_t *port);
void port_init(port_t *port);
void port_free(port_t *port);


#endif
