#ifndef ASSERT_H
#define ASSERT_H


#include <stdio.h>
#include <errno.h>
#include <signal.h>

#include "color.h"


#define warn(fmt, VARGS...) \
	fprintf(stderr, C_LIGHT_YELLOW "[" __FILE__ ":%i] " fmt "\n" C_RESET_FG, __LINE__, ##VARGS);


#define panic(fmt, VARGS...) { \
	fprintf(stderr, C_LIGHT_RED "[" __FILE__ ":%i] " fmt " (E%i)\n" C_RESET_FG, __LINE__, ##VARGS, errno); \
	/* Force crash with stacktrace: */ \
	fprintf(stderr, "\n\n" C_BOLD C_BG_RED "!!! FORCED SEGMENTATION FAULT BECAUSE OF PANIC !!!" C_RESET_BOLD C_RESET_BG "\n"); \
	raise(SIGSEGV); \
}


#ifndef IGNORE_ASSERT
	#define assert(x) \
		if (! (x)) \
			panic(#x " is not true");

	#define assert_not_null(x) \
		if (x == NULL) \
			panic(#x " is NULL");


	#define assert_eq(x, y) \
		if ((x) != (y)) \
			panic(#x " (%i) != " #y " (%i)", (x), (y));


	#define assert_str_eq(x, y) \
		if (strcmp((x), (y)) != 0) \
			panic(#x " != " #y);


	#define assert_neq(x, y) \
		if ((x) == (y)) \
			panic(#x " == " #y " (%i)", (y));


	#define assert_str_neq(x, y) \
		if (strcmp((x), (y)) == 0) \
			panic(#x " == " #y);

#else

	#define assert(x) ((void) x);
	#define assert_not_null(x) ((void) x);
	#define assert_eq(x, y) ((void) x);
	#define assert_str_eq(x, y) ((void) x);
	#define assert_neq(x, y) ((void) x);
	#define assert_str_neq(x, y) ((void) x);

#endif


#endif
