#ifndef LABRATORY_C_FINAL_PREPROCESSOR
#define LABRATORY_C_FINAL_PREPROCESSOR

#include "string_util.h"
#include "token.h"
#include "error_handler.h"
#include "lexer.h"

typedef struct Macro {
    Token identifier; /* the identifier of the macro */
    String expansion; /* the expansion data of the macro */

    int start; /* the starting index of the macro (the char starter of MACR) */
    int end; /* the ending index of the macro (the char ender of ENDMACR) */
} Macro;

typedef struct MacroList {
    Macro macro; /* the current macro */
    struct MacroList * next; /* the next macro */
} MacroList;

typedef struct Preprocessor {
    String string; /* the string of the source file after the preprocessor pass */
    MacroList * macroList; /* the macroList of the preprocessor */

    ErrorHandler errorHandler;  /* the error handler of the preprocessor */
    TokenList * tokens;   /* the token list refrence from the token */
} Preprocessor;


/**
 * Initializes the preprocesso
 *
 * @param preprocessor the preprocessor.
 * @param lexer the lexer.
*/
void preprocessor_init(Preprocessor * preprocessor, Lexer lexer);

/**
 * Generate a macro from the current preprocessor looked at token.
 *
 * @param preprocessor the preprocessor.
 * @param source the srouce file as string.
 * @note An MACR token must be pressent at the current context.
*/
void preprocessor_generate_macro(Preprocessor * preprocessor, String source);

/**
 * Generate a macro list from lexer tokens and the new macroList and append the out source to the preprocessor.
 *
 * @param preprocessor the preprocessor.
 * @param lexer the lexer.
 * @return a pointer to a memory allocated MacroList.
*/
void generate_macro_list(Preprocessor * preprocessor, Lexer lexer);

/**
 * Add a macro to the end of the preprocessor macro list
 *
 * @param preprocessor the preprocessor.
 * @param macro the macro to be added.
*/
void add_macro_to_preprocessor_macro_list(Preprocessor * preprocessor, Macro macro);

#endif /* LABRATORY_C_FINAL_PREPROCESSOR */
