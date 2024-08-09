#include "../include/safe_allocations.h"
#include "../include/node.h"
#include "../include/parser.h"

/**
 * Add a token refrence to the end of the token refrence list
 *
 * @param list the refrence to the token refrence list.
 * @param Token the token refrence.
*/
static void add_token_refrence_to_the_token_refrence_list(TokenRefrenceList ** list, Token * Token){
    TokenRefrenceList * head, * toAdd;

    if (list == NULL) return;

    head = *list;
    toAdd = safe_malloc(sizeof(TokenRefrenceList));
    toAdd->token = Token;
    toAdd->next = NULL;

    if (head == NULL){
        *list = toAdd;
    } else {
        while (head->next != NULL)
        {
            head = head->next;
        }

        head->next = toAdd;
    }
}

/**
 * Parse instruction operand
 *
 * @param translationUnit the translation unit.
 * @return the instruction operand
*/
static InstructionOperand parser_parse_instruction_operand(TranslationUnit * translationUnit, bool * wasError){
    InstructionOperand out;
    TokenError error;
    out.Operand = NULL;
    out.isOperandDerefrenced = false;

    *wasError = true;

    /* check if a number operand */
    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == HASHTAG) {
        /* look for a number operator */
        translationUnit->tokens = translationUnit->tokens->next; /* move over the # token */

        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != NUMBER) {
            error.message = string_init_with_data("A # was found but no number was found after it");
            error.token = translationUnit->tokens->token;

            error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

            parser_move_to_last_end_of_line(translationUnit);
            return out;
        }

        out.Operand = &translationUnit->tokens->token;
        translationUnit->tokens = translationUnit->tokens->next; /* move over the number token */

        *wasError = false;

        return out;
    }

    /* check if a identifier/register operand */
    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == STAR) {
        /* a derefrence on operand is looked for */
        
        out.isOperandDerefrenced = true;
        translationUnit->tokens = translationUnit->tokens->next; /* move over the * token */
    } 
    
    if (translationUnit->tokens != NULL && !(translationUnit->tokens->token.kind == REGISTER || translationUnit->tokens->token.kind == IDENTIFIER)) {
        error.message = string_init_with_data("No operand token was found here (looking for register/identifier)");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return out;
    }

    out.Operand = &translationUnit->tokens->token; /* the token is identifier/register */
    translationUnit->tokens = translationUnit->tokens->next; /* move over the identifier/register token */

    *wasError = false;

    return out;
}

void parser_init_translation_unit(TranslationUnit * translationUnit, Lexer lexer){
    translationUnit->externalNodeList = NULL;
    translationUnit->entryNodeList = NULL;
    translationUnit->instructionLabalList = NULL;
    translationUnit->guidanceLabalList = NULL;

    translationUnit->tokens = lexer.tokens;

    error_handler_init(&translationUnit->errorHandler, lexer.string, lexer.filePath);
}

void parser_free_translation_unit(TranslationUnit * translationUnit){
    void * copy;
    /* free translationUnit's lists */
    while (translationUnit->externalNodeList != NULL)
    {
        copy = translationUnit->externalNodeList->next;
        free(translationUnit->externalNodeList);
        translationUnit->externalNodeList = copy;
    }
    while (translationUnit->entryNodeList != NULL)
    {
        copy = translationUnit->entryNodeList->next;
        free(translationUnit->entryNodeList);
        translationUnit->entryNodeList = copy;
    }
    while (translationUnit->instructionLabalList != NULL){
        parser_free_instruction_sentences(translationUnit->instructionLabalList->labal.instructionNodeList);
        copy = translationUnit->instructionLabalList->next;
        free(translationUnit->instructionLabalList);
        translationUnit->instructionLabalList = copy;
    }
    while (translationUnit->guidanceLabalList != NULL){
        parser_free_guidance_sentences(translationUnit->guidanceLabalList->labal.guidanceNodeList);
        copy = translationUnit->guidanceLabalList->next;
        free(translationUnit->guidanceLabalList);
        translationUnit->guidanceLabalList = copy;
    }

    /* free the error handler */
    error_handler_free_error_list(&translationUnit->errorHandler);
}

