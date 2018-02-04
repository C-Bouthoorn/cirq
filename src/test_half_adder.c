#include "read_template.h"
#include "test.h"
#include "benchmark.h"


test_result_t test_half_adder(void) {
	FUNC_START();
	TEST_START;

	// Create circuit
	circuit_t ha_circ;
	circuit_init(&ha_circ);

	// Read Half-Adder circuit
	assert_true(read_template("tests/half_adder", &ha_circ, NULL));

	// ... Do fun things

	// Get all I/O gates

	/*
	a5987c34 IN #I0
	03808f52 IN #I1
	811b6b1b OUT #C
	382da1d0 OUT #S
	*/

	port_t *i0 = circuit_get_port_by_name(&ha_circ, "a5987c34", "I0");
	port_t *i1 = circuit_get_port_by_name(&ha_circ, "03808f52", "I1");
	port_t *oc = circuit_get_port_by_name(&ha_circ, "811b6b1b", "C");
	port_t *os = circuit_get_port_by_name(&ha_circ, "382da1d0", "S");

	// Make sure we got them
	assert_not_null(i0);
	assert_not_null(i1);
	assert_not_null(oc);
	assert_not_null(os);

	// Check their types
	assert_eq(i0->type, PortType_OUTPUT);
	assert_eq(i1->type, PortType_OUTPUT);
	assert_eq(oc->type, PortType_INPUT);
	assert_eq(os->type, PortType_INPUT);

	// Make sure default inputs are false
	assert_eq(i0->state, false);
	assert_eq(i1->state, false);

	// 0 0 -> 0 0
	port_set_state(i0, false);
	port_set_state(i1, false);
	assert_eq(oc->state, false);
	assert_eq(os->state, false);

	// 0 1 -> 0 1
	port_set_state(i0, false);
	port_set_state(i1, true);
	assert_eq(oc->state, false);
	assert_eq(os->state, true);

	// 1 0 -> 0 1
	port_set_state(i0, true);
	port_set_state(i1, false);
	assert_eq(oc->state, false);
	assert_eq(os->state, true);

	// 1 1 -> 1 0
	port_set_state(i0, true);
	port_set_state(i1, true);
	assert_eq(oc->state, true);
	assert_eq(os->state, false);


	// Free everything
	circuit_free(&ha_circ);


	FUNC_END();
	TEST_END;
}
