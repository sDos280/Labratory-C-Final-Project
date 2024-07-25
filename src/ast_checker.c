#include "../include/ast_checker.h"
#include "../include/string_util.h"

#define MULTIPLIER (37)
#define HIGHST_INTEGER_IN_COMPILER  (signed int)(((2 << (14-1))) -1)
#define LOWEST_INTEGER_IN_COMPILER (signed int)(-(2 << (14-1)))

/**
 * Get the addressing mod of an operand, if the grammar is wrond raise an error.
 *
 * @param astChecker the ast checker.
 * @param operand_token the operand token.
 * @param operand_token true if the operand is derefrenced else false.
 * @return the addressing mode.
*/
static AddressingMode get_addressing_mode_of_operand(AstChecker * astChecker, Token * operand_token, bool isDerefrenced){
    TokenError error;
    if (operand_token->kind == NUMBER) {
        if (isDerefrenced == true){
            error.message = string_init_with_data("A Number can't be derefrenced");
            error.token = *operand_token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
        }
        
        return AbsoluteAddressing;
    } else if (operand_token->kind == IDENTIFIER) {
        if (isDerefrenced == true){
            error.message = string_init_with_data("A Labal can't be derefrenced");
            error.token = *operand_token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
        }
        
        return DirectAddressing;
    } else if (operand_token->kind == REGISTER) {
        if (isDerefrenced == true) return IndirectRegisterAddressing;
        
        return DirectRegisterAddressing;
    } else {
        error.message = string_init_with_data("Wrong operand grammar");
        error.token = *operand_token;

        error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
    }

    return AbsoluteAddressing;
}

unsigned long hash(String str) {
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) str.data;

    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 

    return h;
}

void ast_checker_init(AstChecker * astChecker, TranslationUnit * translationUnit, Lexer lexer){
    ExternalNodeList * externalNodeList = translationUnit->externalNodeList;
    LabalNodeList * instructionLabalList = translationUnit->instructionLabalList;
    LabalNodeList * guidanceLabalList = translationUnit->guidanceLabalList;
    astChecker->hash = NULL;
      astChecker->size = 0;
    /*unsigned int i = 0;
    IdentifierHashCell temp;
    temp.key = NULL;
    temp.value.labal = NULL;
    temp.value.external = NULL;
    temp.kind = 0;
    temp.hasEntry = false;

    */

    /* add the size of the instraction labals (note, all instruction labals have a labal so we can add that with no problem) */
    while (instructionLabalList != NULL){
        astChecker->size++;
        instructionLabalList = instructionLabalList->next;
    }

    /* add the size of the guidance labals */
    while (guidanceLabalList != NULL){
        if (guidanceLabalList->labal.labal != NULL)
            astChecker->size++;
        guidanceLabalList = guidanceLabalList->next;
    }

    /* add the size of the external labals */
    while (externalNodeList != NULL){
        astChecker->size++;
        externalNodeList = externalNodeList->next;
    }

    printf("size: %d\n", astChecker->size);

    /* allocate the hash memory */
    astChecker->hash = calloc(astChecker->size, sizeof(IdentifierHashCell));

    error_handler_init(&astChecker->errorHandler, lexer.string, lexer.filePath);
}

void ast_checker_free(AstChecker * astChecker){
    /* free the hash memory */
    free(astChecker->hash);

    error_handler_free_error_list(&astChecker->errorHandler);
}

IdentifierHashCell * ast_checker_get_hash_cell_by_string(AstChecker * astChecker, String key){
    unsigned long index = hash(key) % astChecker->size;
    unsigned long indexCopy = hash(key) % astChecker->size;

    /* Loop till we find an empty entry. */
    while (astChecker->hash[index].key != NULL) {
        if (strcmp(key.data, astChecker->hash[index].key->data) == 0) {
            return astChecker->hash + index;
        }

        index++;

        /* loop back to start*/
        if (index >= astChecker->size) {
            index = 0;
        }

        /* we looped once but we found nothing so we just break */
        if (index == indexCopy) break;
    }

    return NULL;
}

bool ast_checker_set_hash_cell_by_string(AstChecker * astChecker, IdentifierHashCell cell){
    unsigned long index = hash(*cell.key) % astChecker->size;
    unsigned long indexCopy = hash(*cell.key) % astChecker->size;

    cell.wasEntryAdded = false; /* make sure that under any circumstances the ast checker couldn't 
    interfire with the work of the emitter in relation to the entry check */

    /* Loop till we find an empty entry. */
    while (astChecker->hash[index].key != NULL) {
        /* check if there is an existing cell with the cell with the same key as passed*/
        if (strcmp(cell.key->data, astChecker->hash[index].key->data) == 0) {
            return false;
        }

        index++;

        /* loop back to start*/
        if (index >= astChecker->size) {
            index = 0;
        }

        /* we looped once but we found nothing so we just break */
        if (index == indexCopy) return false;
    }

    /* we are now in an empty place */
    astChecker->hash[index] = cell;
    return true;
}

