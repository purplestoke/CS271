/****************************************
 * C-ploration 4 for CS 271
 *
 * [NAME] PRESTON EVERSAUL
 * [TERM] FALL 2024
 *
 ****************************************/
#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "hack.h"

#define MAX_HACK_ADDRESS INT16_MAX
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS
#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH (MAX_LINE_LENGTH - 2)

typedef int16_t hack_addr;
typedef int16_t opcode;

enum instruction_type {
  INVALID_INSTRUCTION = -1,
  A_TYPE_INSTRUCTION,
  C_TYPE_INSTRUCTION,
  L_TYPE_INSTRUCTION
};

/** function prototypes **/
char *strip(char *s);

bool is_Atype(const char *);
bool is_label(const char *);
bool is_Ctype(const char *);

char *extract_label(const char *line, char *label);

// STRUCT FOR C-TYPE INSTRUCTIONS
typedef struct c_instruction
{
  opcode a;
  unsigned comp:7;
  opcode dest:4;
  opcode jump:4;
} c_instruction;

// STRUCT FOR A-TYPE INSTRUCTIONS
typedef struct a_instruction
{
  union {
    hack_addr address;
    char *label;
  };
  bool is_addr;
} a_instruction;

typedef struct instruction
{
  enum instruction_type type;
  union {
    a_instruction a_instr;
    c_instruction c_instr;
  };
  char label[MAX_LABEL_LENGTH + 1];
} instruction;

void add_predefined_symbols();
bool parse_A_instruction(const char *line, a_instruction *instr);
void parse_C_instruction(char *line, c_instruction *instr);
int parse(FILE * fin, instruction *instructions);
opcode instruction_to_opcode(c_instruction instr);
void assemble(const char *file_name, instruction *instructions, int num_instructions);

#endif
