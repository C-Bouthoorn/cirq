#ifndef UTILS_H
#define UTILS_H


#include "circuit.h"


void STDOUT_TO_STDERR(void);
void RESET_STDOUT(void);
circuit_t *find_dependency(vector_t *dependencies, char *name);

#endif
