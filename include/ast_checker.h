#ifndef LABRATORY_C_MAMAN22_AST_CHECKER
#define LABRATORY_C_MAMAN22_AST_CHECKER

#include "parser.h"
#include "lexer.h"

typedef enum IdentifierHashCellKind {
    LabalCellKind,
    ExternalCellKind
}IdentifierHashCellKind;

typedef struct IdentifierHashCell {
    union data {
        LabalNode * labal;
        ExternalNode * external;
    }; /* the data of the cell */

    IdentifierHashCellKind kind; /* the kind of the cell */
    bool hasEntry; /* true if there is an .entry for this identifier else false */
}IdentifierHashCell;

typedef struct AstChecker {
    IdentifierHashCell * hash; /* a pointer to the array of the identifiers cells, used for naming checking */
    unsigned int size; /* the amount of cells in the array (hash) */

    ErrorHandler errorHandler; /* the error handler of the translation unit */
}AstChecker;

/**
 * The hash function for mapping a string to a number.
 *
 * @param str the string.
 * @return the hash of the string.
 * @note from https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29.
*/
unsigned long hash(String str);

/**
 * Initialize the AST checker.
 *
 * @param astChecker the AST checker.
 * @param lexer the lexer.
*/
void ast_checker_init(AstChecker * astChecker, Lexer lexer);

#endif /* LABRATORY_C_MAMAN22_AST_CHECKER */