/****************************************
 * C-ploration 4 for CS 271
 *
 * [NAME] PRESTON EVERSAUL
 * [TERM] FALL 2024
 *
 ****************************************/
#include "parser.h"

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){

	// POINTERS TO TRAVERSE STRING AND BUILD STRIPPED STRING
	char *p = s;
	char*q = s;

	while(*p)
	{
		// CHECK FOR COMMENT START
		if (*p == '/' && *(p + 1) == '/')
		{
			break;
		}
		// CHECK FOR NO WHITESPACE
		if (!isspace((unsigned char)*p))
		{
			// COPY CHARACTER TO q AND INCREMENT p
			*q++ = *p;
		}
		p++;
	}
	// NULL TERMINATE STRIPPED STR
	g* = '\0'

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
void parse(FILE * file){
	// BUFFET FOR EACH LINE TO BE READ
	char line[1024];

	// READ LINE FROM FILE
	while (fgets(line, sizeof(line), file))
	{
		// STRIP COMMENTS AND WHITESPACE
		strip(line);

		// CHECK FOR NON EMPTY LINE AFTER STRIP
		if (line[0] != '\0')
		{
			printf("%s\n", line);
		}
	}
}