void parser_move_to_last_end_of_line(TranslationUnit * translationUnit){
    while (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOL) return;

        translationUnit->tokens = translationUnit->tokens->next;
    }
}

DataNode parser_parse_data_guidance_sentence(TranslationUnit * translationUnit){
    DataNode dataNode;
    TokenError error;
    dataNode.numbers = NULL;

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != DATA_INS){
        error.message = string_init_with_data("No .data was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return dataNode;
    }

    translationUnit->tokens = translationUnit->tokens->next; /* move over the .data token */

    while (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == NUMBER) {
            add_token_refrence_to_the_token_refrence_list(&dataNode.numbers, &translationUnit->tokens->token);

            translationUnit->tokens = translationUnit->tokens->next; /* move over the number token */

            if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == COMMA){
                translationUnit->tokens = translationUnit->tokens->next; /* move over the comma token */
            } else if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOL){
                translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
                break;
            } else if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOFT){
                break;
            } else {
                error.message = string_init_with_data("No comma or end of line found here");
                error.token = translationUnit->tokens->token;

                error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

                parser_move_to_last_end_of_line(translationUnit);
                return dataNode;
            }
        } else {
            error.message = string_init_with_data("No number was found here");
            error.token = translationUnit->tokens->token;

            error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

            parser_move_to_last_end_of_line(translationUnit);
            return dataNode;
        }
    }

    if (dataNode.numbers == NULL){
        error.message = string_init_with_data("No numbers were found after .data");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
    }

    return dataNode;
}

void parser_free_data_guidance_sentence(DataNode dataNode){
    TokenRefrenceList * head = dataNode.numbers;
    TokenRefrenceList * copy = head;

    while (head != NULL){
        copy = head;
        head = head->next;
        free(copy);
    }
}

StringNode parser_parse_string_guidance_sentence(TranslationUnit * translationUnit){
    StringNode stringNode;
    TokenError error;
    stringNode.token = NULL;

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != STRING_INS){
        error.message = string_init_with_data("No .string was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return stringNode;
    }

    translationUnit->tokens = translationUnit->tokens->next; /* move over the .string token */

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != STRING){
        error.message = string_init_with_data("No string literal was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return stringNode;
    }

    stringNode.token = &translationUnit->tokens->token;
    translationUnit->tokens = translationUnit->tokens->next; /* move over the .string token */

    if (translationUnit->tokens != NULL && !(translationUnit->tokens->token.kind == EOL || translationUnit->tokens->token.kind == EOFT)){
        error.message = string_init_with_data("No end of line found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return stringNode;
    }

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOL){
        translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
    }

    return stringNode;
}

GuidanceNodeList * parser_parse_guidance_sentences(TranslationUnit * translationUnit){
    GuidanceNodeList * guidanceList = NULL;
    GuidanceNodeList ** guidanceListLast = &guidanceList; /* the last node in the guidanceList */

    while (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOL){
            translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
        } else if (translationUnit->tokens != NULL && (translationUnit->tokens->token.kind == DATA_INS || translationUnit->tokens->token.kind == STRING_INS)){
            if (guidanceList == NULL){ /* no sentence was added yet */
                guidanceList = safe_malloc(sizeof(GuidanceNodeList));
                guidanceList->next = NULL;

                if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == DATA_INS){
                    guidanceList->kind = DataNodeKind;
                    guidanceList->node.dataNode = parser_parse_data_guidance_sentence(translationUnit);
                } else if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == STRING_INS){
                    guidanceList->kind = StringNodeKind;
                    guidanceList->node.stringNode = parser_parse_string_guidance_sentence(translationUnit);
                }

                guidanceListLast = &guidanceList;
            } else { /* at least one sentence was added */
                (*guidanceListLast)->next = safe_malloc(sizeof(GuidanceNodeList));
                guidanceListLast = &((*guidanceListLast)->next); /* update guidanceListLast to the last allocated node */
                (*guidanceListLast)->next = NULL;

                if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == DATA_INS){
                    (*guidanceListLast)->kind = DataNodeKind;
                    (*guidanceListLast)->node.dataNode = parser_parse_data_guidance_sentence(translationUnit);
                } else if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == STRING_INS){
                    (*guidanceListLast)->kind = StringNodeKind;
                    (*guidanceListLast)->node.stringNode = parser_parse_string_guidance_sentence(translationUnit);
                }
            }
        } else {
            break; /* the end of the grammer*/ 
        }
    }

    return guidanceList;
}

