/****************************************
 * C-ploration 4 for CS 271
 *
 * [NAME] PRESTON EVERSAUL
 * [TERM] FALL 2024
 *
 ****************************************/
#include "parser.h"
#include <string.h>
#include "symtable.h"


/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
 char *strip(char *s) {

     // REMOVE COMMENTS
     char *comment_start = strstr(s, "//");
     if (comment_start) {
         *comment_start = '\0';
     }

     // REMOVE TRAILING NEWLINE
     size_t len = strlen(s);
     if (len > 0 && s[len - 1] == '\n') {
         s[len - 1] = '\0';
     }

     // TRIM LEADING WHITESPACE
     char *start = s;
     while (isspace((unsigned char)*start)) {
         start++;
     }

     // TRIM TRAILING WHITESPACE
     char *end = start + strlen(start) - 1;
     while (end > start && isspace((unsigned char)*end)) {
         *end-- = '\0';
     }

     // MOVE TRIMMED STRING TO START OF s
     if (start != s) {
         memmove(s, start, strlen(start) + 1);
     }

     return s;
 }

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments.
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
void parse(FILE *file){
	// BUFFER FOR EACH LINE TO BE READ
	char line[MAX_LINE_LENGTH];
  char label[MAX_LABEL_LENGTH + 1];
  int line_number = 0;

	// READ LINE FROM FILE
	while (fgets(line, sizeof(line), file))
	{
		// STRIP COMMENTS AND WHITESPACE
		strip(line);

		// CHECK FOR NON EMPTY LINE AFTER STRIP
		if (line[0] != '\0')
		{
      if (is_label(line))
			{
        extract_label(line, label);
        symtable_insert(label, line_number);
		  } else
      {
        if (is_Atype(line) || is_Ctype(line))
        {
          line_number++;
        }
      }
    }
	}
}

bool is_Atype(const char *line)
{
	return line[0] == '@';
}

bool is_label(const char *line)
{
	size_t len = strlen(line);
	// CHECKS FOR STARTING ( AND AN ENDING )
  bool result = len > 2 && line[0] == '(' && line[len - 1] == ')';
  return result;
}


bool is_Ctype(const char *line)
{
	// IF NOT A TYPE OR LABEL THEN IT MUST BE C TYPE
	return !is_Atype(line) && !is_label(line);
}

/* Function extract_label */
char *extract_label(const char *line, char *label)
{
  // CHEK IF INPUT LINE STARTS WITH ( AND ENDS WITH )
  if (line[0] == '(' && line[strlen(line) - 1] == ')')
  {
    // COPY INFO BETWEEN () INTO label
    strncpy(label, line + 1, strlen(line) - 2);
    label[strlen(line) - 2] = '\0';
  }
  return label;
}
