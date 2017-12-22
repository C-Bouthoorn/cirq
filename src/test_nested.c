#include "read_template.h"
#include "test.h"


#if DEBUG_ON
	#define DEBUG_PRINT() \
		DEBUG_PRINT_GATE(notor2, "243a080a"); \
		DEBUG_PRINT_GATE(notor2, "a3b247c8"); \
		DEBUG_PRINT_GATE(notor2, "8edeefd9"); \
		\
		DEBUG_PRINT_GATE(notor2, "751a7396"); \
		\
		DEBUG_PRINT_GATE(notor2, "a0055d19"); \
		DEBUG_PRINT_GATE(notor2, "8a8bda59"); \

#endif


test_result_t test_nested(void) {
	TEST_START;

	// Create circuit
	circuit_t notor;
	circuit_init(&notor);
	circuit_t notor2;
	circuit_init(&notor2);

	vector_t deps;
	vector_init(&deps, 4);

	// Read templates
	assert_true(read_template("tests/notor", &notor, NULL));
	assert_true(vector_push(&deps, &notor));

	assert_true(read_template("tests/notor2", &notor2, &deps));


	// ... Do fun things

	// Get all I/O gates

	/*
	243a080a IN #I0
	a3b247c8 IN #I1
	8edeefd9 OUT #O0

	751a7396 OR
	a0055d19 notor
	8a8bda59 notor
	*/

	port_t *i0 = circuit_get_port_by_name(&notor2, "243a080a", "I0");
	port_t *i1 = circuit_get_port_by_name(&notor2, "a3b247c8", "I1");
	port_t *o0 = circuit_get_port_by_name(&notor2, "8edeefd9", "O0");

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

	#if DEBUG_ON
		DEBUG_PRINT()
	#endif

	// Try something
	port_set_state(i0, false);
	port_set_state(i1, true);
	assert_eq(o0->state, true);
	// DEBUG_PRINT();

	port_set_state(i0, true);
	port_set_state(i1, true);
	assert_eq(o0->state, true);
	// DEBUG_PRINT();

	port_set_state(i0, false);
	port_set_state(i1, false);
	assert_eq(o0->state, true);
	// DEBUG_PRINT();



	// Free everything
	circuit_free(&notor2);
	circuit_free(&notor);
	vector_free(&deps);

	TEST_END;
}