void parser_free_guidance_sentences(GuidanceNodeList * guidanceList){
    GuidanceNodeList * copy;

    while (guidanceList != NULL){
        if (guidanceList->kind == DataNodeKind){
            parser_free_data_guidance_sentence(guidanceList->node.dataNode);
        } /*else if (guidanceList->kind == StringNodeKind){ there is no need for that check because the StringNode doesn't use malloc/calloc
            ...
        }*/
        copy = guidanceList->next;
        free(guidanceList);
        guidanceList = copy;
    }
}

InstructionNode parser_parse_instruction_sentence(TranslationUnit * translationUnit){
    InstructionNode instruction;
    TokenError error;
    InstructionOperand firstOperand;
    InstructionOperand secondOperand;
    bool wasErrorInOperand = false;
    instruction.operation = NULL;
    instruction.firstOperand = NULL;
    instruction.isFirstOperandDerefrenced = false;
    instruction.secondOperand = NULL;
    instruction.isSecondOperandDerefrenced = false;
    firstOperand.Operand = NULL;
    firstOperand.isOperandDerefrenced = false;
    secondOperand.Operand = NULL;
    secondOperand.isOperandDerefrenced = false;
    

    if (translationUnit->tokens != NULL && 
        !(
            translationUnit->tokens->token.kind == MOV ||
            translationUnit->tokens->token.kind == CMP ||
            translationUnit->tokens->token.kind == ADD ||
            translationUnit->tokens->token.kind == SUB ||
            translationUnit->tokens->token.kind == LEA ||
            translationUnit->tokens->token.kind == CLR ||
            translationUnit->tokens->token.kind == NOT ||
            translationUnit->tokens->token.kind == INC ||
            translationUnit->tokens->token.kind == DEC ||
            translationUnit->tokens->token.kind == JMP ||
            translationUnit->tokens->token.kind == BNE ||
            translationUnit->tokens->token.kind == RED ||
            translationUnit->tokens->token.kind == PRN ||
            translationUnit->tokens->token.kind == JSR ||
            translationUnit->tokens->token.kind == RTS ||
            translationUnit->tokens->token.kind == STOP
        )
    ){
        error.message = string_init_with_data("No instruction was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return instruction;
    }

    instruction.operation = &translationUnit->tokens->token;
    translationUnit->tokens = translationUnit->tokens->next; /* move over the instruction token */

    /* check if a zero operand instruction, if it is, just return */
    if (translationUnit->tokens != NULL && (translationUnit->tokens->token.kind == EOL || translationUnit->tokens->token.kind == EOFT)){
        return instruction;
    }

    /* parse operands */
    firstOperand = parser_parse_instruction_operand(translationUnit, &wasErrorInOperand);

    if (wasErrorInOperand == true) return instruction; /* there is no need to raise any errors since parser_parse_instruction_operand already done that */

    instruction.firstOperand = firstOperand.Operand;
    instruction.isFirstOperandDerefrenced = firstOperand.isOperandDerefrenced;

    /* check if a one operand instruction, if it is, just return */
    if (translationUnit->tokens != NULL && (translationUnit->tokens->token.kind == EOL || translationUnit->tokens->token.kind == EOFT)){
        return instruction;
    }

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != COMMA){
        error.message = string_init_with_data("No comma was found after the first operand");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return instruction;
    }

    translationUnit->tokens = translationUnit->tokens->next; /* move over the , token */

    secondOperand = parser_parse_instruction_operand(translationUnit, &wasErrorInOperand);

    if (wasErrorInOperand == true) return instruction; /* there is no need to raise any errors since parser_parse_instruction_operand already done that */

    instruction.secondOperand = secondOperand.Operand;
    instruction.isSecondOperandDerefrenced = secondOperand.isOperandDerefrenced;

    if (translationUnit->tokens != NULL && !(translationUnit->tokens->token.kind == EOL || translationUnit->tokens->token.kind == EOFT)){
        error.message = string_init_with_data("No end of line found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return instruction;
    }

    return instruction;
}

InstructionNodeList * parser_parse_instruction_sentences(TranslationUnit * translationUnit){
    InstructionNodeList * instructionList = NULL;
    InstructionNodeList ** instructionListLast = &instructionList; /* the last node in the instructionList */

    while (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOL){
            translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
        } else if (translationUnit->tokens != NULL && 
                  (translationUnit->tokens->token.kind == MOV ||
                   translationUnit->tokens->token.kind == CMP ||
                   translationUnit->tokens->token.kind == ADD ||
                   translationUnit->tokens->token.kind == SUB ||
                   translationUnit->tokens->token.kind == LEA ||
                   translationUnit->tokens->token.kind == CLR ||
                   translationUnit->tokens->token.kind == NOT ||
                   translationUnit->tokens->token.kind == INC ||
                   translationUnit->tokens->token.kind == DEC ||
                   translationUnit->tokens->token.kind == JMP ||
                   translationUnit->tokens->token.kind == BNE ||
                   translationUnit->tokens->token.kind == RED ||
                   translationUnit->tokens->token.kind == PRN ||
                   translationUnit->tokens->token.kind == JSR ||
                   translationUnit->tokens->token.kind == RTS ||
                   translationUnit->tokens->token.kind == STOP)){
            if (instructionList == NULL){ /* no sentence was added yet */
                instructionList = safe_malloc(sizeof(InstructionNodeList));
                instructionList->next = NULL;
                instructionList->node = parser_parse_instruction_sentence(translationUnit);
                instructionListLast = &instructionList;
            } else { /* at least one sentence was added */
                (*instructionListLast)->next = safe_malloc(sizeof(InstructionNodeList));
                instructionListLast = &((*instructionListLast)->next); /* update guidanceListLast to the last allocated node */
                (*instructionListLast)->next = NULL;

                (*instructionListLast)->node = parser_parse_instruction_sentence(translationUnit);
            }
        } else {
            break; /* the end of the grammer*/ 
        }
    }

    return instructionList;
}

