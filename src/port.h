#ifndef PORT_H
#define PORT_H


#include <stdbool.h>

#include "defines.h"
#include "vector.h"


typedef struct gate gate_t;


typedef enum PortType {
	PortType_INPUT,
	PortType_OUTPUT,
	PortType_NODE
} PortType_t;



typedef struct port {
	char *name;
	bool state;
	gate_t *gate;

	PortType_t type;
	vector_t connections;
} port_t;


bool port_update_state(port_t *port);
bool port_set_state(port_t *port, bool state);


port_t *port_copy(port_t *src);
void port_print(port_t *port);
void port_init(port_t *port);
void port_free(port_t *port);


#endif
