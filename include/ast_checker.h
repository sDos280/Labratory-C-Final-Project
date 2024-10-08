#ifndef LABRATORY_C_MAMAN22_AST_CHECKER
#define LABRATORY_C_MAMAN22_AST_CHECKER

#include "parser.h"
#include "lexer.h"

typedef enum AddressingMode {
    AbsoluteAddressing = 1,          /* 0001 - # then number */
    DirectAddressing = 2,            /* 0010 - just a labal */
    IndirectRegisterAddressing = 4,  /* 0100 - derefrencing a register */
    DirectRegisterAddressing = 8     /* 1000 - just a register */
}AddressingMode;

typedef enum IdentifierHashCellKind {
    LabalCellKind,
    ExternalCellKind
}IdentifierHashCellKind;

typedef struct IdentifierHashCell {
    String * key; /* the key of the labal*/

    union {
        LabalNode * labal;
        ExternalNode * external;
    }value; /* the data of the cell */

    IdentifierHashCellKind kind; /* the kind of the cell */
    bool wasEntryAdded; /* true if an entry for the labal if the same identfier as the entry was added to the .ent file */
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
 * @param translationUnit the translation unit.
 * @param lexer the lexer.
*/
void ast_checker_init(AstChecker * astChecker, TranslationUnit * translationUnit, Lexer lexer);

/**
 * Free the (memory of) AST checker.
 *
 * @param astChecker the AST checker.
*/
void ast_checker_free(AstChecker * astChecker);

/**
 * Retrive the cell that has the passed key.
 *
 * @param astChecker the AST checker.
 * @param key the key as string.
 * @return the pointer to the cell (if no found/allocated return null)
*/
IdentifierHashCell * ast_checker_get_hash_cell_by_string(AstChecker * astChecker, String key);

/**
 * Set the cell that has the passed key with the data of the passed cell.
 *
 * @param astChecker the AST checker.
 * @param cell the cell.
 * @return true for success else false.
*/
bool ast_checker_set_hash_cell_by_string(AstChecker * astChecker, IdentifierHashCell cell);

/**
 * Check if the passed data node is currect (the numbers aren't too low/high)
 *
 * @param astChecker the AST checker.
 * @param node the data node.
*/
void ast_checker_check_data_guidance_sentence(AstChecker * astChecker, DataNode node);

/**
 * Check if the passed instruction node is currect (the source/destination is allowed)
 *
 * @param astChecker the AST checker.
 * @param node the instruction node.
*/
void ast_checker_check_instruction_sentence(AstChecker * astChecker, InstructionNode node);

/**
 * Check if the passed labal node is currect (the instructions/guidance sentences don't have a problem)
 *
 * @param astChecker the AST checker.
 * @param node the labal node.
*/
void ast_checker_check_labal(AstChecker * astChecker, LabalNode node);

/**
 * Check for duplicate identifiers
 *
 * @param astChecker the AST checker.
 * @param translationUnit the translation unit.
*/
void ast_checker_check_duplicate_identifiers(AstChecker * astChecker, TranslationUnit * translationUnit);

/**
 * Check all of the translation unit.
 *
 * @param astChecker the AST checker.
 * @param translationUnit the translation unit.
*/
void ast_checker_check_translation_unit(AstChecker * astChecker, TranslationUnit * translationUnit);

#endif /* LABRATORY_C_MAMAN22_AST_CHECKER */