#include "tracy.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


char const * KNRM = "\x1B[0m";
char const * KRED = "\x1B[31m";


void log_err_in_red(char const * fmt, va_list argp) {

  char colored_string[1000] = { '\0' };

  strcat(colored_string, KRED);
  strcat(colored_string, fmt);
  strcat(colored_string, KNRM);

  vfprintf(stderr, colored_string, argp);
}


TRC_err func1(void) {
  TRC_START_ERROR(ENOMEM, "SORRY!!");
  return TRC_OK;
}


TRC_err func2(void) {
  TRC_RETURN_ON_ERROR(func1());
  printf("should not be printed\n");
  return TRC_OK;
}


TRC_err func3(void) {
  TRC_RETURN_ON_ERROR(func2());
  printf("should not be printed\n");
  return TRC_OK;
}


TRC_err main(void) {

  TRC_register_err_log_callback(log_err_in_red);

  TRC_err ret = func3();
  if (ret != TRC_OK) {
    TRC_log_traceback(ret);
  }

  return TRC_OK;
}

