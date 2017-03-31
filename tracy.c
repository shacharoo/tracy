#include "tracy.h"


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
void add_error_trace(char const * file, char const * func, int line);

// Get the error traceback in the out parameter. Returns the traceback length.
int get_error_trace(stack_buff_t** out);

// TBD
char const * get_error_msg(void);

void save_traceback_position(void);
void restore_traceback_position(void);

