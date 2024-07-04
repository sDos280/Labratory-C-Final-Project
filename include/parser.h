#ifndef LABRATORY_C_FINAL_PROJECT_PARSER_H
#define LABRATORY_C_FINAL_PROJECT_PARSER_H

#include "lexer.h"
#include "node.h"

/**
 * Initialize the translation unit.
 *
 * @param translationUnit the translation unit.
 * @param lexer the lexer.
*/
void parser_init_translation_unit(TranslationUnit * translationUnit, Lexer lexer);

/**
 * Move the current token pointer (tokens) to the end of line token.
 *
 * @param translationUnit the translation unit.
*/
void parser_move_to_last_end_of_line(TranslationUnit * translationUnit);

/**
 * Parse a data guidance sentence
 *
 * @param translationUnit the translation unit.
 * @return the data node
*/
DataNode parser_parse_data_guidance_sentence(TranslationUnit * translationUnit);

#endif /*LABRATORY_C_FINAL_PROJECT_PARSER_H*/
