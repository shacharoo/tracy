#ifndef TRACY_H
#define TRACY_H


#include <errno.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef int TRC_err;
enum { TRC_OK = 0 };


/* --------------------------- Public Functions --------------------------- */


/* Clear the error traceback. */
void TRC_clear_error(void);


/* Save the current stack position in a temporary variable. */
void TRC_save_traceback_position(void);


/* Restore the stack position from the temprary variable. */
void TRC_restore_traceback_position(void);


/* Format the traceback and log it to stderr. */
void TRC_log_traceback(TRC_err err);


/* Same as `TRC_log_traceback`, but also clears the error afterwards. */
void TRC_log_and_clear_error(TRC_err err);


/* Same as `TRC_log_and_clear_error`, but only if `err` != TRC_OK. */
void TRC_log_and_clear_on_error(TRC_err err);


typedef void (*TRC_err_log_callback)(char const * fmt, va_list vargs);

/* Register an output handler for Tracy's outputs */
void TRC_register_err_log_callback(TRC_err_log_callback callback);


/* -------------------------- Private Functions --------------------------- */


/* Negotiator function for the listed log callback */
void trc_private_log(char const * fmt, ...);
#ifdef __cplusplus
/* Overload `trc_private_log` to allow empty __VA_ARGS__ in macros */
/* in C++. */
inline void trc_private_log(void) {}
#endif


/* Default callback function for printing. Log the given format to stderr */
void trc_private_default_log_callback(char const * fmt, va_list args);


/* Start an error traceback. */
void trc_private_start_error(char const * file, char const * func, int line);


/* Add a trace point to the error traceback. */
void trc_private_add_error_trace(
  char const * file,
  char const * func,
  int line);


/* Save the initial error message. */
void trc_private_set_error_msg(char const * fmt, ...);
#ifdef __cplusplus
/* Overload `trc_private_set_error_msg` to allow empty __VA_ARGS__ in macros */
/* in C++. */
inline void trc_private_set_error_msg(void) {}
#endif 


/* -------------------------- Start Error Macros -------------------------- */


/* Start an error traceback and return the error number. */
/* If an error message is given, sets the error message to the traceback. */
#define TRC_START_ERROR(err, ...) do { \
  trc_private_start_error(__FILE__, __FUNCTION__, __LINE__); \
  trc_private_set_error_msg(__VA_ARGS__); \
  return (err); \
} while (0);


/* Like `TRC_START_ERROR`, but only if and only if `predicate` is true. */
#define TRC_START_ERROR_IF(predicate, err, ...) do { \
  if ((predicate)) { \
    TRC_START_ERROR((err), ##__VA_ARGS__); \
  } \
} while (0);


/* Like `TRC_START_ERROR_IF`, but runs `cleanup` before returning. */
#define TRC_CLEANUP_START_ERROR_IF(predicate, err, cleanup, ...) do { \
  if ((predicate)) { \
    (cleanup); \
    TRC_START_ERROR((err), ##__VA_ARGS__); \
  } \
} while (0)


/* Start error if `res` < 0. Return `errno` as the error. */
#define TRC_START_ERROR_ON_ERRNO(res, ...) do { \
  TRC_err const trc_private_seoe_retval = (res); \
  TRC_err const trc_private_seoe_errno = errno; \
  TRC_START_ERROR_IF( \
    trc_private_seoe_retval < 0, \
    trc_private_seoe_errno, \
    ##__VA_ARGS__); \
} while (0)


/* If `ptr` == NULL, start error with `errno`. */
#define TRC_START_ERROR_ON_NULL(ptr, ...) do { \
  if ((ptr) == NULL) { \
    int const trc_private_seon_errno = errno; \
    TRC_START_ERROR(trc_private_seon_errno, ##__VA_ARGS__); \
  } \
} while (0)


/* -------------------------- Return Error Macros ------------------------- */


/* Add current position to traceback and return `err`. */
#define TRC_RETURN_ERROR(err) do { \
  trc_private_add_error_trace(__FILE__, __FUNCTION__, __LINE__); \
  return (err); \
} while (0)


/* Like `TRC_RETURN_ERROR`, but only if and only if `predicate` is true. */
#define TRC_RETURN_ERROR_IF(predicate, err) do { \
  if ((predicate)) { \
    TRC_RETURN_ERROR(err); \
  } \
} while (0)


/* Like `TRC_RETURN_ERROR`, but only if `err` != TRC_OK. */
#define TRC_RETURN_ON_ERROR(err) do { \
  TRC_err const trc_private_ror_local_err = (err); \
  TRC_RETURN_ERROR_IF( \
    trc_private_ror_local_err != TRC_OK, \
    trc_private_ror_local_err); \
} while (0)


/* Like `TRC_RETURN_ON_ERROR`, but runs `cleanup` before returning. */
#define TRC_CLEANUP_RETURN_ON_ERROR(err, cleanup) do { \
  TRC_err const trc_private_croe_local_err = (err); \
  if (trc_private_croe_local_err != TRC_OK) { \
    (cleanup); \
    TRC_RETURN_ERROR(trc_private_croe_local_err); \
  } \
} while (0)


#ifdef __cplusplus
}  /* Close the `extern c` section. */
#endif


#endif  /* TRACY_H */
