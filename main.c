#include "tracy.h"
#include <stdio.h>

err_t func1(void) {
    START_ERROR(ENOMEM, "SORRY!!");
}

err_t func2(void) {
    RETURN_ON_ERROR(func1());
    printf("should not be printed\n");
}

err_t func3(void) {
    RETURN_ON_ERROR(func2());
    printf("should not be printed\n");
}

err_t main(int argc, char* argv[]) {
    err_t ret = func3();
    if (ret != OK) {
        log_traceback(ret);
    }
    
    return OK;
}