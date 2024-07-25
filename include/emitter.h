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
 * Update the size and position of the labals in the translation unit
 *
 * @param emitter the emitter.
 * @param translationUnit the translation unit.
*/
void emmiter_update_labals_size_and_position(Emitter * emitter, TranslationUnit * translationUnit);

#endif /*LABRATORY_C_FINAL_PROJECT_EMITTER*/
