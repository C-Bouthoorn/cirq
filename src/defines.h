#ifndef DEFINES_H
#define DEFINES_H


#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// The default size for buffers, vectors, etc
#define BUF_SIZE 256

// Enable or disable debugging
#define DEBUG_ON false


#if DEBUG_ON
	#define DEBUG \
		printf("DEBUG %s:%i %s\n", __FILE__, __LINE__, __FUNCTION__)

	#define DEBUG_PRINT_PORT(p) \
		printf("<0x%04x> %s{%s}:%s\n", (0xffff & (unsigned int) p->gate), p->gate->name, p->gate->type, p->name);


	#define DEBUG_PRINT_GATE(circ, name) { \
		printf("\t"); \
		gate_print(circuit_get_gate_by_name(&circ, name), 0); \
		printf("\n"); \
	}

#else
	#define DEBUG
#endif


#define CONCAT_(x, y) x ## y
#define CONCAT(x, y) CONCAT_(x, y)


// Scope a variable for the following block
#define with(statement) \
	for (char CONCAT(__wx, __LINE__) = 1; CONCAT(__wx, __LINE__);) \
		for (statement; CONCAT(__wx, __LINE__); CONCAT(__wx, __LINE__) = 0) \

// Scope another variable for the following block
#define and_with(statement) \
	for (char CONCAT(__wy, __LINE__) = 1; CONCAT(__wy, __LINE__);) \
		for (statement; CONCAT(__wy, __LINE__); CONCAT(__wy, __LINE__) = 0) \


// Easier string switch'ing
#define switch_str(x) \
	with(char *SWITCH_STR = (x))


#define case_str(y) \
	if (strcmp(SWITCH_STR, (y)) == 0)


/*
// use it like:

switch_str(name) {
	case_str("foo") {

	}

	case_str("bar") {

	}
}
*/


#endif
