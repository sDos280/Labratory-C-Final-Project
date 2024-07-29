#include "../include/emitter.h"
#include "../include/ast_checker.h"
#include "../include/string_util.h"

#define STARTING_POSITION 100
#define MAX_POSITION 9999

#define InstructionTokenKindToInstructionCode(kind) (InstructionCode)(kind - MOV)
/* 0xFFF is a mast for 12 bit */
#define ConvertIntTo2Complement(value) ((value >= 0)? (value & 0xFFF) : (((~(-value) & 0xFFF) + 1) & 0xFFF))

/**
 * Get the addressing mod of an operand.
 *
 * @param operand_token the operand token.
 * @param operand_token true if the operand is derefrenced else false.
 * @return the addressing mode.
 * @note almost the same function as in ast_cheker
*/
static AddressingMode get_addressing_mode_of_operand(Token * operand_token, bool isDerefrenced){
    if (operand_token->kind == NUMBER) {
        return AbsoluteAddressing;
    } else if (operand_token->kind == IDENTIFIER) {
        return DirectAddressing;
    } else if (operand_token->kind == REGISTER) {
        if (isDerefrenced == true) return IndirectRegisterAddressing;
        
        return DirectRegisterAddressing;
    }

    return AbsoluteAddressing;
}

/**
 * Calculate the size of memory a labal would occupied 
 *
 * @param labal the labal.
 * @return the size of the labal
*/
static unsigned int calc_labal_size(LabalNode labal){
    InstructionNodeList * instructionNodeList = labal.instructionNodeList;
    GuidanceNodeList * guidanceNodeList = labal.guidanceNodeList;
    TokenRefrenceList * numbers = NULL;
    AddressingMode first = AbsoluteAddressing;
    AddressingMode second = AbsoluteAddressing;
    int out = 0;

    while (instructionNodeList != NULL){
        out++; /* +1 for the memory instruction itself */
        
        first = AbsoluteAddressing;
        second = AbsoluteAddressing;

        if (instructionNodeList->node.firstOperand != NULL) 
            first = get_addressing_mode_of_operand(instructionNodeList->node.firstOperand, 
                                           instructionNodeList->node.isFirstOperandDerefrenced);
        if (instructionNodeList->node.secondOperand != NULL) 
            second = get_addressing_mode_of_operand(instructionNodeList->node.secondOperand, 
                                           instructionNodeList->node.isSecondOperandDerefrenced);

        if ((first == IndirectRegisterAddressing || first == DirectRegisterAddressing) &&
            (second == IndirectRegisterAddressing || second == DirectRegisterAddressing))
            out++; /* only 2 registers have the property to fit in one word together */
        else {
            if (instructionNodeList->node.firstOperand != NULL) 
                out++; /* add +1 for the first operand */
            if (instructionNodeList->node.secondOperand != NULL) 
                out++; /* add +1 for the second operand */
        }

        instructionNodeList = instructionNodeList->next;
    }

    while (guidanceNodeList != NULL){
        if (guidanceNodeList->kind == StringNodeKind)
            out += string_length(guidanceNodeList->node.stringNode.token->string) + 1 - 2; /* +1 for the \0 char and -2 for the 2 \" "*/

        if (guidanceNodeList->kind == DataNodeKind){
            numbers = guidanceNodeList->node.dataNode.numbers;
            while (numbers != NULL){
                out++; /* each number have the size of 1*/
                numbers = numbers->next;
            }
        }
        
        guidanceNodeList = guidanceNodeList->next;
    }

    return out;
}

void emitter_init(Emitter * emitter, Lexer lexer){
    emitter->entryFile = string_init();
    emitter->externalFile = string_init();
    emitter->objectFile = string_init();

    error_handler_init(&emitter->errorHandler, lexer.string, lexer.filePath);
}

void emitter_free(Emitter * emitter){
    string_free(emitter->entryFile);
    string_free(emitter->externalFile);
    string_free(emitter->objectFile);

    error_handler_free_error_list(&emitter->errorHandler);
}

