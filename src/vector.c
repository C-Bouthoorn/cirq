#include "vector.h"


bool vector_push(vector_t *vec, void *item) {
  // Check for overflow
  if (vec->amount == vec->size - 1) {
    return false;
  }

  // Add item
  vec->items[vec->amount] = item;

  // Increase counter
  vec->amount++;

  return true;
}


void vector_init(vector_t *vec, size_t size) {
  vec->items = malloc(size * sizeof(void*));

  // Set all fields to zero (clear array)
  memset(vec->items, 0, size * sizeof(void*));

  vec->amount = 0;
  vec->size = size;
}


void vector_free(vector_t *vec) {
  free(vec->items);
}
