/****************************************
 * C-ploration 4 for CS 271
 *
 * [NAME] PRESTON EVERSAUL
 * [TERM] FALL 2024
 *
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"

int main(int argc, const char *argv[])
{
	// CHECKING IF NUMBER OF ARGS IS CORRECT
	if (argc != 2)
	{
		exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
		//printf("Usage: %s [filename]\n", argv[0]);
		//exit(EXIT_FAILURE);
	}

	// OPEN INPUT FILE
  FILE *fin = fopen(argv[1], "r");
  if (!fin)
  {
		exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
    //perror("Cannot open file");
    //exit(EXIT_FAILURE);
  }

	// PARSE THE FILE
	parse(fin);

	symtable_print_labels();

	fclose(fin);
	return EXIT_SUCCESS;

}
