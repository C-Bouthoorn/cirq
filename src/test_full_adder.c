#include "read_template.h"
#include "test.h"


test_result_t test_full_adder(void) {
  TEST_START;

  // Create circuit
  circuit_t xor_circ;
  circuit_init(&xor_circ);
  circuit_t ha_circ;
  circuit_init(&ha_circ);
  circuit_t fa_circ;
  circuit_init(&fa_circ);

  vector_t deps;
  vector_init(&deps, 4);

  // Read templates
  assert_true(read_template("tests/xor", &xor_circ, NULL));
  assert_true(vector_push(&deps, &xor_circ));

  assert_true(read_template("tests/half_adder", &ha_circ, &deps));
  assert_true(vector_push(&deps, &ha_circ));

  assert_true(read_template("tests/full_adder", &fa_circ, &deps));


  // ... Do fun things

  // Get all I/O gates

  /*
    0e36e9ea IN #I0
    63f0d9e3 IN #I1
    b9f8820d IN #Ci
    2c9ced3a OUT #S
    57f26486 OUT #Co
  */

  port_t *i0 = circuit_get_port_by_name(&fa_circ, "0e36e9ea", "I0");
  port_t *i1 = circuit_get_port_by_name(&fa_circ, "63f0d9e3", "I1");
  port_t *ici = circuit_get_port_by_name(&fa_circ, "b9f8820d", "Ci");
  port_t *os = circuit_get_port_by_name(&fa_circ, "2c9ced3a", "S");
  port_t *oco = circuit_get_port_by_name(&fa_circ, "57f26486", "Co");

  // Make sure we got them
  assert_neq(i0, NULL);
  assert_neq(i1, NULL);
  assert_neq(ici, NULL);
  assert_neq(os, NULL);
  assert_neq(oco, NULL);

  // Check their types
  assert_eq(i0->type, PortType_OUTPUT);
  assert_eq(i1->type, PortType_OUTPUT);
  assert_eq(ici->type, PortType_OUTPUT);
  assert_eq(os->type, PortType_INPUT);
  assert_eq(oco->type, PortType_INPUT);

  // Make sure default inputs are false
  assert_eq(port_get_state(i0), false);
  assert_eq(port_get_state(i1), false);
  assert_eq(port_get_state(ici), false);

  // A B C -> C S
  // 0 0 0 -> 0 0
  port_set_state(i0, false);
  port_set_state(i1, false);
  port_set_state(ici, false);
  assert_eq(port_get_state(oco), false);
  assert_eq(port_get_state(os), false);

  // A B C -> C S
  // 0 0 1 -> 0 1
  port_set_state(i0, false);
  port_set_state(i1, false);
  port_set_state(ici, true);
  assert_eq(port_get_state(oco), false);
  assert_eq(port_get_state(os), true);

  // A B C -> C S
  // 0 1 0 -> 0 1
  port_set_state(i0, false);
  port_set_state(i1, true);
  port_set_state(ici, false);
  assert_eq(port_get_state(oco), false);
  assert_eq(port_get_state(os), true);

  // A B C -> C S
  // 0 1 1 -> 1 0
  port_set_state(i0, false);
  port_set_state(i1, true);
  port_set_state(ici, true);
  assert_eq(port_get_state(oco), true);
  assert_eq(port_get_state(os), false);

  // A B C -> C S
  // 1 0 0 -> 0 1
  port_set_state(i0, true);
  port_set_state(i1, false);
  port_set_state(ici, false);
  assert_eq(port_get_state(oco), false);
  assert_eq(port_get_state(os), true);

  // A B C -> C S
  // 1 0 1 -> 1 0
  port_set_state(i0, true);
  port_set_state(i1, false);
  port_set_state(ici, true);
  assert_eq(port_get_state(oco), true);
  assert_eq(port_get_state(os), false);

  // A B C -> C S
  // 1 1 0 -> 1 0
  port_set_state(i0, true);
  port_set_state(i1, true);
  port_set_state(ici, false);
  assert_eq(port_get_state(oco), true);
  assert_eq(port_get_state(os), false);

  // A B C -> C S
  // 1 1 1 -> 1 1
  port_set_state(i0, true);
  port_set_state(i1, true);
  port_set_state(ici, true);
  assert_eq(port_get_state(oco), true);
  assert_eq(port_get_state(os), true);



  // Free everything
  circuit_free(&fa_circ);
  circuit_free(&ha_circ);
  circuit_free(&xor_circ);
  vector_free(&deps);

  TEST_END;
}
