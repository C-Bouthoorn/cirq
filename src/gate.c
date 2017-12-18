#include "gate.h"

#include "circuit.h"
#include "assert.h"


port_t *gate_get_port_by_name(gate_t *gate, char *name) {
  assert_neq(gate, NULL);
  assert_neq(name, NULL);

  VEC_EACH(gate->ports, port_t *port) {
    assert_neq(port, NULL);

    if (strcmp(port->name, name) == 0) {
      return port;
    }
  }

  fprintf(stderr, "Failed to find port '%s' for gate '%s'\n", name, gate->name);
  return NULL;
}


void gate_add_port(gate_t *gate, unsigned int i, char *name, PortType_t type) {
  assert_neq(gate, NULL);

  if (name == NULL) {
    // Set default name
    name = malloc(BUF_SIZE * sizeof(char));

    char typechar = (type == PortType_INPUT) ? 'I' : 'O';
    sprintf(name, "%c%i", typechar, i);
  }


  // Create port
  port_t *p = malloc(sizeof(port_t));
  port_init(p);

  p->name = name;
  p->gate = gate;
  p->type = type;

  // Add port
  assert(vector_push(&gate->ports, p));
}


void gate_add_input(gate_t *gate, unsigned int i, char *name) {
  gate_add_port(gate, i, name, PortType_INPUT);
}


void gate_add_output(gate_t *gate, unsigned int i, char *name) {
  gate_add_port(gate, i, name, PortType_OUTPUT);
}


bool gate_set_ports(gate_t *gate, char *portname, vector_t *dependencies) {
  assert_neq(gate, NULL);

  switch_str(gate->type) {
    case_str("AND") {
      gate_add_input(gate, 0, NULL);
      gate_add_input(gate, 1, NULL);
      gate_add_output(gate, 0, NULL);
      return true;
    }


    else case_str("OR") {
      gate_add_input(gate, 0, NULL);
      gate_add_input(gate, 1, NULL);
      gate_add_output(gate, 0, NULL);
      return true;
    }


    else case_str("NOT") {
      gate_add_input(gate, 0, NULL);
      gate_add_output(gate, 0, NULL);
      return true;
    }


    else case_str("IN") {
      gate_add_output(gate, 0, portname);
      return true;
    }


    else case_str("OUT") {
      gate_add_input(gate, 0, portname);
      return true;
    }

    else {
      circuit_t *custom = NULL;

      if (dependencies == NULL) goto no_deps;

      for (size_t i = 0; i < dependencies->amount; i++) {
        circuit_t *dep = dependencies->items[i];

        if (strcmp(dep->name, gate->type) == 0) {
          custom = dep;
          break;
        }
      }


      if (custom == NULL) {
        no_deps:
        fprintf(stderr, "Failed to set ports for gate of type '%s'\n", gate->type);
        return false;
      }

      return gate_take_gates_from_circuit(gate, custom);
    }
  }

  return false;
}


bool gate_take_gates_from_circuit(gate_t *gate, circuit_t *circuit) {
  assert_neq(gate, NULL);
  assert_neq(circuit, NULL);

  VEC_EACH(circuit->gates, gate_t *g) {
    assert_neq(g, NULL);

    char *newportname = NULL;

    if ( ! (strcmp(g->type, "IN") == 0 || strcmp(g->type, "OUT") == 0) ) {
      // Skip if it's not an I/O port
      continue;
    }

    // Get old name
    char *portname = ((port_t *) g->ports.items[0])->name;

    assert_neq(portname, NULL);

    // Copy
    newportname = malloc(sizeof(portname) * sizeof(char));
    strcpy(newportname, portname);

    switch_str(g->type) {
      case_str("IN") {
        gate_add_input(gate, 0, newportname);
      }

      // There is only one other option: OUT
      else {
        gate_add_output(gate, 0, newportname);
      }
    }
  }

  return true;
}


bool gate_update_state(gate_t *gate) {
  switch_str(gate->type) {
    case_str("AND") {
      port_t *i0 = gate->ports.items[0];
      port_t *i1 = gate->ports.items[1];
      port_t *o0 = gate->ports.items[2];

      assert_eq(i0->type, PortType_INPUT);
      assert_eq(i1->type, PortType_INPUT);
      assert_eq(o0->type, PortType_OUTPUT);

      o0->state = i0->state && i1->state;
      return port_update_state(o0);
    }


    else case_str("OR") {
      port_t *i0 = gate->ports.items[0];
      port_t *i1 = gate->ports.items[1];
      port_t *o0 = gate->ports.items[2];

      assert_eq(i0->type, PortType_INPUT);
      assert_eq(i1->type, PortType_INPUT);
      assert_eq(o0->type, PortType_OUTPUT);

      o0->state = i0->state || i1->state;
      return port_update_state(o0);
    }


    else case_str("NOT") {
      port_t *i0 = gate->ports.items[0];
      port_t *o0 = gate->ports.items[1];

      assert_eq(i0->type, PortType_INPUT);
      assert_eq(o0->type, PortType_OUTPUT);

      o0->state = ! i0->state;
      return port_update_state(o0);
    }


    else case_str("IN") {
      port_t *o0 = gate->ports.items[0];

      assert_eq(o0->type, PortType_OUTPUT);
      return true;
    }


    else case_str("OUT") {
      port_t *i0 = gate->ports.items[0];

      assert_eq(i0->type, PortType_INPUT);
      return true;
    }


    else {
      fprintf(stderr, "Can't update the state of the gate with unknown type '%s'\n", gate->type);
      return false;
    }
  }

  return false;
}



void gate_print(gate_t *gate) {
  printf("{ %s [%s] }\n", gate->name, gate->type);

  VEC_EACH(gate->ports, port_t* p) {
    printf("\t");
    port_print(p);
    printf("\n");
  }
}


void gate_init(gate_t *gate) {
  gate->name = NULL;
  gate->type = NULL;

  vector_init(&gate->ports, BUF_SIZE);
}


void gate_free(gate_t *gate) {
  if (gate->name) free(gate->name);
  if (gate->type) free(gate->type);

  // Free all ports
  VEC_EACH(gate->ports, port_t* p) {
    port_free(p);
    free(p);
  }

  vector_free(&gate->ports);
}
