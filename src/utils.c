#include "utils.h"

#include <stdio.h>
#include <unistd.h>

#include "assert.h"
#include "benchmark.h"


// Redirect stdout to stderr
void STDOUT_TO_STDERR(void) {
	FUNC_START();

	fflush(stdout);
	dup2(2, 1);

	FUNC_END();
}


// Reset the redirection from stdout to stderr
void RESET_STDOUT(void) {
	FUNC_START();

	fflush(stderr);
	dup2(1, 1);

	FUNC_END();
}


#define ABS(x) ( ((x) < 0) ? -(x) : (x) )

char *__LEFT_PAD_SPACE(char *dest, double x, unsigned int width) {
	// Calculate width of number
	unsigned int num_width = 0;

	for (int y = (int) ABS(x); y >= 1; y /= 10) {
		num_width++;
	}

	if (x < 0) {
		num_width++;
	}

	// Add spaces
	if (width > num_width) {
		sprintf(dest, "%*s%.f", width - num_width, " ", x);
	}
	else {
		sprintf(dest, "%.f", x);
	}


	return dest;
}


circuit_t *find_dependency(vector_t *dependencies, char *name) {
	FUNC_START();

	assert_not_null(name);

	if (dependencies == NULL) {
		goto notfound;
	}


	for (size_t i = 0; i < dependencies->amount; i++) {
		circuit_t *dep = dependencies->items[i];

		if (strcmp(dep->name, name) == 0) {
			FUNC_END();
			return dep;
		}
	}


	notfound:
	warn("Failed to find dependency");
	FUNC_END();
	return NULL;
}
