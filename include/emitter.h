#ifndef LABRATORY_C_FINAL_PROJECT_EMITTER
#define LABRATORY_C_FINAL_PROJECT_EMITTER

#include "lexer.h"
#include "ast_checker.h"
#include "node.h"

typedef struct __attribute__((packed)) InstrucitonMemory {
    /* the Instruction memory layout as in the object file*/
    unsigned int ARS: 3; /* ARS field */
    unsigned int dst: 4; /* destination adressing mode */
    unsigned int src: 4; /* source adressing mode */
    unsigned int code : 4; /* 4 bit for the code */
    /* so in total we have 3 + 4 + 4 + 4 = 15 bits */
} InstrucitonMemory;

typedef struct __attribute__((packed)) InstrucitonOperandMemory {
    /* the Instruction memory layout as in the object file*/
    unsigned int ARS: 3; /* ARS field */
    union {
        unsigned int full: 12; /* for a case of number\labal\external */
        
        /* for adressing mode of one registr or more */
        unsigned int rdst: 4; /* the register used in destination */
        unsigned int rsrc: 4; /* the register used in source */
    }other;
    /* so in total we have 3 + max(12, 4 + 4) = 3 + 12 = 15 bits */
} InstrucitonOperandMemory;


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
*/
void emitter_generate_object_and_external_files_string(Emitter * emitter, AstChecker * astChecker, TranslationUnit * translationUnit);

#endif /*LABRATORY_C_FINAL_PROJECT_EMITTER*/
