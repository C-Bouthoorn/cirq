#include "hex_hashmap.h"

#include "assert.h"
#include "benchmark.h"


size_t hex_hashmap_get_list_index_for_name(char *name) {
	FUNC_START();

	assert_not_null(name);

	// Get first character
	char c = name[0];

	if (c >= '0' && c <= '9') {
		FUNC_END();
		return (size_t) (c - '0');
	}

	if (c >= 'a' && c <= 'f') {
		FUNC_END();
		return (size_t) (c - 'a' + 10);
	}


	warn("Failed to calculate index for name '%s'", name);

	FUNC_END();
	return (size_t) -1;
}


hex_hashmap_list_t *hex_hashmap_get_list(hex_hashmap_t *map, char *name) {
	FUNC_START();

	assert_not_null(map);
	assert_not_null(name);


	hex_hashmap_list_t *list = map->items[hex_hashmap_get_list_index_for_name(name)];


	FUNC_END();
	return list;
}


void *hex_hashmap_get_item(hex_hashmap_t *map, char *name) {
	FUNC_START();

	assert_not_null(map);
	assert_not_null(name);


	FUNC_PAUSE();
	hex_hashmap_list_t *list = hex_hashmap_get_list(map, name);
	void *item = hex_hashmap_list_get_item(list, name);
	FUNC_RESUME();


	FUNC_END();
	return item;
}


bool hex_hashmap_add_item(hex_hashmap_t *map, char *name, void *value) {
	FUNC_START();

	assert_not_null(map);
	assert_not_null(name);
	// NOTE: Not allowing NULL value
	assert_not_null(value);


	FUNC_PAUSE();
	hex_hashmap_list_t *list = hex_hashmap_get_list(map, name);
	assert_not_null(list);
	FUNC_RESUME();


	// NOTE: Don't allow duplicates
	if (hex_hashmap_list_contains_item(list, name)) {
		panic("Can not add item: already in map");
		FUNC_END();
		return NULL;
	}


	FUNC_PAUSE();
	bool success = hex_hashmap_list_add_item(list, name, value);
	FUNC_RESUME();

	FUNC_END();
	return success;
}


bool hex_hashmap_remove_item(hex_hashmap_t *map, char *name) {
	FUNC_START();

	assert_not_null(map);
	assert_not_null(name);

	FUNC_PAUSE();
	hex_hashmap_list_t *list = hex_hashmap_get_list(map, name);
	assert_not_null(list);

	bool success = hex_hashmap_list_remove_item(map, list, name);
	FUNC_RESUME();

	FUNC_END();
	return success;
}


size_t hex_hashmap_amount(hex_hashmap_t *map) {
	FUNC_START();

	assert_not_null(map);

	// TODO: Optimise

	size_t amount = 0;

	HEX_HASHMAP_EACH_VALUE(*map, void *_x) {
		(void) _x;
		amount++;
	}

	FUNC_END();
	return amount;
}



void *hex_hashmap_list_get_item(hex_hashmap_list_t *list, char *name) {
	FUNC_START();

	assert_not_null(list);
	assert_not_null(name);

	hex_hashmap_list_t *item = list;


	if (item->name == NULL) {
		goto fail;
	}


	while (item != NULL) {
		if (strcmp(item->name, name) == 0) {
			FUNC_END();
			return item->value;
		}

		item = item->next;
	}


	fail:
	warn("Failed to find hex_hashmap item for name '%s'", name);
	FUNC_END();
	return NULL;
}


bool hex_hashmap_list_remove_item(hex_hashmap_t *map, hex_hashmap_list_t *list, char *name) {
	FUNC_START();

	assert_not_null(list);
	assert_not_null(name);

	hex_hashmap_list_t *item = list;


	if (item->name == NULL) {
		goto notfound;
	}


	// Check if first item
	if (strcmp(item->name, name) == 0) {
		if (item->next != NULL) {
			// Replace the first item with the next
			map->items[hex_hashmap_get_list_index_for_name(name)] = item->next;
		}
		else {
			// Reset to initial state
			item->name = NULL;
			item->value = NULL;
		}

		FUNC_END();
		return true;
	}


	while (item->next != NULL) {
		if (strcmp(item->next->name, name) == 0) {
			item->next = item->next->next;

			FUNC_END();
			return true;
		}

		item = item->next;
	}


	notfound:
	warn("Failed to remove item %s, since it's not in the hexmap", name);
	FUNC_END();
	return false;
}


bool hex_hashmap_list_contains_item(hex_hashmap_list_t *list, char *name) {
	FUNC_START();

	assert_not_null(list);
	assert_not_null(name);

	hex_hashmap_list_t *item = list;


	if (item->name == NULL) {
		FUNC_END();
		return false;
	}


	while (item != NULL) {
		if (strcmp(item->name, name) == 0) {
			FUNC_END();
			return true;
		}

		item = item->next;
	}


	FUNC_END();
	return false;
}


bool hex_hashmap_list_add_item(hex_hashmap_list_t *list, char *name, void *value) {
	FUNC_START();

	assert_not_null(list);
	assert_not_null(name);
	// NOTE: Not allowing NULL value
	assert_not_null(value);


	// First item
	if (list->name == NULL) {
		// Replace first item
		list->name = name;
		list->value = value;

		FUNC_END();
		return true;
	}


	// Create new item
	hex_hashmap_list_t *item = malloc(sizeof(hex_hashmap_list_t));
	hex_hashmap_list_init(item);

	item->name = name;
	item->value = value;

	// Add item to list
	FUNC_PAUSE();
	hex_hashmap_list_t *last = hex_hashmap_list_get_last(list);
	FUNC_RESUME();

	last->next = item;

	FUNC_END();
	return true;
}


hex_hashmap_list_t *hex_hashmap_list_get_last(hex_hashmap_list_t *list) {
	FUNC_START();

	assert_not_null(list);

	hex_hashmap_list_t *last = list;


	if (last == NULL) {
		warn("Failed to get last item of list: lits doesn't have any items");
		FUNC_END();
		return NULL;
	}


	while (last->next != NULL) {
		last = last->next;
	}


	FUNC_END();
	return last;
}



void hex_hashmap_init(hex_hashmap_t *map) {
	for (size_t i=0; i < 16; i++) {
		map->items[i] = malloc(sizeof(hex_hashmap_list_t));
		hex_hashmap_list_init(map->items[i]);
	}
}


void hex_hashmap_free(hex_hashmap_t *map) {
	for (size_t i=0; i < 16; i++) {
		hex_hashmap_list_free(map->items[i]);
		free(map->items[i]);
	}
}



void hex_hashmap_list_init(hex_hashmap_list_t *item) {
	item->name = NULL;
	item->value = NULL;
	item->next = NULL;
}


void hex_hashmap_list_free(hex_hashmap_list_t *item) {
	// if (item->name) free(item->name);
	// if (item->value) free(item->value);

	if (item->next != NULL) {
		hex_hashmap_list_free(item->next);
		free(item->next);
	}
}
