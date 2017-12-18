#include "test.h"
#include "defines.h" // switch_str, case_str


int main(int argc, char *argv[]) {
  TESTS_START;

  // If no tests specified, run some default tests
  if (argc == 1) {
    TEST(test_nand);
    TEST(test_nor);
    TEST(test_xand);
    TEST(test_xor);

    TEST(test_half_adder);
    TEST(test_full_adder);
  }


  for (int i = 1; i < argc; i++) {
    switch_str(argv[i]) {
      case_str("full_adder") {
        TEST(test_full_adder);
      }

      else case_str("half_adder") {
        TEST(test_half_adder);
      }

      else case_str("nand") {
        TEST(test_nand);
      }

      else case_str("nor") {
        TEST(test_nor);
      }

      else case_str("not_loop") {
        printf("\nNOTE: This should crash with a stack-overflow\n");
        TEST(test_not_loop);
      }

      else case_str("xand") {
        TEST(test_xand);
      }

      else case_str("xor") {
        TEST(test_xor);
      }
    }
  }


  TESTS_RESULT;
  return (int) __failed_tests;
}
