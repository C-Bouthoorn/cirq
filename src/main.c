#include "test.h"

#include "benchmark.h"
#ifdef BENCH
	#include <stdlib.h>
#endif


int main(int argc, char *argv[]) {
	#ifdef BENCH
		bench_prepare();
		atexit(bench_write_states);
	#endif

	FUNC_START();

	#ifdef TEST
		int failed_tests = (int) test_all(argc, argv);

		FUNC_END();
		return failed_tests;
	#else

		// Actual code here

		FUNC_END();
		return EXIT_SUCCESS;
	#endif
}
