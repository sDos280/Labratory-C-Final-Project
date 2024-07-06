#include "../include/node.h"
#include "../include/parser.h"

/**
 * Add a token refrence to the end of the token refrence list
 *
 * @param list the refrence to the token refrence list.
 * @param Token the token refrence.
*/
static void add_token_refrence_to_the_token_refrence_list(TokenRefrenceList ** list, Token * Token){
    if (list == NULL) return;
    TokenRefrenceList * head = *list;
    TokenRefrenceList * toAdd = malloc(sizeof(TokenRefrenceList));
    toAdd->token = Token;
    toAdd->next = NULL;

    while (head->next != NULL)
    {
        head = head->next;
    }

    head->next = toAdd;
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
    while (translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens->token.kind == EOL) return;

        translationUnit->tokens = translationUnit->tokens->next;
    }
}

DataNode parser_parse_data_guidance_sentence(TranslationUnit * translationUnit){
    DataNode dataNode;
    dataNode.numbers = NULL;
    TokenRefrenceList * listHead = NULL;
    TokenRefrenceList * listBack = NULL;
    TokenError error;

    if (translationUnit->tokens->token.kind != DATA_INS){
        error.message = string_init_with_data("No .data was found");
        error.token = translationUnit->tokens->token;

        error_handler_push_token_error(&translationUnit->errorHandler, PreprocessorErrorKind, error);

        parser_move_to_last_end_of_line(translationUnit);
        return dataNode;
    }

    while (translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens->token.kind == NUMBER) {
            /* add the number to the token refrence list */
            if (listBack == NULL){ /* check if no number was added before */
                listBack = malloc(sizeof(TokenRefrenceList));
                listBack->token = &translationUnit->tokens->token;
                listBack->next = NULL;

                listHead = listBack;
            } else {
                listHead->next = malloc(sizeof(TokenRefrenceList));
                listHead->next->token = &translationUnit->tokens->token;
                listHead->next->next = NULL;

                listHead = listHead->next;
            }

            translationUnit->tokens = translationUnit->tokens->next; /* move over the number token */

            if (translationUnit->tokens->token.kind == COMMA){
                translationUnit->tokens = translationUnit->tokens->next; /* move over the comma token */
            } else if (translationUnit->tokens->token.kind == EOL){
                translationUnit->tokens = translationUnit->tokens->next; /* move over the EOL token */
                break;
            } else if (translationUnit->tokens->token.kind == EOFT){
                break;
            } else {
                error.message = string_init_with_data("No comma or end of line found");
                error.token = translationUnit->tokens->token;

                error_handler_push_token_error(&translationUnit->errorHandler, PreprocessorErrorKind, error);

                parser_move_to_last_end_of_line(translationUnit);
                return dataNode;
            }
        } else {
                error.message = string_init_with_data("No number found");
                error.token = translationUnit->tokens->token;

                error_handler_push_token_error(&translationUnit->errorHandler, PreprocessorErrorKind, error);

                parser_move_to_last_end_of_line(translationUnit);
                return dataNode;
            }
    }
}