#include "../include/node.h"
#include "../include/parser.h"

void parser_move_to_last_end_of_line(TranslationUnit * translationUnit){
    while (translationUnit->tokens->token.kind != EOFT){
        if (translationUnit->tokens->token.kind == EOL) return;

        translationUnit->tokens = translationUnit->tokens->next;
    }
}