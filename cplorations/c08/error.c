#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

// INDEX 0 IS UNUSED
const char *error_messages[] = {
  NULL,
  "Usage: %s [filename]",
  "Cannot open file %s",
  "File contains more than the maximum of %u instructions.",
  "Line %u: %s : Invalid label name",
  "Line %u: %s : Symbol is already defined"
};

void exit_program(enum exitcode code, ...)
{
  va_list arguments;
  va_start(arguments, code);

  // PRINT ERROR MESSAGE
  printf("ERROR: ");
  vfprintf(stdout, error_messages[code], arguments);
  printf("\n");

  va_end(arguments);

  // EXIT THE PROGRAM WITH THE CODE
  exit(code);
}
