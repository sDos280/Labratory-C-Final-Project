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

static void update_instruction_memory_stuff(Emitter * emitter, 
                                            AstChecker * astChecker, 
                                            InstructionNode node, 
                                            InstrucitionMemory * instrucitionMemory, 
                                            InstrucitionOperandMemory * instrucitionFirstOperandMemory,
                                            InstrucitionOperandMemory * instrucitionSecondOperandMemory,
                                            int * position) {
    AddressingMode first = AbsoluteAddressing;
    AddressingMode second = AbsoluteAddressing;
    int temp;
    IdentifierHashCell * tempCellP = NULL;

    /* inisialize memory of struct */
    instrucitionMemory->ARE = 0;
    instrucitionMemory->dst = 0;
    instrucitionMemory->src = 0;
    instrucitionMemory->code = 0; 
    instrucitionFirstOperandMemory->ARE = 0;
    instrucitionFirstOperandMemory->other.full = 0;
    instrucitionSecondOperandMemory->ARE = 0;
    instrucitionSecondOperandMemory->other.full = 0;
    
    /* 0 operand instruction case */
    if (node.firstOperand == NULL && node.secondOperand == NULL){
        /* update instruction memory */
        instrucitionMemory->ARE = 4; /* 4 = 0b100 */
        instrucitionMemory->dst = 0;
        instrucitionMemory->src = 0;
        instrucitionMemory->code = InstructionTokenKindToInstructionCode(node.operation->kind);

        /* NOTE: should add the output here */

        /* update position counter */
        *position++;
    }

    /* 1 operand instruction case */
    else if (node.firstOperand != NULL && node.secondOperand == NULL) {
        first = get_addressing_mode_of_operand(node.firstOperand, node.isFirstOperandDerefrenced);

        instrucitionMemory->ARE = 4; /*4 = 0b100 */
        instrucitionMemory->dst = first;
        instrucitionMemory->src = 0;
        instrucitionMemory->code = InstructionTokenKindToInstructionCode(node.operation->kind);

        /* NOTE: should add the output here */

        /* update position counter (once for the instruction itself)*/
        *position++;

        switch (first) {
            case AbsoluteAddressing:
                instrucitionFirstOperandMemory->ARE = 4; /*4 = 0b100 */
                temp = atoi(node.firstOperand->string.data);
                instrucitionFirstOperandMemory->other.full = ConvertIntTo2Complement(temp);
                break;
                    
            case DirectAddressing:
                tempCellP = ast_checker_get_hash_cell_by_string(astChecker, node.firstOperand->string);
                if (tempCellP != NULL){
                    if (tempCellP->kind == LabalCellKind) {
                        instrucitionFirstOperandMemory->ARE = 2; /* 4 = 0b010 */
                        instrucitionFirstOperandMemory->other.full = ConvertIntTo2Complement(tempCellP->value.labal->position);
                    }
                    else /* if (tempCellP->kind == ExternalCellKind) */ {
                        instrucitionFirstOperandMemory->ARE = 1; /* 1 = 0b001 */
                        instrucitionFirstOperandMemory->other.full = 0;
                        /* NOTE: should add the output here */
                    }
                }
                    
                break;
                
            case IndirectRegisterAddressing:
            case DirectRegisterAddressing:
                instrucitionFirstOperandMemory->ARE = 4; /*4 = 0b100 */
                temp = atoi(node.firstOperand->string.data[1]); /* on data[0] would be an 'r' so we move one for the number */
                instrucitionFirstOperandMemory->other.rdst = ConvertIntTo2Complement(temp);
                break;
                
            default:
                break;
        }

        /* NOTE: should add the output here */

        /* update position counter secound time */
        *position++;
    }

    /* 0 operand instruction case */
    else if (node.firstOperand != NULL && node.secondOperand != NULL) {
        first = get_addressing_mode_of_operand(node.firstOperand, node.isFirstOperandDerefrenced);
        second = get_addressing_mode_of_operand(node.secondOperand, node.isSecondOperandDerefrenced);

        instrucitionMemory->ARE = 4; /*4 = 0b100 */
        instrucitionMemory->dst = first;
        instrucitionMemory->src = second;
        instrucitionMemory->code = InstructionTokenKindToInstructionCode(node.operation->kind);

        /* NOTE: should add the output here */

        /* update position counter (once for the instruction itself)*/
        *position++;

        switch (first) {
            case AbsoluteAddressing:
                instrucitionFirstOperandMemory->ARE = 4; /*4 = 0b100 */
                temp = atoi(node.firstOperand->string.data);
                instrucitionFirstOperandMemory->other.full = ConvertIntTo2Complement(temp);
                break;
                    
            case DirectAddressing:
                tempCellP = ast_checker_get_hash_cell_by_string(astChecker, node.firstOperand->string);
                if (tempCellP != NULL){
                    if (tempCellP->kind == LabalCellKind) {
                        instrucitionFirstOperandMemory->ARE = 2; /* 4 = 0b010 */
                        instrucitionFirstOperandMemory->other.full = ConvertIntTo2Complement(tempCellP->value.labal->position);
                    }
                    else /* if (tempCellP->kind == ExternalCellKind) */ {
                        instrucitionFirstOperandMemory->ARE = 1; /* 1 = 0b001 */
                        instrucitionFirstOperandMemory->other.full = 0;
                        /* NOTE: should add the output here */
                    }
                }
                    
                break;
                
            case IndirectRegisterAddressing:
            case DirectRegisterAddressing:
                instrucitionFirstOperandMemory->ARE = 4; /*4 = 0b100 */
                temp = atoi(node.firstOperand->string.data[1]); /* on data[0] would be an 'r' so we move one for the number */
                instrucitionFirstOperandMemory->other.rsrc = ConvertIntTo2Complement(temp);
                break;
                
            default:
                break;
        }

        /* NOTE: should add the output here */

        *position++;

        switch (second) {
            case AbsoluteAddressing:
                instrucitionSecondOperandMemory->ARE = 4; /*4 = 0b100 */
                temp = atoi(node.firstOperand->string.data);
                instrucitionSecondOperandMemory->other.full = ConvertIntTo2Complement(temp);
                break;
                    
            case DirectAddressing:
                tempCellP = ast_checker_get_hash_cell_by_string(astChecker, node.firstOperand->string);
                if (tempCellP != NULL){
                    if (tempCellP->kind == LabalCellKind) {
                        instrucitionSecondOperandMemory->ARE = 2; /* 4 = 0b010 */
                        instrucitionSecondOperandMemory->other.full = ConvertIntTo2Complement(tempCellP->value.labal->position);
                    }
                    else /* if (tempCellP->kind == ExternalCellKind) */ {
                        instrucitionSecondOperandMemory->ARE = 1; /* 1 = 0b001 */
                        instrucitionSecondOperandMemory->other.full = 0;
                        /* NOTE: should add the output here */
                    }
                }
                    
                break;
                
            case IndirectRegisterAddressing:
            case DirectRegisterAddressing:
                instrucitionSecondOperandMemory->ARE = 4; /*4 = 0b100 */
                temp = atoi(node.firstOperand->string.data[1]); /* on data[0] would be an 'r' so we move one for the number */
                instrucitionSecondOperandMemory->other.rdst = ConvertIntTo2Complement(temp);
                break;
                
            default:
                break;
        }

        /* NOTE: should add the output here */

        /* update position counter secound time */
        *position++;
    }
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
    InstrucitionMemory instrucitionMemory;
    InstrucitionOperandMemory instrucitionFirstOperandMemory;
    InstrucitionOperandMemory instrucitionSecondOperandMemory;
    int position = 100;

    while (instructionLabalList != NULL){
        while (instructionNodeList != NULL){
            update_instruction_memory_stuff(emitter, astChecker, 
                                            instructionNodeList->node, 
                                            &instrucitionMemory, 
                                            &instrucitionFirstOperandMemory,
                                            &instrucitionSecondOperandMemory,
                                            &position);

            instructionNodeList = instructionNodeList->next;
        }
        
        instructionLabalList = instructionLabalList->next;
    }
}