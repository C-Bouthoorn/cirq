#include "read_template.h"
#include "test.h"


test_result_t test_not_loop(void) {
  TEST_START;

  // Create circuit
  circuit_t *circ = malloc(sizeof(circuit_t));
  circuit_init(circ);

  // Read template
  assert_false(read_template("tests/not_loop", circ, NULL));

  // Free everything
  circuit_free(circ);
  free(circ);

  TEST_END;
}
