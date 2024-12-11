/****************************************
 * Hack Assembler - parser.c
 *
 * [NAME] PRESTON EVERSAUL
 * [TERM] FALL 2024
 ****************************************/

#include "parser.h"
#include "symtable.h"
#include "error.h"
#include "hack.h"
#include <string.h>
#include <ctype.h>


/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s)
{

  // REMOVE COMMENTS
  char *comment_start = strstr(s, "//");
  if (comment_start)
  {
      *comment_start = '\0';
  }

  // REMOVE TRAILING NEWLINE
  size_t len = strlen(s);
  if (len > 0 && s[len - 1] == '\n')
  {
      s[len - 1] = '\0';
  }

  // TRIM LEADING WHITESPACE
  char *start = s;
  while (isspace((unsigned char)*start))
  {
      start++;
  }

  // TRIM TRAILING WHITESPACE
  char *end = start + strlen(start) - 1;
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

bool is_Atype(const char *line)
{
  return line[0] == '@';
}

bool is_label(const char *line)
{
  size_t len = strlen(line);
  // CHECKS FOR STARTING ( AND AN ENDING )
  return len > 2 && line[0] == '(' && line[len - 1] == ')';
}

bool is_Ctype(const char *line)
{
  // IF NOT A TYPE OR LABEL THEN IT MUST BE C TYPE
  return !is_Atype(line) && !is_label(line);
}

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

void add_predefined_symbols()
{
  for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++)
  {
      symtable_insert(predefined_symbols[i].name, predefined_symbols[i].address);
  }
}

bool parse_A_instruction(const char *line, a_instruction *instr)
{
  // ALLOCATE MEMORY FOR THE STRING, INCLUDING THE NULL TERMINATOR
  char *s = (char *)malloc(strlen(line) + 1);
  strcpy(s, line + 1);

  // CONVERT STRING TO A LONG INTEGER
  char *s_end = NULL;
  long result = strtol(s, &s_end, 10);

  if (s == s_end)
  {
      // CASE: LABEL
      instr->is_addr = false;
      instr->label = strdup(s);
  } else if (*s_end != '\0')
  {
      // CASE: INVALID LABEL
      free(s);
      return false;
  } else
  {
      // CASE: NUMERIC ADDRESS
      instr->is_addr = true;
      instr->address = (int16_t)result;
  }

  free(s);
  return true;
}

void parse_C_instruction(char *line, c_instruction *instr)
{
  instr->jump = JMP_NULL;
  instr->dest = DEST_NULL;
  instr->comp = COMP_INVALID;
  instr->a = 0;

  // SPLIT THE LINE BY ';' TO SEPERATE comp/dest AND jump
  char *comp_dest_part = strtok(line, ";");
  char *jump_part = strtok(NULL, ";");

  if (jump_part != NULL)
  {
      instr->jump = str_to_jumpid(jump_part);
  }

  char *dest_part = strtok(comp_dest_part, "=");
  char *comp_part = strtok(NULL, "=");

  if (comp_part == NULL)
  {
      comp_part = dest_part;
      dest_part = NULL;
  }

  if (dest_part != NULL)
  {
      instr->dest = str_to_destid(dest_part);
  }

  if (comp_part != NULL)
  {
      int a_bit = 0;
      instr->comp = str_to_compid(comp_part, &a_bit);
      instr->a = (opcode)a_bit;
  }
}

opcode instruction_to_opcode(c_instruction instr)
{
  opcode op = 0;
  op |= (7 << 13);
  op |= (instr.a << 12);
  op |= (instr.comp << 6);
  op |= (instr.dest << 3);
  op |= instr.jump;
  return op;
}

