/* Defines the symbol table we use to parse our language
 *
 */

#include "lang_definitions.h"

/* A symbol, which can be a label on the text segment or in the
 * data segment (representing a "variable")
 */
struct symbol
{
    char    label[MAXIMUM_LABEL_SIZE]; /* label name */
    int16_t address;            /* current lable address on the
                                 * memory*/
};

typedef struct symbol_table
{
    struct symbol symbols[65536]; /* assuming one symbol per slot */

} symbol_table;


