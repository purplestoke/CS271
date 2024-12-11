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
#include "error.h"
#include <ctype.h>
#include "hack.h"


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


   void add_predefined_symbols()
   {
     for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++)
     {
         symtable_insert(predefined_symbols[i].name, predefined_symbols[i].address);
     }
 }

 bool parse_A_instruction(const char *line, a_instruction *instr) {
     // ALLOCATE MEMORY FOR THE STRING
     char *s = (char *)malloc(strlen(line));
     strcpy(s, line + 1);

     // CONVERT STRING TO LONG
     char *s_end = NULL;
     long result = strtol(s, &s_end, 10);

     // CASE 1: LABEL (STRING)
     if (s == s_end) {
         instr->is_addr = false;
         instr->label = (char *)malloc(strlen(s) + 1);
         strcpy(instr->label, s);
     }
     // CASE 2: INVALID LABEL
     else if (*s_end != '\0') {
         free(s);
         return false;
     }
     // CASE 3: NUMBER (ADDRESS)
     else {
         instr->is_addr = true;
         instr->address = (int16_t)result;
     }

     free(s);
     return true;
 }

 // FUNCTION TO PARSE C-TYPE INSTRUCTION
 void parse_C_instruction(char *line, c_instruction *instr) {
     // INITIALIZE FIELDS TO DEFAULT VALUES
     instr->jump = JMP_NULL;
     instr->dest = DEST_NULL;
     instr->comp = COMP_INVALID;
     instr->a = 0;

     // SPLIT THE LINE BY ';' TO SEPARATE comp/dest AND jump
     char *comp_dest_part = strtok(line, ";");
     char *jump_part = strtok(NULL, ";");

     // HANDLE NULL CASE FOR jump_part
     if (jump_part != NULL) {
         instr->jump = str_to_jumpid(jump_part);
     }

     // SPLIT THE comp_dest_part BY '=' TO SEPARATE dest AND comp
     char *dest_part = strtok(comp_dest_part, "=");
     char *comp_part = strtok(NULL, "=");

     // IF comp_part IS NULL, dest_part CONTAINS THE COMPUTATION
     if (comp_part == NULL) {
         comp_part = dest_part;
         dest_part = NULL;
     }

     // SET THE dest FIELD USING str_to_destid
     if (dest_part != NULL) {
         instr->dest = str_to_destid(dest_part);
     }

     // SET THE comp FIELD AND THE a BIT USING str_to_compid
     if (comp_part != NULL) {
         int a_bit = 0;
         instr->comp = str_to_compid(comp_part, &a_bit);
         instr->a = (opcode)a_bit;
     }
 }


/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments.
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
 int parse(FILE *file, instruction *instructions) {
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
