#ifndef BENCHMARK_H
#define BENCHMARK_H


#include <time.h>


#define double_time(time) ((double) time.tv_sec + 1.0e-9 * time.tv_nsec)


#ifdef BENCH
	#define FUNC_START() \
		struct timespec __actual_start = {0, 0}, __actual_end = {0, 0}, __tmp = {0, 0}; \
		clock_gettime(CLOCK_MONOTONIC, &__tmp); \
		__actual_start = __tmp; \
		double __start = double_time(__tmp), __end = 0; \
		// printf("start %lf\n", __start); \

	#define FUNC_PAUSE() ; \
		clock_gettime(CLOCK_MONOTONIC, &__tmp); \
		__end = double_time(__tmp); \
		// printf("pause %lf %lf\n", __end, __end - __start); \


	#define FUNC_RESUME() ; \
		clock_gettime(CLOCK_MONOTONIC, &__tmp); \
		__start = double_time(__tmp) - (__end - __start); \
		// printf("resume %lf\n", __start); \


	#define FUNC_END() \
		clock_gettime(CLOCK_MONOTONIC, &__tmp); \
		__actual_end = __tmp; \
		__end = double_time(__tmp); \
		BENCH_ADD(__FUNCTION__, __end - __start, double_time(__actual_end) - double_time(__actual_start)); \
		// printf("end %lf %lf\n", __end,__end - __start); \

#else
	#define FUNC_START()
	#define FUNC_PAUSE()
	#define FUNC_RESUME()
	#define FUNC_END()

#endif


void BENCH_ADD(const char func[], double effective_time, double actual_time);


#endif
