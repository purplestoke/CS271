/****************************************
 * C-ploration 4 for CS 271
 *
 * [NAME] PRESTON EVERSAUL
 * [TERM] FALL 2024
 *
 ****************************************/
#include "parser.h"

int main(int argc, const char *argv[])
{
	// CHECKING IF NUMBER OF ARGS IS CORRECT
	if (argc != 2)
	{
		printf("Usage: %s [filename]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// OPEN INPUT FILE
  FILE *fin = fopen(argv[1], "r");
  if (!fin)
  {
    perror("Cannot open file");
    exit(EXIT_FAILURE);
  }

	parse(fin);

	fclose(fin);
	return EXIT_SUCCESS;

}
