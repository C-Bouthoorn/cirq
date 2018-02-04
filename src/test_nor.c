#include "read_template.h"
#include "test.h"
#include "benchmark.h"


test_result_t test_nor(void) {
	FUNC_START();
	TEST_START;

	// Create circuit
	circuit_t *circ = malloc(sizeof(circuit_t));
	circuit_init(circ);

	// Read template
	assert_true(read_template("tests/nor", circ, NULL));

	// Get all I/O gates
	port_t *i0 = circuit_get_port_by_name(circ, "832532c7", "O0");
	port_t *i1 = circuit_get_port_by_name(circ, "eb2ff896", "O0");
	port_t *o0 = circuit_get_port_by_name(circ, "26cbc7d3", "I0");

	// Make sure we got them
	assert_not_null(i0);
	assert_not_null(i1);
	assert_not_null(o0);

	// Check their types
	assert_eq(i0->type, PortType_OUTPUT);
	assert_eq(i1->type, PortType_OUTPUT);
	assert_eq(o0->type, PortType_INPUT);

	// Make sure default inputs are false
	assert_eq(i0->state, false);
	assert_eq(i1->state, false);

	// 0 0 -> 1
	port_set_state(i0, false);
	port_set_state(i1, false);
	assert_eq(o0->state, true);

	// 0 1 -> 0
	port_set_state(i0, true);
	port_set_state(i1, false);
	assert_eq(o0->state, false);

	// 1 0 -> 0
	port_set_state(i0, false);
	port_set_state(i1, true);
	assert_eq(o0->state, false);

	// 1 1 -> 0
	port_set_state(i0, true);
	port_set_state(i1, true);
	assert_eq(o0->state, false);

	// Free everything
	circuit_free(circ);
	free(circ);


	FUNC_END();
	TEST_END;
}
