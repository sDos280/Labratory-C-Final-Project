#ifndef LABRATORY_C_MAMAN22_AST_CHECKER
#define LABRATORY_C_MAMAN22_AST_CHECKER

#include "parser.h"

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
    IdentifierHashCell * hash; /* a pointer to the array of the identifiers cells */
    unsigned int size; /* the amount of cells in the array (hash) */
}AstChecker;

/**
 * The hash function for mapping a string to a number.
 *
 * @param str the string.
 * @return the hash of the string.
 * @note from https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29.
*/
unsigned long hash(String str);

#endif /* LABRATORY_C_MAMAN22_AST_CHECKER */