int parse(FILE *file, instruction *instructions)
{
  // LOAD PREDEFINED SYMBOLS INTO THE SYMBOL TABLE
  add_predefined_symbols();

  // BUFFER FOR EACH LINE TO BE READ
  char line[MAX_LINE_LENGTH];
  char tmp_line[MAX_LINE_LENGTH];
  char label[MAX_LABEL_LENGTH + 1];
  unsigned int line_num = 0;
  unsigned int instr_num = 0;

  // INSTRUCTION VARIABLE
  instruction instr;

  // READ LINE FROM FILE
  while (fgets(line, sizeof(line), file)) {
      line_num++;
      strip(line);

      // CHECK FOR NON-EMPTY LINE AFTER STRIP
      if (line[0] != '\0') {
          if (is_label(line))
          {
              // EXTRACT LABEL
              extract_label(line, label);

              // CHECK IF LABEL STARTS WITH A LETTER
              if (!isalpha(label[0])) {
                  exit_program(EXIT_INVALID_LABEL, line_num, line);
              }

              // CHECK IF LABEL ALREADY EXISTS
              if (symtable_find(label) != NULL) {
                  exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line);
              }

              // INSERT LABEL INTO SYMBOL TABLE
              symtable_insert(label, instr_num);
              continue;
          }

          if (is_Atype(line))
          {
              // PARSE A-TYPE INSTRUCTION
              if (!parse_A_instruction(line, &instr.a_instr)) {
                  exit_program(EXIT_INVALID_A_INSTR, line_num, line);
              }
              instr.type = A_TYPE_INSTRUCTION;

              // PRINT A-TYPE INSTRUCTION
              if (instr.a_instr.is_addr)
              {
                printf("A: %d\n", instr.a_instr.address);
              } else
              {
                 printf("A: %s\n", instr.a_instr.label);
              }
          } else if (is_Ctype(line))
          {
            // HANDLE C-TYPE INSTRUCTION
            strcpy(tmp_line, line);
            parse_C_instruction(tmp_line, &instr.c_instr);

            // CHECK FOR INVALID dest, comp, AND jump FIELDS
            if (instr.c_instr.dest == DEST_INVALID) {
                exit_program(EXIT_INVALID_C_DEST, line_num, line);
            }
            if (instr.c_instr.comp == COMP_INVALID) {
                exit_program(EXIT_INVALID_C_COMP, line_num, line);
            }
            if (instr.c_instr.jump == JMP_INVALID) {
                exit_program(EXIT_INVALID_C_JUMP, line_num, line);
            }

            // SET THE INSTRUCTION TYPE TO C-TYPE
            instr.type = C_TYPE_INSTRUCTION;

            // PRINT C-TYPE INSTRUCTION
            printf("C: d=%d, c=%u, j=%d\n", instr.c_instr.dest, instr.c_instr.comp, instr.c_instr.jump);


        }

        // SAVE INSTRUCTION TO THE ARRAY AND INCREMENT instr_num
        instructions[instr_num++] = instr;

          // CHECK IF MAX NUMBER OF INSTRUCTIONS IS EXCEEDED
          if (instr_num > MAX_INSTRUCTIONS)
          {
              exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
          }
      }
  }
  // RETURN THE NUMBER OF INSTRUCTIONS PARSED
  return instr_num;
}

void assemble(const char *file_name, instruction *instructions, int num_instructions)
{
  // CREATE THE OUTPUT FILE NAME BY APPENDING ".hack" TO THE ORIGINAL FILE NAME
  char output_file_name[256];
  snprintf(output_file_name, sizeof(output_file_name), "%s.hack", file_name);

  // OPEN THE OUTPUT FILE FOR WRITING
  FILE *output_file = fopen(output_file_name, "w");
  if (!output_file)
  {
      // PRINT AN ERROR MESSAGE IF THE FILE CANNOT BE OPENED AND EXIT
      perror("Error opening output file");
      exit(EXIT_FAILURE);
  }

  // INITIALIZE THE NEXT AVAILABLE ADDRESS FOR NEW VARIABLES (STARTING AT 16)
  int next_available_address = 16;

  // LOOP THROUGH EACH INSTRUCTION IN THE ARRAY
  for (int i = 0; i < num_instructions; i++)
  {
      instruction *instr = &instructions[i]; // GET THE CURRENT INSTRUCTION
      char opcode[17] = {0}; // INITIALIZE THE OPCODE BUFFER (16 BITS + NULL TERMINATOR)

      // HANDLE A-TYPE INSTRUCTION
      if (instr->type == A_TYPE_INSTRUCTION)
      {
          int address;
          if (!instr->a_instr.is_addr)
          {
              // LOOKUP THE ADDRESS FOR THE LABEL IN THE SYMBOL TABLE
              address = symtable_lookup(instr->a_instr.label);
              if (address == -1)
              {
                  // IF LABEL IS NOT FOUND, INSERT IT AS A NEW VARIABLE WITH A NEW ADDRESS
                  address = next_available_address++;
                  symtable_insert(instr->a_instr.label, address);
              }
              // FREE THE LABEL MEMORY AFTER USE
              free(instr->a_instr.label);
          }
          else
          {
              // USE THE DIRECT ADDRESS IF IT IS PROVIDED
              address = instr->a_instr.address;
          }

          // CONVERT THE ADDRESS TO A 16-BIT BINARY STRING
          for (int j = 15; j >= 0; j++)
          {
              opcode[15 - j] = (address >> j) & 1 ? '1' : '0';
          }
      }
      // HANDLE C-TYPE INSTRUCTION
      else if (instr->type == C_TYPE_INSTRUCTION)
      {
          // CONVERT THE C-TYPE INSTRUCTION TO BINARY OPCODE
          int binary_opcode = instruction_to_opcode(instr->c_instr);
          for (int j = 15; j >= 0; j++)
          {
              opcode[15 - j] = (binary_opcode >> j) & 1 ? '1' : '0';
          }
      }

      // WRITE THE GENERATED OPCODE TO THE OUTPUT FILE
      fprintf(output_file, "%s\n", opcode);
  }

  // CLOSE THE OUTPUT FILE
  fclose(output_file);
}
