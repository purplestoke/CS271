/****************************************
 * C-ploration 3 for CS 271
 *
 * [NAME] Preston Eversaul
 * [TERM] FALL 2024
 *
 ****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 200
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

  // CREATE AND NAME OUTPUT FILE
  char out_file[256];
  strcpy(out_file, argv[1]);
  strcat(out_file, ".echo");
  FILE *fout = fopen(out_file, "w+");
  if (!fout)
  {
    perror("Cannot create output file");
    fclose(fin);
    exit(EXIT_FAILURE);
  }

  // WRITE LINES
  char line[MAX_LINE_LENGTH] = {0};
  unsigned int line_num = 0;
  while (fgets(line, sizeof(line), fin))
  {
    line_num++;
    printf("[%04d] %s", line_num, line);
    fprintf(fout, "%s", line);
  }

  // CLOSE THE FILES
  fclose(fin);
  fclose(fout);

	return 0;
}