void parser_free_instruction_sentences(InstructionNodeList * instractionList){
    InstructionNodeList * copy;

    while (instractionList != NULL){
        copy = instractionList->next;
        free(instractionList);
        instractionList = copy;
    }
}

EntryNode parser_parse_entry_sentence(TranslationUnit * translationUnit){
    EntryNode node;
    TokenError error;
    node.token = NULL;

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != ENTRY_INS){
        error.message = string_init_with_data("No .entry was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return node;
    }

    translationUnit->tokens = translationUnit->tokens->next; /* move over the .entry token */

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != IDENTIFIER){
        error.message = string_init_with_data("No identifier was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return node;
    }

    node.token = &translationUnit->tokens->token;
    translationUnit->tokens = translationUnit->tokens->next; /* move over the identifier token */

    if (translationUnit->tokens != NULL && !(translationUnit->tokens->token.kind == EOL || translationUnit->tokens->token.kind == EOFT)){
        error.message = string_init_with_data("No end of line was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return node;
    }

    translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */

    return node;
}

ExternalNode parser_parse_external_sentence(TranslationUnit * translationUnit){
    ExternalNode node;
    TokenError error;
    node.token = NULL;

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != EXTERN_INS){
        error.message = string_init_with_data("No .external was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return node;
    }

    translationUnit->tokens = translationUnit->tokens->next; /* move over the .entry token */

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != IDENTIFIER){
        error.message = string_init_with_data("No identifier was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return node;
    }

    node.token = &translationUnit->tokens->token;
    translationUnit->tokens = translationUnit->tokens->next; /* move over the identifier token */

    if (translationUnit->tokens != NULL && !(translationUnit->tokens->token.kind == EOL || translationUnit->tokens->token.kind == EOFT)){
        error.message = string_init_with_data("No end of line was found here");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return node;
    }

    translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */

    return node;
}

LabalNode parser_parse_labal(TranslationUnit * translationUnit){
    LabalNode labal;
    TokenError error;
    bool wasLabalIdentifierFound = false;
    labal.guidanceNodeList = NULL;
    labal.instructionNodeList = NULL;
    labal.labal = NULL;
    labal.position = 0;
    labal.size = 0;

    if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == IDENTIFIER){
        labal.labal = &translationUnit->tokens->token;
        translationUnit->tokens = translationUnit->tokens->next; /* move over the labal identifier */
            
        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != COLON){
            error.message = string_init_with_data("No colon was found after labal identifier");
            error.token = translationUnit->tokens->token;

            error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

            parser_move_to_last_end_of_line(translationUnit);
            return labal;
        }

        if (labal.labal->index + string_length(labal.labal->string) != translationUnit->tokens->token.index){
            error.message = string_init_with_data("The colon should be exactly next to the labal identifier");
            error.token = translationUnit->tokens->token;

            error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

            parser_move_to_last_end_of_line(translationUnit);
            return labal;
        }

        translationUnit->tokens = translationUnit->tokens->next; /* move over the colon token */

        /* move over the EOL that are after the colon*/
        while (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != EOFT)
            if (translationUnit->tokens->token.kind == EOL)
                translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
            else
                break;
        
        wasLabalIdentifierFound = true;
    }

    if (translationUnit->tokens != NULL && 
       (translationUnit->tokens->token.kind == MOV ||
        translationUnit->tokens->token.kind == CMP ||
        translationUnit->tokens->token.kind == ADD ||
        translationUnit->tokens->token.kind == SUB ||
        translationUnit->tokens->token.kind == LEA ||
        translationUnit->tokens->token.kind == CLR ||
        translationUnit->tokens->token.kind == NOT ||
        translationUnit->tokens->token.kind == INC ||
        translationUnit->tokens->token.kind == DEC ||
        translationUnit->tokens->token.kind == JMP ||
        translationUnit->tokens->token.kind == BNE ||
        translationUnit->tokens->token.kind == RED ||
        translationUnit->tokens->token.kind == PRN ||
        translationUnit->tokens->token.kind == JSR ||
        translationUnit->tokens->token.kind == RTS ||
        translationUnit->tokens->token.kind == STOP)){
        if (wasLabalIdentifierFound == false){
            error.message = string_init_with_data("An instruction was found here but no labal identifier, please add a labal identifier");
            error.token = translationUnit->tokens->token;

            error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

            parser_move_to_last_end_of_line(translationUnit);
            return labal;
        }

        labal.instructionNodeList = parser_parse_instruction_sentences(translationUnit);
        labal.guidanceNodeList = NULL;
        labal.size = 0; /* not yet initialized */
        labal.position = 0; /* not yet initialized */
        return labal;
    }

    else if (translationUnit->tokens != NULL && 
       (translationUnit->tokens->token.kind == STRING_INS ||
        translationUnit->tokens->token.kind == DATA_INS)){
        labal.guidanceNodeList = parser_parse_guidance_sentences(translationUnit);
        labal.instructionNodeList = NULL;
        labal.size = 0; /* not yet initialized */
        labal.position = 0; /* not yet initialized */
        return labal;
    } 

    else {
        error.message = string_init_with_data("No instraction/guidance was found here");
        error.token = (wasLabalIdentifierFound == true)? *labal.labal : translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

        /*parser_move_to_last_end_of_line(translationUnit); we moved all over the \n token to get to here (from the labal identifier while loop)
         so there is no need to do the same here */
        return labal;
    }

    return labal;
}

