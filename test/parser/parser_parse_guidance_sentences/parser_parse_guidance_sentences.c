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
    GuidanceNodeList * guidanceList;
    GuidanceNodeList * copy;
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
    guidanceList = parser_parse_guidance_sentences(&translationUnit);
    copy = guidanceList;
    error_handler_flush_error_list(&translationUnit.errorHandler);

    while (guidanceList != NULL){
        if (guidanceList->kind == DataNodeKind){
            printf(".data ");
            
            list = guidanceList->node.dataNode.numbers;

            while (list != NULL){
                printf("%d, ", atoi(list->token->string.data));
                list = list->next;
            }
            printf("\n");
        } else if (guidanceList->kind == StringNodeKind){
            if (guidanceList->node.stringNode.token != NULL)
                printf(".string %s\n", guidanceList->node.stringNode.token->string.data);
        }

        guidanceList = guidanceList->next;
    }
    printf("\n");


    lexer_free(&lexerPreprocess);
    lexer_free(&lexerPostprocess);
    preprocessor_free(&preprocessor);
    parser_free_guidance_sentences(copy);
    parser_free_translation_unit(&translationUnit);

    return 0;
}