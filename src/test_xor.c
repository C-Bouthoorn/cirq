#include "read_template.h"
#include "test.h"


unsigned int test_xor(void) {
  TEST_START;

  // Create circuit
  circuit_t *circ = malloc(sizeof(circuit_t));
  circuit_init(circ);

  // Read template
  assert_true(read_template("tests/xor", circ, NULL));

  // Get all I/O gates
  port_t *i0 = circuit_get_port_by_name(circ, "5cf6cdaa", "I0");
  port_t *i1 = circuit_get_port_by_name(circ, "3958dd83", "I1");
  port_t *o0 = circuit_get_port_by_name(circ, "961b5519", "O0");

  // Make sure we got them
  assert_neq(i0, NULL);
  assert_neq(i1, NULL);
  assert_neq(o0, NULL);

  // Check their types
  assert_eq(i0->type, PortType_OUTPUT);
  assert_eq(i1->type, PortType_OUTPUT);
  assert_eq(o0->type, PortType_INPUT);

  // Make sure default inputs are false
  assert_eq(port_get_state(i0), false);
  assert_eq(port_get_state(i1), false);

  // 0 0 -> 0
  port_set_state(i0, false);
  port_set_state(i1, false);
  assert_eq(port_get_state(o0), false);

  // 0 1 -> 1
  port_set_state(i0, true);
  port_set_state(i1, false);
  assert_eq(port_get_state(o0), true);

  // 1 0 -> 1
  port_set_state(i0, false);
  port_set_state(i1, true);
  assert_eq(port_get_state(o0), true);

  // 1 1 -> 0
  port_set_state(i0, true);
  port_set_state(i1, true);
  assert_eq(port_get_state(o0), false);

  // Free everything
  circuit_free(circ);
  free(circ);

  TEST_END;
}
