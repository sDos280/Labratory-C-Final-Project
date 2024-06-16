#ifndef LABRATORY_C_FINAL_PREPROCESSOR
#define LABRATORY_C_FINAL_PREPROCESSOR

#include "string_util.h"
#include "token.h"
#include "lexer.h"

typedef struct Macro {
    Token identifier; /* the identifier of the macro */
    String expansion; /* the expansion of the macro */

    int start; /* the starting index of the macro (the char starter of MACR) */
    int end; /* the ending index of the macro (the char ender of ENDMACR) */
} Macro;

typedef struct MacroList {
    Macro macro; /* the current macro */
    struct MacroList * next; /* the next macro */
} MacroList;


/**
 * Add macro to the end of a macro list
 *
 * @param macroList a pointer to a (MacroList *) macroList .
 * @param macro the macro to be added.
*/
void add_macro_to_macro_list(MacroList ** macroList, Macro macro);

/**
 * Generate a macro list from lexer tokens.
 *
 * @param lexer the lexer.
 * @return a pointer to a memory allocated MacroList.
*/
MacroList * generate_macro_list(Lexer lexer);



#endif /* LABRATORY_C_FINAL_PREPROCESSOR */
