#ifndef UTILS_H
#define UTILS_H


#include "circuit.h"


void STDOUT_TO_STDERR(void);
void RESET_STDOUT(void);

char *__LEFT_PAD_SPACE(char *dest, double x, unsigned int width);
#define LEFT_PAD_SPACE(x, y) __LEFT_PAD_SPACE((char [16]){""}, (x), (y))

circuit_t *find_dependency(vector_t *dependencies, char *name);


#endif
