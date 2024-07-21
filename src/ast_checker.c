#include "../include/ast_checker.h"
#include "../include/string_util.h"

#define MULTIPLIER (37)
#define HIGHST_INTEGER_IN_COMPILER  (signed int)(((2 << (14-1))) -1)
#define LOWEST_INTEGER_IN_COMPILER (signed int)(-(2 << (14-1)))

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
    EntryNodeList * entryNodeList = translationUnit->entryNodeList;
    LabalNodeList * instructionLabalList = translationUnit->instructionLabalList;
    LabalNodeList * guidanceLabalList = translationUnit->guidanceLabalList;

    astChecker->size = 0;

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
        guidanceLabalList = guidanceLabalList->next;
    }

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
    unsigned long index = hash(key) % astChecker->size - 1;
    unsigned long indexCopy = hash(key) % astChecker->size - 1;
    IdentifierHashCell * out = NULL;

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
    unsigned long index = hash(*cell.key) % astChecker->size - 1;
    unsigned long indexCopy = hash(*cell.key) % astChecker->size - 1;

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

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerKind, error);
        }else if (value < LOWEST_INTEGER_IN_COMPILER){
            error.message = string_init_with_data("This number is too low");
            error.token = *numbers->token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerKind, error);
        }

        numbers = numbers->next;
    }
}

void ast_checker_check_duplicate_identifiers(AstChecker * astChecker){

}