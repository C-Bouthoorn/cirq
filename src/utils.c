#include <stdio.h>
#include <unistd.h>

#include "utils.h"


void STDOUT_TO_STDERR(void) {
  fflush(stdout);
  dup2(2, 1);
}


void RESET_STDOUT(void) {
  fflush(stderr);
  dup2(1, 1);
}
