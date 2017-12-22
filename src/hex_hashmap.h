#ifndef HEX_HASHMAP_H
#define HEX_HASHMAP_H


#include "vector.h"


typedef struct hex_hashmap_list {
	char *name;
	void *value;

	struct hex_hashmap_list *next;
} hex_hashmap_list_t;


typedef struct hex_hashmap {
	hex_hashmap_list_t *items[16];
} hex_hashmap_t;


// Loop over all items in the hashmap, only use the value
#define HEX_HASHMAP_EACH_VALUE(map, var) \
	for ( \
		size_t CONCAT(__i, __LINE__) = 0; \
		CONCAT(__i, __LINE__) < 16; \
		CONCAT(__i, __LINE__)++ \
	) \
		if ((map).items[CONCAT(__i, __LINE__)]->name != NULL) \
			for ( \
				hex_hashmap_list_t *list = (map).items[CONCAT(__i, __LINE__)]; \
				list != NULL; \
				list = list->next \
			) \
				with(var = list->value) \


// Loop over all items in the hashmap, only use the key
#define HEX_HASHMAP_EACH_KEY(map, var) \
	for ( \
		size_t CONCAT(__i, __LINE__) = 0; \
		CONCAT(__i, __LINE__) < 16; \
		CONCAT(__i, __LINE__)++ \
	) \
		if ((map).items[CONCAT(__i, __LINE__)]->name != NULL) \
			for ( \
				hex_hashmap_list_t *list = (map).items[CONCAT(__i, __LINE__)]; \
				list != NULL; \
				list = list->next \
			) \
				with(var = list->name) \


// Loop over all items in the hashmap, using both key and value
#define HEX_HASHMAP_EACH_KEY_VALUE(map, keyvar, valvar) \
	for ( \
		size_t CONCAT(__i, __LINE__) = 0; \
		CONCAT(__i, __LINE__) < 16; \
		CONCAT(__i, __LINE__)++ \
	) \
		if ((map).items[CONCAT(__i, __LINE__)]->name != NULL) \
			for ( \
				hex_hashmap_list_t *list = (map).items[CONCAT(__i, __LINE__)]; \
				list != NULL; \
				list = list->next \
			) \
				with(keyvar = list->name) \
					and_with(valvar = list->value) \


// Loop over all items, only use the value, with given index variable
#define HEX_HASHMAP_EACH_VALUE_INDEX(map, var, ix_var) \
	for ( \
		size_t CONCAT(__i, __LINE__) = 0, (ix_var) = 0; \
		CONCAT(__i, __LINE__) < 16; \
		CONCAT(__i, __LINE__)++ \
	) \
		if ((map).items[CONCAT(__i, __LINE__)]->name != NULL) \
			for ( \
				hex_hashmap_list_t *list = (map).items[CONCAT(__i, __LINE__)]; \
				list != NULL; \
				list = list->next, (ix_var)++ \
			) \
				with(var = list->value) \


// Loop over all items, only use the key, with given index variable
#define HEX_HASHMAP_EACH_KEY_INDEX(map, var, ix_var) \
	for ( \
		size_t CONCAT(__i, __LINE__) = 0, (ix_var) = 0; \
		CONCAT(__i, __LINE__) < 16; \
		CONCAT(__i, __LINE__)++ \
	) \
		if ((map).items[CONCAT(__i, __LINE__)]->name != NULL) \
			for ( \
				hex_hashmap_list_t *list = (map).items[CONCAT(__i, __LINE__)]; \
				list != NULL; \
				list = list->next, (ix_var)++ \
			) \
				with(var = list->name) \


// Loop over all items, using both key and value, with given index variable
#define HEX_HASHMAP_EACH_KEY_VALUE_INDEX(map, keyvar, valvar, ix_var) \
	for ( \
		size_t CONCAT(__i, __LINE__) = 0, (ix_var) = 0; \
		CONCAT(__i, __LINE__) < 16; \
		CONCAT(__i, __LINE__)++ \
	) \
		if ((map).items[CONCAT(__i, __LINE__)]->name != NULL) \
			for ( \
				hex_hashmap_list_t *list = (map).items[CONCAT(__i, __LINE__)]; \
				list != NULL; \
				list = list->next, (ix_var)++ \
			) \
				with(keyvar = list->name) \
					and_with(valvar = list->value) \


void *hex_hashmap_get_item(hex_hashmap_t *map, char *name);
bool hex_hashmap_remove_item(hex_hashmap_t *map, char *name);
bool hex_hashmap_add_item(hex_hashmap_t *map, char *name, void *value);
hex_hashmap_list_t *hex_hashmap_get_list(hex_hashmap_t *map, char *name);
size_t hex_hashmap_get_list_index_for_name(char *name);
size_t hex_hashmap_amount(hex_hashmap_t *map);


void *hex_hashmap_list_get_item(hex_hashmap_list_t *list, char *name);
bool hex_hashmap_list_remove_item(hex_hashmap_t *map, hex_hashmap_list_t *list, char *name);
bool hex_hashmap_list_contains_item(hex_hashmap_list_t *list, char *name);
bool hex_hashmap_list_add_item(hex_hashmap_list_t *list, char *name, void *value);
hex_hashmap_list_t *hex_hashmap_list_get_last(hex_hashmap_list_t *list);


void hex_hashmap_init(hex_hashmap_t *map);
void hex_hashmap_free(hex_hashmap_t *map);
void hex_hashmap_list_init(hex_hashmap_list_t *item);
void hex_hashmap_list_free(hex_hashmap_list_t *item);


#endif
