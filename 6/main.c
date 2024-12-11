/****************************************
 * Hack Assembler - main.c
 *
 * [NAME] PRESTON EVERSAUL
 * [TERM] FALL 2024
 ****************************************/

#include "parser.h"
#include "symtable.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_INSTRUCTION_COUNT 32000

int main(int argc, const char *argv[])
{
  // CHECKING IF NUMBER OF ARGS IS CORRECT
  if (argc != 2)
	{
      exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
  }

  // OPEN INPUT FILE
  FILE *fin = fopen(argv[1], "r");
  if (!fin)
	{
      exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
  }

  // ALLOCATE MEMORY FOR INSTRUCTIONS
  instruction *instructions = malloc(MAX_INSTRUCTION_COUNT * sizeof(instruction));
  if (!instructions)
	{
      perror("Failed to allocate memory for instructions");
      fclose(fin);
      return EXIT_FAILURE;
  }

  // PARSE THE FILE AND GET THE NUMBER OF INSTRUCTIONS
  int num_instructions = parse(fin, instructions);
  printf("Number of instructions parsed: %d\n", num_instructions);

  // PRINT SYMBOL TABLE
  symtable_print_labels();

  // CALL THE ASSEMBLE FUNCTION
  assemble(argv[1], instructions, num_instructions);

  // CLEANUP
  fclose(fin);
  free(instructions);
  return EXIT_SUCCESS;
}
