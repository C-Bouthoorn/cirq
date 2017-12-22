#ifndef HHMTEST_H
#define HHMTEST_H


#include "hex_hashmap.h"
#include "test.h"


typedef struct item {
	int value;
} item_t;


test_result_t populate_map(hex_hashmap_t *map);
test_result_t hhmtest(void);


#endif
