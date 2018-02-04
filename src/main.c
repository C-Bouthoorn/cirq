#include "test.h"

#ifdef BENCH
	#include <stdlib.h>
	#include "benchmark.h"
#endif


int main(int argc, char *argv[]) {
	#ifdef BENCH
		bench_prepare();

		FUNC_START();
		atexit(bench_write_states);
	#endif


	int failed_tests = (int) test_all(argc, argv);


	#ifdef BENCH
		FUNC_END();
	#endif

	return failed_tests;
}
