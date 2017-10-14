[![Build Status](https://travis-ci.org/shacharoo/tracy.svg?branch=master)](https://travis-ci.org/shacharoo/tracy)

# tracy
Tracy is a thread-safe stack trace and error propagation utility for C that makes 
exception-less code readable and beautiful.

### Example

```c
#include <tracy.h>

TRC_ALLOCATE_TRACE_STACK(1024)

TRC_err inner_func(void) {
  TRC_START_ERROR(ENOMEM, "Could not allocate... anything!");
}

TRC_err top_func(void) {
  TRC_RETURN_ON_ERROR(inner_func());
  return TRC_OK;
}

TRC_err main(void) {
  TRC_log_and_clear_on_error(top_func());
  return TRC_OK;
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

### Allocating the trace stack

The trace stack must be allocated using the `TRC_ALLOCATE_TRACE_STACK` macro.
It must be placed in the global scope of one of the linked source files.
The given size will determine the maximum lines logged in the traceback.

```c
/* Allocate the trace stack buffer in the global scope with given size.
   The given size will determine the maximum lines logged in the traceback. */
TRC_ALLOCATE_TRACE_STACK(max_traceback_lines)
```

Failing to do so will cause a linkage error that may look like this:

```shell
tracy.c: undefined reference to `TRC_trace_stack_size'
tracy.c: undefined reference to `__emutls_v.TRC_trace_stack'
```

If you see a link error like this, you probably didn't use `TRC_ALLOCATE_TRACE_STACK`,
or didn't link the file that uses it.

### Program Structure
If a function can return an error that should be handled in its calling scope, its return 
value should be `TRC_err`, which is defined in `tracy.h`. 

If the function finished successfuly, it should `return TRC_OK;`. 
Otherwise, it should propagate errors using the tracy macros.

### The Error Macros

#### Error Initiation Macros

```c
/* Start an error traceback and return the error number. */
/* If an error message is given, sets the error message to the traceback. */
TRC_START_ERROR(err, ...)
```
This is the most basic error initiation macro. If in some point in the code it has been
decided that there's an error - this macro is used for starting a new traceback.

It (and all other initiation macros) may or may not receive an error message that will
be displayed when the traceback is printed.

---

```c
/* Like `TRC_START_ERROR`, but only if and only if `predicate` is true. */
TRC_START_ERROR_IF(predicate, err, ...)
```

This macro basically spares this piece of code from the user:

```c
if (some_condition) {
  TRC_START_ERROR(ENOMEM);
}
```

---

```c
/* Like `TRC_START_ERROR_IF`, but runs `cleanup` before returning. */
TRC_CLEANUP_START_ERROR_IF(predicate, err, cleanup, ...)
```

This macro is used to clean up resources allocated in the function. For example:

```c
int* i = malloc(sizeof(int));
...
if (some_condition) {
  free(i);
  TRC_START_ERROR(EINVAL);
}
```

This could be replaced by:

```c
int* i = malloc(sizeof(int));
...
TRC_CLEANUP_START_ERROR_IF(some_condition, EINVAL, free(i));
```

---

```c
/* Start error if `res` < 0. Return `errno` as the error. */
TRC_START_ERROR_ON_ERRNO(res, ...)
```

This macro is extremely useful for system call error handling. It checks `res`, and if
it's a negative number then the global `errno` value is returned. For example:

```c
int i = socket();
TRC_START_ERROR_ON_ERRNO(i, "Could not create socket");
```

---

```c
/* If `ptr` == NULL, start error with `errno`. */
TRC_START_ERROR_ON_NULL(ptr, ...)
```

Similar to `TRC_START_ERROR_ON_ERRNO`, only with not-null validation instead of non-negative validation.

#### Error Propagation Macros

```c
/* Add current position to traceback and return `err`. */
TRC_RETURN_ERROR(err)
```

```c
/* Like `TRC_RETURN_ERROR`, but only if and only if `predicate` is true. */
TRC_RETURN_ERROR_IF(predicate, err)
```

```c
/* Like `TRC_RETURN_ERROR`, but only if `err` != TRC_OK. */
TRC_RETURN_ON_ERROR(err)
```

```c
/* Like `TRC_RETURN_ON_ERROR`, but runs `cleanup` before returning. */
TRC_CLEANUP_RETURN_ON_ERROR(err, cleanup)
```

#### Error Handling and Cleanup

The default handler for errors will log the error to `stderr`. Tracy allows
registering of different log handlers using:

```c
typedef void (*TRC_err_log_callback)(char const * fmt, va_list vargs);

/* Register an output handler for Tracy's outputs */
void TRC_register_err_log_callback(TRC_err_log_callback callback);
```

Sending NULL to this function will re-register the default tracy log handler.
