#include "../include/node.h"
#include "../include/parser.h"

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