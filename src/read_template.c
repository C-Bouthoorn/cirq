#include "read_template.h"

#include "assert.h"


bool read_template(char *filename, circuit_t *circ, vector_t *dependencies) {
  assert_neq(filename, NULL);
  assert_neq(circ, NULL);

  char buf[BUF_SIZE];

  // Open file
  FILE *file = fopen(filename, "r");
  assert_neq(file, NULL);

  // Get name, which is always the first thing in a file
  // example: NAND
  circ->name = malloc(BUF_SIZE * sizeof(char));
  fscanf(file, "%s\n", circ->name);


  // Get gates, which are always second
  // example:  [gates] 16
  size_t amount_gates;

  fscanf(file, "%s %lu\n", buf, &amount_gates);

  if ( strcmp(buf, "[gates]") != 0 ) {
    panic("no [gates]?? '%s' unexpected", buf);
  }


  for (size_t i = 0; i < amount_gates; i++) {
    char *name = malloc(BUF_SIZE * sizeof(char));
    char *type = malloc(BUF_SIZE * sizeof(char));
    char *portname = malloc(BUF_SIZE * sizeof(char));

    // Get data
    // example:  5cf6cdaa IN #I0
    int read_arguments = fscanf(file, "%s %s #%s\n", name, type, portname);

    // If no portname is read, free it
    // example:  6306ee7f NOT
    if (read_arguments == 2) {
      free(portname);
      portname = NULL;
    }

    if ( read_arguments == -1 ) {
      // Unexpected end of file
      panic("EOF??");
    }

    // New gate
    gate_t *g = malloc(sizeof(gate_t));
    gate_init(g);


    g->name = name;
    g->type = type;

    // Set the correct ports
    assert(gate_set_ports(g, portname, dependencies));

    // Add gate to circuit
    assert(vector_push(&circ->gates, g));
  }


  // Get wires, which are third
  // example:  [wires] 20
  size_t amount_wires;

  fscanf(file, "%s %lu\n", buf, &amount_wires);

  if ( strcmp(buf, "[wires]") != 0 ) {
    panic("no [wires]?? '%s' unexpected", buf);
  }

  for (size_t i = 0; i < amount_wires; i++) {
    char *leftuuid = malloc(BUF_SIZE * sizeof(char));
    char *leftport = malloc(BUF_SIZE * sizeof(char));
    char *rightuuid = malloc(BUF_SIZE * sizeof(char));
    char *rightport = malloc(BUF_SIZE * sizeof(char));

    // example:  da2ecd25:O0 eec2fc01:I0
    int x = fscanf(file, "%[a-f0-9]:%s %[a-f0-9]:%s\n", leftuuid, leftport, rightuuid, rightport);

    if ( x == -1 ) {
      // Unexpected end of file
      panic("EOF??");
    }

    // Create new wire
    wire_t *w = malloc(sizeof(wire_t));
    wire_init(w);

    w->leftuuid = leftuuid;
    w->leftport = leftport;
    w->rightuuid = rightuuid;
    w->rightport = rightport;

    assert(circuit_apply_wire(circ, w));

    wire_free(w);
    free(w);
  }

  // Close template file
  fclose(file);


  // Make sure all gates are in the right state
  // NOTE: This will crash if a contradicting loop occurs in the program
  //   example: NOT:I0 <---> NOT:O0
  return circuit_update_state(circ);
}
