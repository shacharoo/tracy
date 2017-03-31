#ifndef __TRACY_H__
#define __TRACY_H__


#include <errno.h>


typedef int err_t;
enum { OK = 0 };


/* Get the string error corresponding to the numeric value of `err`. */
char const * thread_strerror(int err);


/* Start an error traceback. */
void start_error(char const * file, char const * func, int line);


/* Save the initial error message. */
void set_error_msg(char const * fmt, ...);


/* Return the saved error message. */
char const * get_error_msg(void);


#ifdef __cplusplus
/* Overload `set_error_msg` to allow empty __VA_ARGS__ in macros */
/* in C++. */
inline void set_error_msg(void) {}
#endif 


/* Clear the error traceback. */
void clear_error(void);


/* Add a trace point to the error traceback. */
void add_error_trace(char const * file, char const * func, int line);


/* Save the current stack position in a temporary variable. */
void save_traceback_position(void);


/* Restore the stack position from the temprary variable. */
void restore_traceback_position(void);


/* Format the traceback and print it to stderr. */
void log_traceback(err_t err);


/* Same as `log_traceback`, but also clears the error afterwards. */
void log_and_clear_error(err_t err);


/* Same as `log_and_clear_error`, but only if `err` != OK. */
void log_and_clear_on_error(err_t err);


/* -------------------------- Start Error Macros -------------------------- */


/* Start an error traceback and return the error number. */
/* If an error message is given, sets the error message to the traceback. */
#define START_ERROR(err, ...) do { \
  start_error(__FILE__, __FUNCTION__, __LINE__); \
  set_error_msg(__VA_ARGS__); \
  return (err); \
} while (0);


/* Like `START_ERROR`, but only if and only if `predicate` is true. */
#define START_ERROR_IF(predicate, err, ...) do { \
  if ((predicate)) { \
    START_ERROR((err), ##__VA_ARGS__); \
  } \
} while (0);


/* Like `START_ERROR_IF`, but runs `cleanup` before returning. */
#define CLEANUP_START_ERROR_IF(predicate, err, cleanup, ...) do { \
  if ((predicate)) { \
    (cleanup); \
    START_ERROR((err), ##__VA_ARGS__); \
  } \
} while (0)


/* Start error if `res` < 0. Return `errno` as the error. */
#define START_ERROR_ON_ERRNO(res, ...) do { \
  err_t const __seoe_retval = (res); \
  err_t const __seoe_errno = errno; \
  START_ERROR_IF(__seoe_retval < 0, __seoe_errno, ##__VA_ARGS__); \
} while (0)


/* If `ptr` == NULL, start error with `errno`. */
#define START_ERROR_ON_NULL(ptr) do { \
  if ((ptr) == NULL) { \
    int const __seon_errno = errno; \
    START_ERROR(__seon_errno); \
  } \
} while (0)


/* -------------------------- Return Error Macros ------------------------- */


/* Add current position to traceback and return `err`. */
#define RETURN_ERROR(err) do { \
  add_error_trace(__FILE__, __FUNCTION__, __LINE__); \
  return (err); \
} while (0)


/* Like `RETURN_ERROR`, but only if and only if `predicate` is true. */
#define RETURN_ERROR_IF(predicate, err) do { \
  if ((predicate)) { \
    RETURN_ERROR(err); \
  } \
} while (0)


/* Like `RETURN_ERROR`, but only if `err` != OK. */
#define RETURN_ON_ERROR(err) do { \
  err_t const __ror_local_err = (err); \
  RETURN_ERROR_IF(__ror_local_err != OK, __ror_local_err); \
} while (0)


/* Like `RETURN_ON_ERROR`, but runs `cleanup` before returning. */
#define CLEANUP_RETURN_ON_ERROR(err, cleanup) do { \
  err_t const __croe_local_err = (err); \
  if (__croe_local_err != OK) { \
    (cleanup); \
    RETURN_ERROR(__croe_local_err); \
  } \
} while (0)


#endif  /* __TRACY_H__ */
