#ifndef GATE_H
#define GATE_H


#include "port.h"


typedef struct circuit circuit_t;


typedef struct gate {
	char *name;
	char *type;

	vector_t ports;
	circuit_t *inner_circuit;
} gate_t;


bool gate_add_port(gate_t *gate, unsigned int i, char *X_name, PortType_t type);
bool gate_add_input(gate_t *gate, unsigned int i, char *name);
bool gate_add_output(gate_t *gate, unsigned int i, char *name);
bool gate_add_node(gate_t *gate, char *name);
bool gate_set_ports(gate_t *gate, char *portname, vector_t *dependencies);
bool gate_link_inner_circuit(gate_t *gate);
bool gate_update_state(gate_t *gate);
bool gate_is_io(gate_t *gate);
port_t *gate_get_port_by_name(gate_t *gate, char *name);


gate_t *gate_copy(gate_t *src);
void gate_print(gate_t *gate, unsigned int depth);
void gate_init(gate_t *gate);
void gate_free(gate_t *gate);


#endif
