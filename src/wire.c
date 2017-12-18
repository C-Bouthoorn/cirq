#include "wire.h"

#include <stdlib.h>

#include "assert.h"


void wire_print(wire_t *wire) {
  assert_neq(wire, NULL);

  printf("%s:%s <---> %s:%s", wire->leftuuid, wire->leftport, wire->rightuuid, wire->rightport);
}


void wire_init(wire_t *wire) {
  wire->leftuuid = NULL;
  wire->leftport = NULL;
  wire->rightuuid = NULL;
  wire->rightport = NULL;
}


void wire_free(wire_t *wire) {
  if (wire->leftuuid) free(wire->leftuuid);
  if (wire->leftport) free(wire->leftport);
  if (wire->rightuuid) free(wire->rightuuid);
  if (wire->rightport) free(wire->rightport);
}
