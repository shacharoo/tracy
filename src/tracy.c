#include "tracy.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


enum {
  MAX_USER_ERR_MSG_SIZE = 512,
  MAX_ERR_STR_BUF_SIZE = 1024
};


/* The initial error message. */
typedef struct {
  char buff[MAX_USER_ERR_MSG_SIZE + 1];
} msg_data;


/* ----------------------------- Static Data ------------------------------ */


/* Get the string error corresponding to the numeric value of `err`. */
static char const * get_error_string(int err);


extern int TRC_trace_stack_size;
extern __thread trc_private_stack_buff TRC_trace_stack[];
static __thread int stack_ptr = 0;
static __thread int stack_ptr_save = 0;
static __thread msg_data msg_buf;


/* Callback function for log printing. */
static __thread TRC_err_log_callback log_callback =
    trc_private_default_log_callback;


/* --------------------------- Public Functions --------------------------- */


/* Clear the error traceback. */
void TRC_clear_error(void) {
  stack_ptr = 0;
  msg_buf.buff[0] = '\0';
}


/* Save the current stack position in a temporary variable. */
void TRC_save_traceback_position(void) {
  stack_ptr_save = stack_ptr;
}


/* Restore the stack position from the temprary variable. */
void TRC_restore_traceback_position(void) {
  stack_ptr = stack_ptr_save;
}


/* Format the traceback and log it to stderr. */
void TRC_log_traceback(TRC_err err) {
  int i;
  trc_private_log("CC Traceback:\n");
  for (i = stack_ptr - 1; i >= 0; --i) {
    trc_private_log("  File \"%s\", line %d, in %s\n",
        TRC_trace_stack[i].file,
        TRC_trace_stack[i].line,
        TRC_trace_stack[i].func);
  }
  
  trc_private_log("Error: %s (%d)\n", get_error_string(err), err);

  if (msg_buf.buff[0] != '\0') {
    trc_private_log("Error message: %s\n", msg_buf.buff);
  }
}


/* Same as `TRC_log_traceback`, but also clears the error afterwards. */
void TRC_log_and_clear_error(TRC_err err) {
  TRC_log_traceback(err);
  TRC_clear_error();
}


/* Same as `TRC_log_and_clear_error`, but only if `err` != TRC_OK. */
void TRC_log_and_clear_on_error(TRC_err err) {
  if (err != TRC_OK) {
    TRC_log_and_clear_error(err);
  }
}


void TRC_register_err_log_callback(TRC_err_log_callback callback) {
  if (callback == NULL) {
    callback = trc_private_default_log_callback;
  }

  log_callback = callback;
}


/* -------------------------- Private Functions --------------------------- */


/* Start an error traceback. */
void trc_private_start_error(char const * file, char const * func, int line) {
  trc_private_add_error_trace(file, func, line);
}


/* Add a trace point to the error traceback. */
void trc_private_add_error_trace(
    char const * file,
    char const * func,
    int line) {
  if (stack_ptr >= TRC_trace_stack_size) {
    fprintf(
      stderr,
      "Warning: error stack overflow (no room for stack trace)\n");
    return;
  }

  TRC_trace_stack[stack_ptr].file = file;
  TRC_trace_stack[stack_ptr].func = func;
  TRC_trace_stack[stack_ptr].line = line;
  ++stack_ptr;
}


/* Save the initial error message. */
void trc_private_set_error_msg(char const * fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsnprintf(msg_buf.buff, MAX_USER_ERR_MSG_SIZE, fmt, args);
  va_end(args);
}


/* Get the string error corresponding to the numeric value of `err`. */
static char const * get_error_string(int err) {
  static __thread char errbuf[MAX_ERR_STR_BUF_SIZE + 1];

  #if ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE)
  if (strerror_r(err, errbuf, MAX_ERR_STR_BUF_SIZE) != 0) {
    fprintf(stderr, "Error: strerror_r returned errno %d\n", errno);
    return NULL;
  }
  return errbuf;
#else
  return strerror_r(err, errbuf, MAX_ERR_STR_BUF_SIZE);
#endif
}


void trc_private_log(char const * fmt, ...) {
  va_list args;
  va_start(args, fmt);
  log_callback(fmt, args);
  va_end(args);
}


void trc_private_default_log_callback(char const * fmt, va_list args) {
  vfprintf(stderr, fmt, args);
}

