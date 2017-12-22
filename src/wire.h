#ifndef WIRE_H
#define WIRE_H


#include "defines.h"


typedef struct wire {
	char *leftuuid;
	char *leftport;
	char *rightuuid;
	char *rightport;
} wire_t;


void wire_print(wire_t *wire);
void wire_init(wire_t *wire);
void wire_free(wire_t *wire);


#endif
