#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/parser.h"
#include "../../../include/string_util.h"

static void print_labal(LabalNode labal){
    InstructionNodeList * copyI = labal.instructionNodeList;
    GuidanceNodeList * copyG = labal.guidanceNodeList;
    TokenRefrenceList * list = NULL;

    printf("labal size: %d\n", labal.size);
    printf("labal position: %d\n", labal.position);

    if (labal.labal != NULL){
        printf("%s:\n", labal.labal->string.data);
    }

    while (copyI != NULL){
        printf("    %s", copyI->node.operation->string.data);

        if (copyI->node.firstOperand != NULL)
            printf(" %s", copyI->node.firstOperand->string.data);
    
        if (copyI->node.secondOperand != NULL)
            printf(", %s", copyI->node.secondOperand->string.data);

        printf("\n");
        copyI = copyI->next;
    }

    while (copyG != NULL){
        if (copyG->kind == DataNodeKind){
            printf("    .data ");
            
            list = copyG->node.dataNode.numbers;

            while (list != NULL){
                printf("%d", atoi(list->token->string.data));
                if (list->next != NULL) printf(", ");
                list = list->next;
            }

            printf("\n");
        } else if (copyG->kind == StringNodeKind){
            if (copyG->node.stringNode.token != NULL)
                printf("    .string %s\n", copyG->node.stringNode.token->string.data);
        }

        copyG = copyG->next;
    }
}

int main(){
    Lexer lexerPreprocess;
    Lexer lexerPostprocess;
    Preprocessor preprocessor;
    TranslationUnit translationUnit;
    LabalNode labal;

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
    labal = parser_parse_labal(&translationUnit);
    error_handler_flush_error_list(&translationUnit.errorHandler);
    
    print_labal(labal);
    
    lexer_free(&lexerPreprocess);
    lexer_free(&lexerPostprocess);
    preprocessor_free(&preprocessor);
    parser_free_instruction_sentences(labal.instructionNodeList);
    parser_free_guidance_sentences(labal.guidanceNodeList);
    parser_free_translation_unit(&translationUnit);

    return 0;
}