#ifndef __TRACY_H__
#define __TRACY_H__


#include <errno.h>


static unsigned int const MAX_STACK_SIZE = 1024;
static unsigned int const MAX_MSG_SIZE = 512;


typedef struct {
  char const * file;
  char const * func;
  int line;
} stack_buff_t;


typedef int err_t;


enum {
  OK = 0
};


typedef struct {
  stack_buff_t buffer[MAX_STACK_SIZE];
} error_stack_t;


typedef struct {
  char buff[MAX_MSG_SIZE];
} msg_data_t;


char const * thread_strerror(int errnum);


// Start an error trace.
void start_error(char const * file, char const * func, int line);

// Set an error message.
void set_error_msg(char const * fmt, ...);

#ifdef __cplusplus
// Overload `set_error_msg` to allow empty __VA_ARGS__ in macros
// in C++.
inline void set_error_msg(void) {}
#endif 


// Clear the error trace.
void clear_error(void);

// Add a trace point to the error traceback.
void add_error_trace(char const * file, char const * func, int line);

// Get the error traceback in the out parameter. Returns the traceback length.
int get_error_trace(stack_buff_t** out);

// TBD
char const * get_error_msg(void);



void save_traceback_position(void);
void restore_traceback_position(void);

/* ----------------------------- Error Macros ----------------------------- */


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

#endif  /* __TRACY_H__ */
