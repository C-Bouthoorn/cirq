#include "port.h"

#include "bool.h"
#include "gate.h"
#include "assert.h"
#include "benchmark.h"


bool port_update_state(port_t *port) {
	FUNC_START();

	assert_not_null(port);

	bool success = true;

	if (port->type == PortType_INPUT) {
		// Input ports should update the gate they are attached to
		success &= gate_update_state(port->gate);
	}

	else { // if (port->type == PortType_OUTPUT || port->type == PortType_NODE) {
		// Outputs and nodes should copy their state to their connected inputs and nodes
		VEC_EACH(port->connections, port_t *connection) {
			// Filter on inputs and nodes
			if (connection->type != PortType_INPUT && connection->type != PortType_NODE) {
				continue;
			}

			success &= port_set_state(connection, port->state);
		}
	}

	FUNC_END();
	return success;
}


bool port_set_state(port_t *port, bool state) {
	FUNC_START();

	// Check if state already matches
	if (port->state == state) {
		// Don't change anything
		FUNC_END();
		return true;
	}

	port->state = state;

	bool success = port_update_state(port);

	FUNC_END();
	return success;
}


port_t *port_copy(port_t *src) {
	FUNC_START();
	assert_not_null(src);

	port_t *dest = malloc(sizeof(port_t));
	port_init(dest);

	dest->name = malloc(strlen(src->name) + 1);
	strcpy(dest->name, src->name);

	dest->state = src->state;
	dest->type = src->type;

	FUNC_END();
	return dest;
}


void port_print(port_t *port) {
	printf("    :%s (", port->name);
	bool_print(port->state);
	printf(")\t");

	// Only print an arrow when it has a connection
	if (port->connections.amount > 0) {
		if (port->type == PortType_INPUT) {
			printf(" <-- ");
		}
		else if (port->type == PortType_OUTPUT) {
			printf(" --> ");
		}
		else if (port->type == PortType_NODE) {
			printf(" <-> ");
		}
	}

	// Print the connections
	VEC_EACH_INDEX(port->connections, port_t *connection, i) {
		char *gatename = connection->gate->name;
		char *gatetype = connection->gate->type;

		printf("%s {%s}:%s", gatename, gatetype, connection->name);

		// Print a comma when it's not the last item
		if (i < port->connections.amount - 1) {
			printf(", ");
		}
	}
}


void port_init(port_t *port) {
	port->name = NULL;
	port->gate = NULL;

	// Defaults
	port->state = false;
	port->type = PortType_NODE;

	vector_init(&port->connections, BUF_SIZE);
}


void port_free(port_t *port) {
	if (port->name) free(port->name);

	vector_free(&port->connections);
}
