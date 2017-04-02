# tracy
Tracy is a thread-safe C/C++ stack trace and error propagation utility that makes 
exception-less code beautiful.

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

### Program Structure
If a function can return an errror that should be handled in its calling scope, its return 
value should be `err_t`, which is defined in `tracy.h`. 

If the function finished successfuly, it should `return OK;`. 
Otherwise, it should propagate errors using the tracy macros.

### The Error Macros

#### Error Initiation Macros

```c
/* Start an error traceback and return the error number. */
/* If an error message is given, sets the error message to the traceback. */
START_ERROR(err, ...)
```
This is the most basic error initiation macro. If in some point in the code it has been
decided that there's an error - this macro is used for starting a new traceback.

It (and all other initiation macros) may or may not receive an error message that will
be displayed when the traceback is printed.

---

```c
/* Like `START_ERROR`, but only if and only if `predicate` is true. */
START_ERROR_IF(predicate, err, ...)
```

This macro basically spares this piece of code from the user:

```c
if (some_condition) {
  START_ERROR(ENOMEM);
}
```

---

```c
/* Like `START_ERROR_IF`, but runs `cleanup` before returning. */
CLEANUP_START_ERROR_IF(predicate, err, cleanup, ...)
```

This macro is used to clean up resources allocated in the function. For example:

```c
int* i = malloc(sizeof(int));
...
if (some_condition) {
  free(i);
  START_ERROR(EINVAL);
}
```

This could be replaced by:

```c
int* i = malloc(sizeof(int));
...
CLEANUP_START_ERROR_IF(some_condition, EINVAL, free(i));
```

---

```c
/* Start error if `res` < 0. Return `errno` as the error. */
START_ERROR_ON_ERRNO(res, ...)
```

This macro is extremely useful for system call error handling. It checks `res`, and if
it's a negative number then the global `errno` value is returned. For example:

```c
int i = socket();
START_ERROR_ON_ERRNO(i, "Could not create socket");
```

---

```c
/* If `ptr` == NULL, start error with `errno`. */
START_ERROR_ON_NULL(ptr, ...)
```

Similar to `START_ERROR_ON_ERRNO`, only with not-null validation instead of non-negative validation.

#### Error Propagation Macros

```c
/* Add current position to traceback and return `err`. */
RETURN_ERROR(err)
```

```c
/* Like `RETURN_ERROR`, but only if and only if `predicate` is true. */
RETURN_ERROR_IF(predicate, err)
```

```c
/* Like `RETURN_ERROR`, but only if `err` != OK. */
RETURN_ON_ERROR(err)
```

```c
/* Like `RETURN_ON_ERROR`, but runs `cleanup` before returning. */
CLEANUP_RETURN_ON_ERROR(err, cleanup)
```

#### Error Handling and Cleanup

TBD
