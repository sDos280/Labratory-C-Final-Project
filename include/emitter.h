#ifndef LABRATORY_C_FINAL_PROJECT_EMITTER
#define LABRATORY_C_FINAL_PROJECT_EMITTER

#include "lexer.h"
#include "ast_checker.h"
#include "node.h"

typedef struct InstrucitionMemory {
    /* the Instruction memory layout as in the object file*/
    unsigned int ARE: 3; /* ARE field */
    unsigned int dst: 4; /* destination adressing mode */
    unsigned int src: 4; /* source adressing mode */
    unsigned int code : 4; /* 4 bit for the code */
    /* so in total we have 3 + 4 + 4 + 4 = 15 bits */
} InstrucitionMemory;

typedef struct InstrucitionOperandMemory {
    /* the Instruction memory layout as in the object file*/
    unsigned int ARE: 3; /* ARE field */
    union {
        unsigned int full: 12; /* for a case of number\labal\external */
        
        /* for adressing mode of one registr or more */
        struct {
        unsigned int rdst: 3; /* the register used in destination */
        unsigned int rsrc: 3; /* the register used in source */
        }reg;
    }other;
    /* so in total we have 3 + max(12, 3 + 3) = 3 + 12 = 15 bits */
} InstrucitionOperandMemory;


typedef enum InstructionCode {
    MOV_CODE,
    CMP_CODE,
    ADD_CODE,
    SUB_CODE,
    LEA_CODE,
    CLR_CODE,
    NOT_CODE,
    INC_CODE,
    DEC_CODE,
    JMP_CODE,
    BNE_CODE,
    RED_CODE,
    PRN_CODE,
    JSR_CODE,
    RTS_CODE,
    STOP_CODE
}InstructionCode;

typedef struct Emitter {
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


/**
 * Generate the object and external files string.
 *
 * @param emitter the emitter.
 * @param astChecker the ast checker.
 * @param translationUnit the translation unit.
 * @param instrucitonLines the pointer to the instrucitonLines counter.
 * @param guidanceLines the pointer to the guidanceLines counter.
*/
void emitter_generate_object_and_external_files_string(Emitter * emitter, AstChecker * astChecker, TranslationUnit * translationUnit, int * instrucitonLines, int * guidanceLines);

/**
 * Generate and emit all the emitter files.
 *
 * @param emitter the emitter.
 * @param astChecker the ast checker.
 * @param translationUnit the translation unit.
 * @param filePath the file path (without extention).
*/
void emitter_emit(Emitter * emitter, AstChecker * astChecker, TranslationUnit * translationUnit, char * filePath);

#endif /*LABRATORY_C_FINAL_PROJECT_EMITTER*/