void emitter_update_labals_size_and_position(Emitter * emitter, TranslationUnit * translationUnit){
    LabalNodeList * instructionLabalList = translationUnit->instructionLabalList;
    LabalNodeList * guidanceLabalList = translationUnit->guidanceLabalList;
    unsigned int position = STARTING_POSITION;
    TokenError error;

    while (instructionLabalList != NULL){
        /* update the labal poistion and size */
        instructionLabalList->labal.size = calc_labal_size(instructionLabalList->labal);
        instructionLabalList->labal.position = position;

        position += instructionLabalList->labal.size;
        if (position > MAX_POSITION) { /* 9999*/
            error.message = string_init_with_data("Maxed out position size, got over 9999");

            if (instructionLabalList->labal.labal != NULL)
                error.token = *instructionLabalList->labal.labal;
            else 
                error.token = *instructionLabalList->labal.instructionNodeList->node.operation;

            error_handler_push_token_error(&emitter->errorHandler, EmitterErrorKind, error);

            return;
        } 

        instructionLabalList = instructionLabalList->next;
    }

    while (guidanceLabalList != NULL){
        /* update the labal poistion and size */
        guidanceLabalList->labal.size = calc_labal_size(guidanceLabalList->labal);
        guidanceLabalList->labal.position = position;

        position += guidanceLabalList->labal.size;
        if (position > MAX_POSITION) { /* 9999*/
            error.message = string_init_with_data("Maxed out position size, got over 9999");

            if (guidanceLabalList->labal.labal != NULL)
                error.token = *guidanceLabalList->labal.labal;
            else 
                if (guidanceLabalList->labal.guidanceNodeList->kind == DataNodeKind)
                    error.token = *guidanceLabalList->labal.guidanceNodeList->node.dataNode.numbers->token;
                else 
                    error.token = *guidanceLabalList->labal.guidanceNodeList->node.stringNode.token;

            error_handler_push_token_error(&emitter->errorHandler, EmitterErrorKind, error);

            return;
        } 

        guidanceLabalList = guidanceLabalList->next;
    }
}

void emitter_generate_entry_file_string(Emitter * emitter, AstChecker * astChecker, TranslationUnit * translationUnit){
    EntryNodeList * entryNodeList = translationUnit->entryNodeList;
    IdentifierHashCell * temp = NULL;
    char * tempAtoiS = NULL; /* temp char pointer for storing the value of the position in a string format*/

    while (entryNodeList != NULL){
        temp = ast_checker_get_hash_cell_by_string(astChecker, entryNodeList->node.token->string);
        
        if (temp != NULL){
            if (temp->wasEntryAdded == false){
                tempAtoiS = calloc(10, sizeof(char));
                string_add_string(&emitter->entryFile, entryNodeList->node.token->string); /* add the name of entry */
                sprintf(tempAtoiS, " %d\n", temp->value.labal->position);
                string_add_char_pointer(&emitter->entryFile, tempAtoiS);
                free(tempAtoiS);
            }

            temp->wasEntryAdded = true;
        }

        entryNodeList = entryNodeList->next;
    }
}

void emitter_generate_object_and_external_files_string(Emitter * emitter, AstChecker * astChecker, TranslationUnit * translationUnit){
    LabalNodeList * instructionLabalList = translationUnit->instructionLabalList;
    LabalNodeList * guidanceLabalList = translationUnit->guidanceLabalList;
    InstructionNodeList * instructionNodeList = instructionLabalList->labal.instructionNodeList;
    GuidanceNodeList * guidanceNodeList = guidanceLabalList->labal.guidanceNodeList;
    /* firsly, generate code image */
    InstrucitonMemory instrucitonMemory;
    InstrucitonOperandMemory instrucitonOperandMemory;
    IdentifierHashCell * tempCellP = NULL;
    AddressingMode first = AbsoluteAddressing;
    AddressingMode second = AbsoluteAddressing;
    int position = 100;
    int temp;
    instrucitonMemory.ARS = 0;
    instrucitonMemory.dst = 0;
    instrucitonMemory.src = 0;
    instrucitonMemory.code = 0;

    while (instructionLabalList != NULL){
        while (instructionNodeList != NULL){
            /* 0 operand instruction case */
            if (instructionNodeList->node.firstOperand == NULL && instructionNodeList->node.secondOperand == NULL){
                /* update instruction memory */
                instrucitonMemory.ARS = 4; /* 4 = 0b100 */
                instrucitonMemory.dst = 0;
                instrucitonMemory.src = 0;
                instrucitonMemory.code = InstructionTokenKindToInstructionCode(instructionNodeList->node.operation->kind);

                /* update position counter */
                position++;
            } 

            instructionNodeList = instructionNodeList->next;
        }
        
        instructionLabalList = instructionLabalList->next;
    }
}