void parser_parse_translation_unit(TranslationUnit * translationUnit){
    ExternalNodeList * externalNodeList = NULL;
    ExternalNodeList ** externalNodeListLast = NULL;
    EntryNodeList * entryNodeList = NULL;
    EntryNodeList ** entryNodeListLast = NULL;
    LabalNodeList * instructionLabalList = NULL;
    LabalNodeList ** instructionLabalListLast = NULL;
    LabalNodeList * guidanceLabalList = NULL;
    LabalNodeList ** guidanceLabalListLast = NULL;
    TokenError error;
    LabalNode labal;
    bool wasLabalFound = false;
    labal.guidanceNodeList = NULL;
    labal.instructionNodeList = NULL;
    labal.labal = NULL;
    labal.position = 0;
    labal.size = 0;
    

    while (translationUnit->tokens != NULL && translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOL){
            translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
        } else if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EXTERN_INS){
            if (externalNodeList == NULL){
                externalNodeList = safe_malloc(sizeof(ExternalNodeList));
                externalNodeList->next = NULL;
                externalNodeList->node = parser_parse_external_sentence(translationUnit);
                externalNodeListLast = &externalNodeList;
            }else{
                (*externalNodeListLast)->next = safe_malloc(sizeof(ExternalNodeList));
                externalNodeListLast = &(*externalNodeListLast)->next;
                (*externalNodeListLast)->next = NULL;
                (*externalNodeListLast)->node = parser_parse_external_sentence(translationUnit);
            }
        } else if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == ENTRY_INS){
            if (entryNodeList == NULL){
                entryNodeList = safe_malloc(sizeof(EntryNodeList));
                entryNodeList->next = NULL;
                entryNodeList->node = parser_parse_entry_sentence(translationUnit);
                entryNodeListLast = &entryNodeList;
            }else{
                (*entryNodeListLast)->next = safe_malloc(sizeof(EntryNodeList));
                entryNodeListLast = &(*entryNodeListLast)->next;
                (*entryNodeListLast)->next = NULL;
                (*entryNodeListLast)->node = parser_parse_entry_sentence(translationUnit);
            }
        }

        else if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == IDENTIFIER){
            labal = parser_parse_labal(translationUnit);
            wasLabalFound = true;
        } else if (translationUnit->tokens != NULL &&
                  (translationUnit->tokens->token.kind == DATA_INS ||
                   translationUnit->tokens->token.kind == STRING_INS)){ /* check if a guidance sentences without labal identifier */
            labal = parser_parse_labal(translationUnit);
            wasLabalFound = true;
        } else {
            error.message = string_init_with_data("No labal starter/external/entry was found here");
            error.token = translationUnit->tokens->token;

            error_handler_push_token_error(&translationUnit->errorHandler, ParserErrorKind, error);

            parser_move_to_last_end_of_line(translationUnit);
        }

        /* add the labal to the coresponding list */
        if (wasLabalFound == true){
            if (labal.instructionNodeList != NULL){
                if (instructionLabalList == NULL){ 
                    instructionLabalList = safe_malloc(sizeof(LabalNodeList));
                    instructionLabalList->next = NULL;
                    instructionLabalList->labal = labal;
                    instructionLabalListLast = &instructionLabalList;
                } else {
                    (*instructionLabalListLast)->next = safe_malloc(sizeof(LabalNodeList));
                    instructionLabalListLast = &(*instructionLabalListLast)->next;
                    (*instructionLabalListLast)->next = NULL;
                    (*instructionLabalListLast)->labal = labal;
                }
            }

            if (labal.guidanceNodeList != NULL){
                if (guidanceLabalList == NULL){ 
                    guidanceLabalList = safe_malloc(sizeof(LabalNodeList));
                    guidanceLabalList->next = NULL;
                    guidanceLabalList->labal = labal;
                    guidanceLabalListLast = &guidanceLabalList;
                } else {
                    (*guidanceLabalListLast)->next = safe_malloc(sizeof(LabalNodeList));
                    guidanceLabalListLast = &(*guidanceLabalListLast)->next;
                    (*guidanceLabalListLast)->next = NULL;
                    (*guidanceLabalListLast)->labal = labal;
                }
            }

            wasLabalFound = false;
            labal.guidanceNodeList = NULL;
            labal.instructionNodeList = NULL;
            labal.labal = NULL;
            labal.position = 0;
            labal.size = 0;
        }
    }

    translationUnit->entryNodeList = entryNodeList;
    translationUnit->externalNodeList = externalNodeList;
    translationUnit->guidanceLabalList = guidanceLabalList;
    translationUnit->instructionLabalList = instructionLabalList;
}