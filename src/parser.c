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
    toAdd = malloc(sizeof(TokenRefrenceList));
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

void parser_init_translation_unit(TranslationUnit * translationUnit, Lexer lexer){
    translationUnit->externalNodeList = NULL;
    translationUnit->entryNodeList = NULL;
    translationUnit->instructionLabalList = NULL;
    translationUnit->guidanceLabalList = NULL;

    translationUnit->tokens = lexer.tokens;

    error_handler_init(&translationUnit->errorHandler, lexer.string, lexer.filePath);
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
        error.message = string_init_with_data("No comma or end of line found here");
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

    while (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOFT){
        if (translationUnit->tokens != NULL && translationUnit->tokens->token.kind == EOL){
            translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
        } else if (translationUnit->tokens != NULL && (translationUnit->tokens->token.kind == DATA_INS || translationUnit->tokens->token.kind == STRING_INS)){
            if (guidanceList == NULL){ /* no sentence was added yet */
                guidanceList = malloc(sizeof(GuidanceNodeList));
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
                (*guidanceListLast)->next = malloc(sizeof(GuidanceNodeList));
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