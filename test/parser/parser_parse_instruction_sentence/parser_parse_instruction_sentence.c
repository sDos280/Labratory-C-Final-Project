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
    InstructionNode instructionNode;

    /* preprocess lexer pass */
    lexer_init_file(&lexerPreprocess, "test1");
    lexer_lex(&lexerPreprocess);
    error_handler_flush_error_list(&lexerPreprocess.errorHandler);
    
    /* preprocesser pass */
    preprocessor_init(&preprocessor, lexerPreprocess);
    preprocessor_preprocess(&preprocessor, lexerPreprocess.string, "test1");
    error_handler_flush_error_list(&preprocessor.errorHandler);

    /* postprocess lexer pass */
    lexer_init_from_string(&lexerPostprocess, preprocessor.errorHandler.filePath, preprocessor.string);
    lexer_lex(&lexerPostprocess);
    error_handler_flush_error_list(&lexerPostprocess.errorHandler);
    
    /* parser pass */
    parser_init_translation_unit(&translationUnit, lexerPostprocess);
    instructionNode = parser_parse_instruction_sentence(&translationUnit);
    error_handler_flush_error_list(&translationUnit.errorHandler);
    
    if (instructionNode.firstOperand != NULL)
        printf("first operand: %s\n", instructionNode.firstOperand->string.data);
    
    if (instructionNode.secondOperand != NULL)
        printf("second operand: %s\n", instructionNode.secondOperand->string.data);

    lexer_free(&lexerPreprocess);
    lexer_free(&lexerPostprocess);
    preprocessor_free(&preprocessor);

    return 0;
}