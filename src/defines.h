#ifndef DEFINES_H
#define DEFINES_H


#include <stdio.h>
#include <string.h>


#define BUF_SIZE 256


#define DEBUG
#ifndef DEBUG
  #define DEBUG printf("DEBUG %s:%i %s\n", __FILE__, __LINE__, __FUNCTION__)
#endif


#define CONCAT_(x, y) x ## y
#define CONCAT(x, y) CONCAT_(x, y)

// Scope a variable for the following block
#define with(statement) \
  for (char CONCAT(x, __LINE__) = 1; CONCAT(x, __LINE__);) \
    for (statement; CONCAT(x, __LINE__); CONCAT(x, __LINE__) = 0) \


// Easier string switch'ing
#define switch_str(x) \
  with(char *SWITCH_STR = (x))


#define case_str(y) \
  if ( strcmp(SWITCH_STR, (y)) == 0 )

/*
  // use it like:

  switch_str(name) {

    case_str("foo") {

    }

    case_str("bar") {

    }
  }
*/


#endif
