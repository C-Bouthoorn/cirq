#include "circuit.h"

#include "utils.h"
#include "assert.h"


bool circuit_apply_wire(circuit_t *circ, wire_t *wire) {
  assert_neq(circ, NULL);
  assert_neq(wire, NULL);

  port_t *left_port = NULL;
  port_t *right_port = NULL;

  // Get the matching ports
  VEC_EACH(circ->gates, gate_t *gate) {
    assert_neq(gate, NULL);

    switch_str(gate->name) {
      case_str(wire->leftuuid) {
        left_port = gate_get_port_by_name(gate, wire->leftport);

        assert_neq(left_port, NULL);
      }

      // NOTE: Using `else` disables the possibility to link a gate to itself,
      // which can be both desired and not desired
      case_str(wire->rightuuid) {
        right_port = gate_get_port_by_name(gate, wire->rightport);

        assert_neq(right_port, NULL);
      }
    }
  }

  // Make sure we got all ports
  if (left_port == NULL || right_port == NULL) {
    STDOUT_TO_STDERR();

    printf("Failed to find both ports for wire ");
    wire_print(wire);
    printf("\n");

    RESET_STDOUT();
    return false;
  }

  // Add connection
  assert(vector_push(&left_port->connections, right_port));
  assert(vector_push(&right_port->connections, left_port));

  return true;
}


bool circuit_update_state(circuit_t *circ) {
  assert_neq(circ, NULL);

  bool success = true;

  VEC_EACH(circ->gates, gate_t *gate) {
    assert_neq(gate, NULL);

    success &= gate_update_state(gate);
  }

  return success;
}


gate_t *circuit_get_gate_by_name(circuit_t *circ, char *name) {
  assert_neq(circ, NULL);
  assert_neq(name, NULL);

  VEC_EACH(circ->gates, gate_t *gate) {
    assert_neq(gate, NULL);

    if (strcmp(gate->name, name) == 0) {
      return gate;
    }
  }

  warn("Failed to find gate '%s' in circuit '%s'", name, circ->name);
  return NULL;
}


port_t *circuit_get_port_by_name(circuit_t *circ, char *gatename, char *portname) {
  assert_neq(circ, NULL);
  assert_neq(gatename, NULL);
  assert_neq(portname, NULL);

  gate_t *gate = circuit_get_gate_by_name(circ, gatename);

  assert_neq(gate, NULL);

  return gate_get_port_by_name(gate, portname);
}


port_t *circuit_get_io_port_by_name(circuit_t *circ, char *portname) {
  assert_neq(circ, NULL);
  assert_neq(portname, NULL);

  VEC_EACH(circ->gates, gate_t *gate) {
    assert_neq(gate, NULL);

    // Filter on I/O ports only
    if (! gate_is_io(gate)) {
      continue;
    }

    // Check portname
    port_t *port = gate->ports.items[0];

    if (strcmp(port->name, portname) == 0)
      return port;
  }

  warn("Failed to find I/O port '%s' for circuit '%s'", portname, circ->name);
  return NULL;
}


void circuit_print(circuit_t *circ) {
  assert_neq(circ, NULL);

  printf("Circuit %s: %lu gates\n\n", circ->name, circ->gates.amount);

  // Print gates
  for (size_t i = 0; i < circ->gates.amount; i++) {
    printf("gate %lu: ", i);

    gate_print(circ->gates.items[i]);
    printf("\n");
  }
}


void circuit_init(circuit_t *circ) {
  assert_neq(circ, NULL);

  circ->name = NULL;

  vector_init(&circ->gates, BUF_SIZE);
}


void circuit_free(circuit_t *circ) {
  assert_neq(circ, NULL);

  if (circ->name) free(circ->name);

  // Free gates
  VEC_EACH(circ->gates, gate_t* g) {
    assert_neq(g, NULL);

    gate_free(g);
    free(g);
  }

  vector_free(&circ->gates);
}
