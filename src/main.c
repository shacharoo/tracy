#include "tracy.h"
#include <stdio.h>

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
  TRC_err ret = func3();
  if (ret != TRC_OK) {
    TRC_log_traceback(ret);
  }

  return TRC_OK;
}
