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

    // CHECK FOR COMMENT
    char *comment_start = strstr(s, "//");
    if (comment_start) {
        *comment_start = '\0';
    }

		// TRIM LEADING WHITESPACE
		char *start = s;
		while (isspace((unsigned char)*start))
		{
			start++;
		}

    // TRIM ENDING WHITESPACE
		char *end = start + strlen(start - 1);
		while (end > start && isspace((unsigned char)*end))
		{
			*end-- = '\0';
		}

		// MOVE TRIMMED STRING TO START OF s
		if (start != s)
		{
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
void parse(FILE * file){
	// BUFFER FOR EACH LINE TO BE READ
	char line[1024];

	// READ LINE FROM FILE
	while (fgets(line, sizeof(line), file))
	{
		// STRIP COMMENTS AND WHITESPACE
		strip(line);

		// CHECK FOR NON EMPTY LINE AFTER STRIP
		if (line[0] != '\0')
		{
			char instruction_type = ' ';

			if (is_Atype(line))
			{
				instruction_type = 'A';
			}
			else if (is_label(line))
			{
				instruction_type = 'L';
			}
			else if (is_Ctype(line))
			{
				instruction_type = 'C';
			}

			// PRINT instruction_type FOLLOWED BY THE LINE
			printf("%c  %s", instruction_type, line);

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

	// RETURNS A LINE OF AT LEAST 2 DIGITS IN LENGTH FOUND BETWEEN PARENTHESES
	return len > 2 && line[0] == '(' && line[len - 1] == ')';
}

bool is_Ctype(const char *line)
{
	// IF NOT A TYPE OR LABEL THEN IT MUST BE C TYPE
	return !is_Atype(line) && !is_label(line);
}
