#include "vector.h"

#include "assert.h"
#include "benchmark.h"


bool vector_push(vector_t *vec, void *item) {
	FUNC_START();

	assert_not_null(vec);
	// NOTE: Blocks vectors from containing NULL
	assert_not_null(item);

	// Check for overflow
	if (vec->amount == vec->size - 1) {
		panic("Overflowing vector!");
		FUNC_END();
		return false;
	}

	// Add item
	vec->items[vec->amount] = item;

	// Increase counter
	vec->amount++;

	FUNC_END();
	return true;
}


bool vector_copy(vector_t *dest, vector_t *src) {
	FUNC_START();
	bool success = true;

	VEC_EACH(*src, void *item) {
		FUNC_PAUSE();
		success &= vector_push(dest, item);
		FUNC_RESUME();
	}

	FUNC_END();
	return success;
}


bool vector_remove(vector_t *vec, void *item) {
	FUNC_START();

	// Get index of item
	size_t i = 0;
	for (; i < vec->amount; i++) {
		if (vec->items[i] == item) {
			break;
		}
	}

	// Check if index is in bounds
	if (i == vec->amount) {
		warn("Failed to remove item from vector, since it's not in the vector");
		FUNC_END();
		return false;
	}

	// Shift all items after the given item 1 to the left
	for (size_t j = i + 1; j < vec->amount + 1; j++) {
		vec->items[j - 1] = vec->items[j];
	}

	// Decrease amount
	vec->amount--;

	FUNC_END();
	return true;
}



void vector_init(vector_t *vec, size_t size) {
	vec->amount = 0;
	vec->size = size;


	vec->items = malloc(size * sizeof(void*));

	// Set all fields to zero (clear array)
	memset(vec->items, 0, size * sizeof(void*));
}


void vector_free(vector_t *vec) {
	assert_not_null(vec);

	free(vec->items);
}
