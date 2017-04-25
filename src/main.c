#include "tracy.h"
#include <stdio.h>

err_t func1(void) {
    START_ERROR(ENOMEM, "SORRY!!");
    return OK;
}

err_t func2(void) {
    RETURN_ON_ERROR(func1());
    printf("should not be printed\n");
    return OK;
}

err_t func3(void) {
    RETURN_ON_ERROR(func2());
    printf("should not be printed\n");
    return OK;
}

err_t main(void) {
    err_t ret = func3();
    if (ret != OK) {
        log_traceback(ret);
    }
    
    return OK;
}
