#ifndef CIRCUIT_H
#define CIRCUIT_H


#include "wire.h"
#include "gate.h"


typedef struct circuit {
  char *name;
  vector_t gates;
} circuit_t;


bool circuit_apply_wire(circuit_t *circ, wire_t *wire);
bool circuit_update_state(circuit_t *circ);
gate_t *circuit_get_gate_by_name(circuit_t *circ, char *name);
port_t *circuit_get_port_by_name(circuit_t *circ, char *gatename, char *portname);
port_t *circuit_get_io_port_by_name(circuit_t *circ, char *portname);


void circuit_print(circuit_t *circ);
void circuit_init(circuit_t *circ);
void circuit_free(circuit_t *circ);


#endif
