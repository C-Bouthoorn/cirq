#include "hhmtest.h"
#include "benchmark.h"


test_result_t populate_map(hex_hashmap_t *map) {
	FUNC_START();
	TEST_START;


	assert_not_null(map);

	{
		item_t *aaa = malloc(sizeof(item_t));
		aaa->value = 0;
		assert_true(hex_hashmap_add_item(map, "aaa", aaa));
	}

	{
		item_t *aab = malloc(sizeof(item_t));
		aab->value = 1;
		assert_true(hex_hashmap_add_item(map, "aab", aab));
	}

	{
		item_t *aba = malloc(sizeof(item_t));
		aba->value = 2;
		assert_true(hex_hashmap_add_item(map, "aba", aba));
	}

	{
		item_t *abb = malloc(sizeof(item_t));
		abb->value = 3;
		assert_true(hex_hashmap_add_item(map, "abb", abb));
	}

	{
		item_t *baa = malloc(sizeof(item_t));
		baa->value = 4;
		assert_true(hex_hashmap_add_item(map, "baa", baa));
	}

	{
		item_t *bab = malloc(sizeof(item_t));
		bab->value = 5;
		assert_true(hex_hashmap_add_item(map, "bab", bab));
	}

	{
		item_t *bba = malloc(sizeof(item_t));
		bba->value = 6;
		assert_true(hex_hashmap_add_item(map, "bba", bba));
	}

	{
		item_t *bbb = malloc(sizeof(item_t));
		bbb->value = 7;
		assert_true(hex_hashmap_add_item(map, "bbb", bbb));
	}


	FUNC_END();
	TEST_END;
}


test_result_t hhmtest(void) {
	FUNC_START();
	TEST_START;


	hex_hashmap_t map;
	hex_hashmap_init(&map);

	// Add items to the map
	// NOTE: Discarding test result
	populate_map(&map);


	// Some basic asserts
	assert_str_eq(hex_hashmap_get_list(&map, "abb")->name, "aaa");
	assert_str_eq(hex_hashmap_get_list(&map, "abb")->next->name, "aab");
	assert_str_eq(hex_hashmap_get_list(&map, "abb")->next->next->name, "aba");
	assert_str_eq(hex_hashmap_get_list(&map, "abb")->next->next->next->name, "abb");


	assert_str_eq(hex_hashmap_get_list(&map, "baa")->name, "baa");
	assert_str_eq(hex_hashmap_get_list(&map, "bbb")->name, "baa");
	assert_str_eq(hex_hashmap_get_list(&map, "bba")->next->next->name, "bba");

	// Test items
	{
		item_t *item;

		item = hex_hashmap_get_item(&map, "bab");
		assert_eq(item->value, 5);

		item = hex_hashmap_get_item(&map, "abb");
		assert_eq(item->value, 3);

		item = hex_hashmap_get_item(&map, "bbb");
		assert_eq(item->value, 7);

		item = hex_hashmap_get_item(&map, "aaa");
		assert_eq(item->value, 0);

		item = hex_hashmap_get_item(&map, "baa");
		assert_eq(item->value, 4);
	}


	// Test macros
	unsigned int amount = 0;

	HEX_HASHMAP_EACH_KEY_VALUE(map, char *name, item_t *item) {
		assert_not_null(name);
		assert_not_null(item);

		// Check bounds of value
		assert_true(item->value >= 0 && item->value <= 7);

		amount++;
	}

	assert_eq(amount, 8);


	// Free everything
	HEX_HASHMAP_EACH_VALUE(map, item_t *item) {
		free(item);
	}
	hex_hashmap_free(&map);


	FUNC_END();
	TEST_END;
}
