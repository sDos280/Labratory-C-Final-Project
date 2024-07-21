#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/parser.h"
#include "../../../include/ast_checker.h"
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
    AstChecker astChecker;
    ExternalNodeList * externalNodeList = NULL;
    EntryNodeList * entryNodeList = NULL;
    LabalNodeList * instructionLabalList = NULL;
    LabalNodeList * guidanceLabalList = NULL;

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
    parser_parse_translation_unit(&translationUnit);
    error_handler_flush_error_list(&translationUnit.errorHandler);
    
    /* ast check pass */
    ast_checker_init(&astChecker, &translationUnit, lexerPostprocess);
    ast_checker_check_duplicate_identifiers(&astChecker, &translationUnit);
    error_handler_flush_error_list(&astChecker.errorHandler);

    externalNodeList = translationUnit.externalNodeList;
    entryNodeList = translationUnit.entryNodeList;
    instructionLabalList = translationUnit.instructionLabalList;
    guidanceLabalList = translationUnit.guidanceLabalList;

    while (externalNodeList != NULL){
        printf(".extern %s\n", externalNodeList->node.token->string.data);
        externalNodeList = externalNodeList->next;
    }

    while (entryNodeList != NULL){
        printf(".entry %s\n", entryNodeList->node.token->string.data);
        entryNodeList = entryNodeList->next;
    }

    while (instructionLabalList != NULL){
        print_labal(instructionLabalList->labal);
        instructionLabalList = instructionLabalList->next;
    }
    
    while (guidanceLabalList != NULL){
        print_labal(guidanceLabalList->labal);
        guidanceLabalList = guidanceLabalList->next;
    }
    
    ast_checker_free(&astChecker);
    parser_free_translation_unit(&translationUnit);
    preprocessor_free(&preprocessor);
    lexer_free(&lexerPostprocess);
    lexer_free(&lexerPreprocess);
    
    return 0;
}