#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <errno.h>
#include <signal.h>


#define panic(str) { \
  fprintf(stderr, "[" __FILE__ ":%i] %s (%i)\n", __LINE__, str, errno); \
  /* Force crash with stacktrace: */ \
  raise(SIGSEGV); \
}

#define assert(x) \
  if (! (x)) \
    panic(#x " is not true");

#define assert_eq(x, y) \
  if ((x) != (y)) \
    panic(#x " != " #y);

#define assert_neq(x, y) \
  if ((x) == (y)) \
    panic(#x " == " #y);

#endif
