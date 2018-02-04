#include "benchmark.h"

#include "assert.h"
#include "utils.h"


#define BENCH_STATE_SIZE 1024
static benchmark_state_t BENCH_STATE[BENCH_STATE_SIZE];
static size_t bench_state_index = BENCH_STATE_SIZE;

static vector_t trace;


static const char *bench_file_name = "/tmp/bench";



void bench_prepare(void) {
	vector_init(&trace, BENCH_STATE_SIZE * BENCH_STATE_SIZE);
}


void bench_write_states(void) {
	FILE *outfile = fopen(bench_file_name, "w+");


	for (size_t i=0; i < bench_state_index && bench_state_index != BENCH_STATE_SIZE; i++) {
		benchmark_state_t *state = &BENCH_STATE[i];

		double frac = state->effective_time / state->amount;

		fprintf(outfile, "%s = %s / %i \t%s \t%s\n",
			LEFT_PAD_SPACE(frac, 8),
			LEFT_PAD_SPACE(state->effective_time, 8),
			state->amount,
			LEFT_PAD_SPACE(state->actual_time, 8),
			state->func_name
		);
	}
}


void benchmark_state_init(benchmark_state_t *state) {
	assert_not_null(state);


	state->effective_time = 0;
	state->actual_time = 0;
	state->amount = 0;
}


benchmark_state_t *bench_get_or_create_state_by_name(const char func_name[]) {
	for (size_t i=0; i < bench_state_index && bench_state_index != BENCH_STATE_SIZE; i++) {
		benchmark_state_t *state = &BENCH_STATE[i];

		if (strcmp(state->func_name, func_name) == 0) {
			return state;
		}
	}

	// No state found
	bool is_initial = bench_state_index == BENCH_STATE_SIZE;

	// no existing data found, creating new one
	benchmark_state_t *state = &BENCH_STATE[is_initial ? 0 : bench_state_index];
	benchmark_state_init(state);

	state->func_name = func_name;

	if (is_initial) {
		bench_state_index = 1;
	}
	else {
		bench_state_index++;
	}

	return state;
}


void bench_apply_starttime(benchmark_state_t *state, struct timespec start) {
	assert_not_null(state);


	state->effective_start = start;
	state->actual_start = start;
}


void bench_apply_endtime(benchmark_state_t *state, struct timespec end) {
	assert_not_null(state);


	long effective_duration = double_time(end) - double_time(state->effective_start);
	long actual_duration = double_time(end) - double_time(state->actual_start);

	state->effective_time += effective_duration;
	state->actual_time = actual_duration;
}


void bench_start_func(const char func_name[]) {
	struct timespec start = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &start);

	benchmark_state_t *last = vector_last(&trace);
	if (last != NULL) {
		bench_apply_endtime(last, start);
	}

	benchmark_state_t *state = bench_get_or_create_state_by_name(func_name);
	bench_apply_starttime(state, start);
	state->amount++;
	vector_push(&trace, state);
}


void bench_end_func(const char func_name[]) {
	struct timespec end = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &end);

	benchmark_state_t *state = vector_pop(&trace);
	assert_not_null(state);

	if (strcmp(state->func_name, func_name) != 0) {
		panic("Failed to end function: %s hasn't been started or %s hasn't been ended.", func_name, state->func_name);
		return;
	}

	bench_apply_endtime(state, end);

	benchmark_state_t *last = vector_last(&trace);
	if (last != NULL) {
		last->effective_start = end;
	}
}
