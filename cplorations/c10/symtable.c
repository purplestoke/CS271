#include "symtable.h"

Symbol *hashArray[SYMBOL_TABLE_SIZE] = {NULL};

// HASH FUNCTION
int hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % SYMBOL_TABLE_SIZE;
}

// INSERT FUNCTION
void symtable_insert(const char *key, hack_addr addr) {
    Symbol *item = (Symbol *)malloc(sizeof(Symbol));
    item->name = strdup(key); // Use strdup to copy the string
    item->addr = addr;

    int index = hash((char *)key);
    while (hashArray[index] != NULL && hashArray[index]->name != NULL)
    {
        index = (index + 1) % SYMBOL_TABLE_SIZE;
    }
    hashArray[index] = item;
}

// FIND FUNCTION
Symbol *symtable_find(char *key) {
    int index = hash(key);
    while (hashArray[index] != NULL)
    {
        if (strcmp(hashArray[index]->name, key) == 0)
        {
            return hashArray[index];
        }
        index = (index + 1) % SYMBOL_TABLE_SIZE;
    }
    return NULL;
}

// DISPLAY FUNCTION
void symtable_display_table() {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++)
    {
        if (hashArray[i] != NULL)
        {
            printf("Index %d: {%s, %d}\n", i, hashArray[i]->name, hashArray[i]->addr);
        }
    }
}

// PRINT LABELS FUNCTION
void symtable_print_labels() {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++)
    {
        if (hashArray[i] != NULL)
        {
            printf("{%s,%d}\n", hashArray[i]->name, hashArray[i]->addr);
        }
    }
}
