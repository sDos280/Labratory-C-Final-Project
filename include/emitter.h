#ifndef LABRATORY_C_FINAL_PROJECT_EMITTER
#define LABRATORY_C_FINAL_PROJECT_EMITTER

#include "node.h"

typedef struct Emitter
{
    String entryFile; /* the .ent file as string */
    String externalFile; /* the .ext file as string */
    String objectFile; /* the .ob file as string */

    ErrorHandler errorHandler; /* the error handler of the translation unit */
}Emitter;

/**
 * Initialize the emitter.
 *
 * @param emmiter the emmiter.
 * @param lexer the lexer.
*/
void emitter_init(Emitter * emitter, Lexer lexer);

/**
 * Free the (memory of) emitter.
 *
 * @param emmiter the emmiter.
*/
void emitter_free(Emitter * emitter);

/**
 * Update the size and position of the labals in the translation unit.
 *
 * @param emitter the emitter.
 * @param translationUnit the translation unit.
*/
void emitter_update_labals_size_and_position(Emitter * emitter, TranslationUnit * translationUnit);

/**
 * Generate the entry file string.
 *
 * @param emitter the emitter.
 * @param astChecker the ast checker.
 * @param translationUnit the translation unit.
*/
void emitter_generate_entry_file_string(Emitter * emitter, AstChecker * astChecker, TranslationUnit * translationUnit);

#endif /*LABRATORY_C_FINAL_PROJECT_EMITTER*/
