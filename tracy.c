#include "tracy.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>


static error_stack_t local_stack;
static int stack_ptr = 0;
static int stack_ptr_save = 0;
static msg_data_t msg_buf;


char const * thread_strerror(int errnum) {
  int const max_errbuf_size = 1024;
  typedef char error_buff_t[max_errbuf_size];
  static error_buff_t errbuf;
  return strerror_r(errnum, errbuf, max_errbuf_size);
}


// Start an error trace.
void start_error(char const * file, char const * func, int line) {
  add_error_trace(file, func, line);
}

// Set an error message.
void set_error_msg(char const * fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsnprintf(msg_buf.buff, MAX_MSG_SIZE, fmt, args);
  va_end(args);
}

void clear_error(void) {
  stack_ptr = 0;
  msg_buf.buff[0] = '\0';
}

// Add a trace point to the error traceback.
void add_error_trace(char const * file, char const * func, int line) {
  if (stack_ptr >= MAX_STACK_SIZE) {
    fprintf(stderr, "Warning: error stack overflow (no room for stack trace)\n");
    return;
  }

  local_stack.buffer[stack_ptr].file = file;
  local_stack.buffer[stack_ptr].func = func;
  local_stack.buffer[stack_ptr].line = line;
  ++stack_ptr;
}

// Get the error traceback in the out parameter. Returns the traceback length.
int get_error_trace(stack_buff_t** out) {
  *out = local_stack.buffer;
  return stack_ptr;
}

// TBD
char const * get_error_msg(void) {
  return msg_buf.buff;
}

void save_traceback_position(void) {
  stack_ptr_save = stack_ptr;
}

void restore_traceback_position(void) {
  stack_ptr = stack_ptr_save;
}




#ifdef __cplusplus

void log_traceback(err_t err) {
  fprintf(stderr, "CC Traceback:\n");
  for (int i = stack_ptr - 1; i >= 0; --i) {
    fprintf(stderr, "  File \"%s\", line %d, in %s\n", 
        local_stack.buffer[i].file,
        local_stack.buffer[i].line,
        local_stack.buffer[i].func);
  }
  
  fprintf(stderr, "Error: %s (%d)\n", thread_strerror(err), err);

  if (msg_buf.buff[0] != '\0') {
    fprintf(stderr, "Error message: %s\n", msg_buf.buff);
  }
}

#endif