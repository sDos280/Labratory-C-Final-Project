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
    DataNode dataNode;
    TokenRefrenceList * list = NULL;

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
    dataNode = parser_parse_data_guidance_sentence(&translationUnit);
    list = dataNode.numbers;
    error_handler_flush_error_list(&translationUnit.errorHandler);

    while (list != NULL){
        printf("%d, ", atoi(list->token->string.data));
        list = list->next;
    }
    printf("\n");


    parser_free_data_guidance_sentence(dataNode);
    lexer_free(&lexerPreprocess);
    lexer_free(&lexerPostprocess);
    preprocessor_free(&preprocessor);
    parser_free_translation_unit(&translationUnit);

    return 0;
}