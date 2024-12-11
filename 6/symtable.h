#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SYMBOL_TABLE_SIZE 1000

typedef int16_t hack_addr;

typedef struct Symbol
{
  char *name;
  hack_addr addr;
} Symbol;

int hash(char *str);
Symbol *symtable_find(char *key);
void symtable_insert(const char *key, hack_addr addr);
void symtable_display_table();
void symtable_print_labels();
int symtable_lookup(const char *symbol_name);

#endif
