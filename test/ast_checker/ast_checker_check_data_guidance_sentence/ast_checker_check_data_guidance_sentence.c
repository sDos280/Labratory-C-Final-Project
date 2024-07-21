#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/ast_checker.h"
#include "../../../include/preprocessor.h"
#include "../../../include/parser.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexerPreprocess;
    Lexer lexerPostprocess;
    Preprocessor preprocessor;
    TranslationUnit translationUnit;
    AstChecker astChecker;
    DataNode node;

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
    node = parser_parse_data_guidance_sentence(&translationUnit);
    error_handler_flush_error_list(&translationUnit.errorHandler);
    
    ast_checker_init(&astChecker, &translationUnit, lexerPostprocess);
    ast_checker_check_data_guidance_sentence(&astChecker, node);
    error_handler_flush_error_list(&astChecker.errorHandler);

    parser_free_data_guidance_sentence(node);

    lexer_free(&lexerPreprocess);
    lexer_free(&lexerPostprocess);
    preprocessor_free(&preprocessor);
    parser_free_translation_unit(&translationUnit);
    ast_checker_free(&astChecker);

    return 0;
}