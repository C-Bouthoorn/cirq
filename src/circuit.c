#include "circuit.h"

#include "utils.h"
#include "assert.h"
#include "benchmark.h"


bool circuit_apply_wire(circuit_t *circ, wire_t *wire) {
	FUNC_START();

	assert_not_null(circ);
	assert_not_null(wire);

	// Get the matching gates
	gate_t *left_gate = circuit_get_gate_by_name(circ, wire->leftuuid);
	gate_t *right_gate = circuit_get_gate_by_name(circ, wire->rightuuid);

	if (left_gate == NULL || right_gate == NULL) {
		goto fail;
	}

	port_t *left_port = gate_get_port_by_name(left_gate, wire->leftport);
	port_t *right_port = gate_get_port_by_name(right_gate, wire->rightport);


	// Make sure we got all ports
	if (left_port == NULL || right_port == NULL) {
		fail:

		STDOUT_TO_STDERR();

		printf("Failed to find both ports for wire ");
		wire_print(wire);
		printf("\n");

		RESET_STDOUT();


		FUNC_END();
		return false;
	}


	// Add connection
	assert(vector_push(&left_port->connections, right_port));
	assert(vector_push(&right_port->connections, left_port));


	FUNC_END();
	return true;
}


bool circuit_update_state(circuit_t *circ) {
	FUNC_START();

	assert_not_null(circ);

	bool success = true;

	HEX_HASHMAP_EACH_VALUE(circ->gates, gate_t *gate) {
		assert_not_null(gate);

		success &= gate_update_state(gate);
	}


	FUNC_END();
	return success;
}


gate_t *circuit_get_gate_by_name(circuit_t *circ, char *name) {
	FUNC_START();

	assert_not_null(circ);
	assert_not_null(name);

	gate_t *gate = hex_hashmap_get_item(&circ->gates, name);

	if (gate == NULL) {
		warn("Failed to find gate %s in circuit %s", name, circ->name);

		FUNC_END();
		return NULL;
	}


	FUNC_END();
	return gate;
}


port_t *circuit_get_port_by_name(circuit_t *circ, char *gatename, char *portname) {
	FUNC_START();

	assert_not_null(circ);
	assert_not_null(gatename);
	assert_not_null(portname);


	gate_t *gate = circuit_get_gate_by_name(circ, gatename);

	if (gate == NULL) {
		goto fail;
	}


	port_t *port = gate_get_port_by_name(gate, portname);

	if (port == NULL) {
		fail:
		warn("Failed to find port %s:%s:%s", circ->name, gatename, portname);

		FUNC_END();
		return NULL;
	}


	FUNC_END();
	return port;
}


port_t *circuit_get_io_port_by_name(circuit_t *circ, char *portname) {
	FUNC_START();

	assert_not_null(circ);
	assert_not_null(portname);


	HEX_HASHMAP_EACH_VALUE(circ->gates, gate_t *gate) {
		assert_not_null(gate);

		// Filter on I/O ports only
		if (! gate_is_io(gate)) {
			continue;
		}

		// Check portname
		port_t *port = gate->ports.items[0];

		if (strcmp(port->name, portname) == 0) {
			FUNC_END();
			return port;
		}
	}

	warn("Failed to find I/O port %s for circuit %s", portname, circ->name);

	FUNC_END();
	return NULL;
}


circuit_t *circuit_copy(circuit_t *src) {
	FUNC_START();

	assert_not_null(src);


	circuit_t *dest = malloc(sizeof(circuit_t));
	circuit_init(dest);

	// Copy name
	dest->name = malloc(strlen(src->name) + 1);
	strcpy(dest->name, src->name);

	// Copy gates + ports
	HEX_HASHMAP_EACH_VALUE(src->gates, gate_t *gate) {
		gate_t *new_gate = gate_copy(gate);

		hex_hashmap_add_item(&dest->gates, new_gate->name, new_gate);
	}

	// Copy connections
	HEX_HASHMAP_EACH_VALUE(src->gates, gate_t *old_gate) {
		gate_t *new_gate = circuit_get_gate_by_name(dest, old_gate->name);
		assert_not_null(new_gate);


		VEC_EACH(old_gate->ports, port_t *old_port) {
			port_t *new_port = gate_get_port_by_name(new_gate, old_port->name);
			assert_not_null(new_port);


			VEC_EACH(old_port->connections, port_t *old_port_conn) {
				// Get corresponding new gate
				gate_t *old_port_conn_gate = old_port_conn->gate;
				assert_not_null(old_port_conn_gate);

				gate_t *new_port_conn_gate = circuit_get_gate_by_name(dest, old_port_conn_gate->name);
				assert_not_null(new_port_conn_gate);

				// Get corresponding new port
				port_t *new_port_conn = gate_get_port_by_name(new_port_conn_gate, old_port_conn->name);
				assert_not_null(new_port_conn);

				// Create new connection
				assert(vector_push(&new_port->connections, new_port_conn));
			}
		}
	}


	FUNC_END();
	return dest;
}



void circuit_print(circuit_t *circ, unsigned int depth) {
	assert_not_null(circ);

	// Create prefix
	char prefix[16];
	memset(prefix, 0, 16);
	memset(prefix, '\t', depth);


	HEX_HASHMAP_EACH_VALUE(circ->gates, gate_t *gate) {
		printf("%s\t", prefix);
		gate_print(gate, depth);
		printf("\n");
	}
}


void circuit_init(circuit_t *circ) {
	assert_not_null(circ);

	circ->name = NULL;

	hex_hashmap_init(&circ->gates);
}


void circuit_free(circuit_t *circ) {
	assert_not_null(circ);

	if (circ->name) free(circ->name);

	// Free gates
	HEX_HASHMAP_EACH_VALUE(circ->gates, gate_t* g) {
		assert_not_null(g);

		gate_free(g);
		free(g);
	}

	hex_hashmap_free(&circ->gates);
}
