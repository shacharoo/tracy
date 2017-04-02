# tracy
Tracy is a C/C++ stack trace and error propagation utility that makes 
exception-less code beautiful.

### Program Structure
If a function can return an errror that should be handled in its calling scope, its return 
value should be `err_t`, which is defined in `tracy.h`. 

If the function finished successfuly, it should `return OK;`. 
Otherwise, it should propagate errors using the tracy macros.

### Example

```c
#include <tracy.h>

err_t inner_func(void) {
  START_ERROR(ENOMEM, "Could not allocate... anything!");
}

err_t top_func(void) {
  RETURN_ON_ERROR(inner_func());
  return OK;
}

err_t main(void) {
  log_and_clear_on_error(top_func());
  return OK;
}
```

The output of this program will be:

```
CC Traceback:
  File "main.c", line 8, in top_func
  File "main.c", line 4, in inner_func
Error: Cannot allocate memory (12)
Error message: Could not allocate... anything!
```

Nice and clean.

Tracy has a lot of useful error macros to offer. Read all about them below!

### Error Propagation Macros

TBD