void ast_checker_check_data_guidance_sentence(AstChecker * astChecker, DataNode node){
    TokenRefrenceList * numbers = node.numbers;
    TokenError error;
    int value;

    while (numbers != NULL){
        value = atoi(numbers->token->string.data);
        if (value > HIGHST_INTEGER_IN_COMPILER){
            error.message = string_init_with_data("This number is too high");
            error.token = *numbers->token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
        }else if (value < LOWEST_INTEGER_IN_COMPILER){
            error.message = string_init_with_data("This number is too low");
            error.token = *numbers->token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
        }

        numbers = numbers->next;
    }
}

void ast_checker_check_instruction_sentence(AstChecker * astChecker, InstructionNode node){
    Token * source = NULL;
    bool isSourceDerefrenced = false;
    Token * destination = NULL;
    bool isDestinationDerefrenced = false;
    TokenError error;
    AddressingMode AM = AbsoluteAddressing;

    /* check for valid operator kind */
    if (node.firstOperand == NULL && node.secondOperand == NULL) {} /* no arguments, so no update is needed*/
    else if (node.secondOperand == NULL){
        destination = node.firstOperand;
        isDestinationDerefrenced = node.isFirstOperandDerefrenced;
    } else { /* both argument */
        source = node.firstOperand;
        isSourceDerefrenced = node.isFirstOperandDerefrenced;
        destination = node.secondOperand;
        isDestinationDerefrenced = node.isSecondOperandDerefrenced;
    }

    if (node.operation->kind == CMP){
        if (source == NULL || destination == NULL){
            error.message = string_init_with_data("This instruction should have 2 operands");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    } else if (node.operation->kind == MOV || node.operation->kind == ADD || node.operation->kind == SUB){
        if (source == NULL || destination == NULL){
            error.message = string_init_with_data("This instruction should have 2 operands");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }

        /* AM of source can be anything, so we check AM of destination */
        AM = get_addressing_mode_of_operand(astChecker, destination, isDestinationDerefrenced);
        if (AM == AbsoluteAddressing){
            error.message = string_init_with_data("Wrong addressing mode of operand");
            error.token = *destination;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    } else if (node.operation->kind == LEA){
        if (source == NULL || destination == NULL){
            error.message = string_init_with_data("This instruction should have 2 operands");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }

        AM = get_addressing_mode_of_operand(astChecker, source, isSourceDerefrenced);
        if (AM != DirectAddressing){
            error.message = string_init_with_data("Wrong addressing mode of operand");
            error.token = *source;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }

        AM = get_addressing_mode_of_operand(astChecker, destination, isDestinationDerefrenced);
        if (AM == AbsoluteAddressing){
            error.message = string_init_with_data("Wrong addressing mode of operand");
            error.token = *destination;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    } else if (node.operation->kind == CLR || node.operation->kind == NOT || node.operation->kind == INC || node.operation->kind == DEC || node.operation->kind == RED){
        if (source != NULL || destination == NULL){
            error.message = string_init_with_data("This instruction should have destination but no source");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }

        AM = get_addressing_mode_of_operand(astChecker, destination, isDestinationDerefrenced);
        if (AM == AbsoluteAddressing){
            error.message = string_init_with_data("Wrong addressing mode of operand");
            error.token = *destination;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    } else if (node.operation->kind == JMP || node.operation->kind == BNE || node.operation->kind == JSR){
        if (source != NULL || destination == NULL){
            error.message = string_init_with_data("This instruction should have destination but no source");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
        
        AM = get_addressing_mode_of_operand(astChecker, destination, isDestinationDerefrenced);
        if (AM == AbsoluteAddressing || AM == DirectRegisterAddressing){
            error.message = string_init_with_data("Wrong addressing mode of operand");
            error.token = *destination;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    } else if (node.operation->kind == RTS || node.operation->kind == STOP){
        if (source != NULL || destination != NULL){
            error.message = string_init_with_data("This instruction should have nor destination or source");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    } else if (node.operation->kind == RED){
        if (source != NULL){
            error.message = string_init_with_data("This instruction should have destination but no source");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
        
        AM = get_addressing_mode_of_operand(astChecker, destination, isDestinationDerefrenced);
        if (AM == AbsoluteAddressing){
            error.message = string_init_with_data("Wrong addressing mode of operand");
            error.token = *destination;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    } else if (node.operation->kind == PRN){
        if (source != NULL || destination == NULL){
            error.message = string_init_with_data("This instruction should have destination but no source");
            error.token = *node.operation;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            return;
        }
    }

    /* check of known identifiers */
    if (source != NULL){
        if (source->kind == IDENTIFIER && ast_checker_get_hash_cell_by_string(astChecker, source->string) == NULL){
            error.message = string_init_with_data("Unknown identifier");
            error.token = *source;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
        }
    }
    
    if (destination != NULL){
        if (destination->kind == IDENTIFIER && ast_checker_get_hash_cell_by_string(astChecker, destination->string) == NULL){
            error.message = string_init_with_data("Unknown identifier");
            error.token = *destination;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            
        }
    }

    return;
}

void ast_checker_check_labal(AstChecker * astChecker, LabalNode node){
    TokenError error;
    InstructionNodeList * copyI = node.instructionNodeList;
    GuidanceNodeList * copyG = node.guidanceNodeList;
    
    /* check if a labal with instructions have a labal identifier */
    if (node.instructionNodeList != NULL && node.labal == NULL){
        error.message = string_init_with_data("A labal with instructions should have a labal identifier");
        error.token = *node.instructionNodeList->node.firstOperand;

        error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
    }

    while (copyI != NULL){
        ast_checker_check_instruction_sentence(astChecker, copyI->node);
        copyI = copyI->next;
    }

    while (copyG != NULL){
        if (copyG->kind == DataNodeKind) ast_checker_check_data_guidance_sentence(astChecker, copyG->node.dataNode);
        copyG = copyG->next;
    }
}

void ast_checker_check_duplicate_identifiers(AstChecker * astChecker, TranslationUnit * translationUnit){
    ExternalNodeList * externalNodeList = translationUnit->externalNodeList;
    EntryNodeList * entryNodeList = translationUnit->entryNodeList;
    LabalNodeList * instructionLabalList = translationUnit->instructionLabalList;
    LabalNodeList * guidanceLabalList = translationUnit->guidanceLabalList;
    IdentifierHashCell * cellPointerTemp = NULL;
    IdentifierHashCell cell;
    bool temp = false;
    TokenError error;

    while (instructionLabalList != NULL){
        cell.key = &instructionLabalList->labal.labal->string;
        cell.kind = LabalCellKind;
        cell.value.labal = &instructionLabalList->labal;

        temp = ast_checker_set_hash_cell_by_string(astChecker, cell);

        if (temp == false){
            error.message = string_init_with_data("A Duplicate of this labal was found");
            error.token = *instructionLabalList->labal.labal;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
        }

        instructionLabalList = instructionLabalList->next;
    }

    while (guidanceLabalList != NULL){
        if (guidanceLabalList->labal.labal != NULL){
            cell.key = &guidanceLabalList->labal.labal->string;
            cell.kind = LabalCellKind;
            cell.value.labal = &guidanceLabalList->labal;

            temp = ast_checker_set_hash_cell_by_string(astChecker, cell);

            if (temp == false){
                error.message = string_init_with_data("A Duplicate of this labal was found");
                error.token = *guidanceLabalList->labal.labal;

                error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            }
        }
        

        guidanceLabalList = guidanceLabalList->next;
    }

    while (externalNodeList != NULL){
        cellPointerTemp = ast_checker_get_hash_cell_by_string(astChecker, externalNodeList->node.token->string);
        
        if (cellPointerTemp == NULL){
            /* this external identifier wasn't added yet so we add it*/
            cell.key = &externalNodeList->node.token->string;
            cell.kind = ExternalCellKind;
            cell.value.external = &externalNodeList->node;
        
            temp = ast_checker_set_hash_cell_by_string(astChecker, cell);
        }else {
            /* this external identifier was added already */

            if (cellPointerTemp->kind == LabalCellKind){
                error.message = string_init_with_data("An identifier can't be declare as a labal and also an external");
                error.token = *externalNodeList->node.token;

                error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            } /*else {  cellPointerTemp->kind == ExternalCellKind */
                /* a decleration of external twice isn't really a problem, so is no really a problem
                error.message = string_init_with_data("A Duplicate of this labal was found");
                error.token = *externalNodeList->node.token;

                error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            }*/
        }

        externalNodeList = externalNodeList->next;
    }

    while (entryNodeList != NULL){
        cellPointerTemp = ast_checker_get_hash_cell_by_string(astChecker, entryNodeList->node.token->string);

        if (cellPointerTemp == NULL){
            error.message = string_init_with_data("No labal was found for this entry");
            error.token = *entryNodeList->node.token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
        }else {
            if (cellPointerTemp->kind == ExternalCellKind){
                error.message = string_init_with_data("An entry can't be also an external");
                error.token = *externalNodeList->node.token;

                error_handler_push_token_error(&astChecker->errorHandler, AstCheckerErrorKind, error);
            } /*else { cellPointerTemp->kind == LabalCellKind 
            }*/
        }

        entryNodeList = entryNodeList->next;
    }
}

void ast_checker_check_translation_unit(AstChecker * astChecker, TranslationUnit * translationUnit){
    LabalNodeList * instructionLabalList = NULL;
    LabalNodeList * guidanceLabalList = NULL;
    
    ast_checker_check_duplicate_identifiers(astChecker, translationUnit);

    instructionLabalList = translationUnit->instructionLabalList;
    guidanceLabalList = translationUnit->guidanceLabalList;

    while (instructionLabalList != NULL){
        ast_checker_check_labal(astChecker, instructionLabalList->labal);
        instructionLabalList = instructionLabalList->next;
    }

    while (guidanceLabalList != NULL){
        ast_checker_check_labal(astChecker, guidanceLabalList->labal);
        guidanceLabalList = guidanceLabalList->next;
    }
}