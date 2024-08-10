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
 * Free the (memory of) translation unit.
 *
 * @param translationUnit the translation unit.
*/
void parser_free_translation_unit(TranslationUnit * translationUnit);

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

/**
 * Free a data guidance sentence
 *
 * @param translationUnit the translation unit.
*/
void parser_free_data_guidance_sentence(DataNode dataNode);

/**
 * Parse a string guidance sentence
 *
 * @param translationUnit the translation unit.
 * @return the string node
*/
StringNode parser_parse_string_guidance_sentence(TranslationUnit * translationUnit);

/**
 * Parse guidance sentences
 *
 * @param translationUnit the translation unit.
 * @return the head of the guidance node list, if no guidance was found return NULL.
*/
GuidanceNodeList * parser_parse_guidance_sentences(TranslationUnit * translationUnit);

/**
 * Free (the memory of the) guidance sentences
 *
 * @param guidanceList the guidance list head.
*/
void parser_free_guidance_sentences(GuidanceNodeList * guidanceList);


/**
 * Parse an instruction sentence
 *
 * @param translationUnit the translation unit.
 * @return the instruction node
*/
InstructionNode parser_parse_instruction_sentence(TranslationUnit * translationUnit);

/**
 * Parse an instruction sentences
 *
 * @param translationUnit the translation unit.
 * @return the instructions list 
*/
InstructionNodeList * parser_parse_instruction_sentences(TranslationUnit * translationUnit);

/**
 * Free (the memory of the) instruction sentences
 *
 * @param translationUnit the translation unit.
 * @return the instructions list 
*/
void parser_free_instruction_sentences(InstructionNodeList * instractionList);

/**
 * Parse an entry sentence
 *
 * @param translationUnit the translation unit.
 * @return the entry node 
*/
EntryNode parser_parse_entry_sentence(TranslationUnit * translationUnit);

/**
 * Parse an external sentence
 *
 * @param translationUnit the translation unit.
 * @return the external node 
*/
ExternalNode parser_parse_external_sentence(TranslationUnit * translationUnit);

/**
 * Parse a sentence
 *
 * @param translationUnit the translation unit.
 * @return the Sentence node 
*/
Sentence parser_parse_sentence(TranslationUnit * translationUnit);

/**
 * Parse a sentences
 *
 * @param translationUnit the translation unit.
 * @return the Sentences node 
*/
Sentences * parser_parse_sentences(TranslationUnit * translationUnit);

/**
 * Parse labal
 *
 * @param translationUnit the translation unit.
 * @return return labal
*/
LabalNode parser_parse_labal(TranslationUnit * translationUnit);

/**
 * Parse translation unit
 *
 * @param translationUnit the translation unit.
*/
void parser_parse_translation_unit(TranslationUnit * translationUnit);

#endif /*LABRATORY_C_FINAL_PROJECT_PARSER_H */
