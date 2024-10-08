#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/parser.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexerPreprocess;
    Lexer lexerPostprocess;
    Preprocessor preprocessor;
    TranslationUnit translationUnit;
    InstructionNodeList * instructionList;
    InstructionNodeList * copy;

    /* preprocess lexer pass */
    lexer_init_file(&lexerPreprocess, "test1");
    lexer_lex(&lexerPreprocess);
    error_handler_flush_error_list(&lexerPreprocess.errorHandler);
    
    /* preprocesser pass */
    preprocessor_init(&preprocessor, lexerPreprocess, "test1");
    preprocessor_preprocess(&preprocessor, lexerPreprocess.string);
    error_handler_flush_error_list(&preprocessor.errorHandler);

    /* postprocess lexer pass */
    lexer_init_from_string(&lexerPostprocess, preprocessor.errorHandler.filePath, preprocessor.string);
    lexer_lex(&lexerPostprocess);
    error_handler_flush_error_list(&lexerPostprocess.errorHandler);
    
    /* parser pass */
    parser_init_translation_unit(&translationUnit, lexerPostprocess);
    instructionList = parser_parse_instruction_sentences(&translationUnit);
    copy = instructionList;
    error_handler_flush_error_list(&translationUnit.errorHandler);
    
    while (instructionList){
        printf("operation: %s\n", instructionList->node.operation->string.data);

        if (instructionList->node.firstOperand != NULL)
            printf("first operand: %s\n", instructionList->node.firstOperand->string.data);
    
        if (instructionList->node.secondOperand != NULL)
            printf("second operand: %s\n", instructionList->node.secondOperand->string.data);

        instructionList = instructionList->next;
    }
    
    lexer_free(&lexerPreprocess);
    lexer_free(&lexerPostprocess);
    preprocessor_free(&preprocessor);
    parser_free_instruction_sentences(copy);
    parser_free_translation_unit(&translationUnit);

    return 0;
}