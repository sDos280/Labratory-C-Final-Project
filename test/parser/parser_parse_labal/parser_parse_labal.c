#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/parser.h"
#include "../../../include/string_util.h"

static void print_labal(LabalNode labal){
    Sentences * copy = labal.nodes;
    TokenRefrenceList * list = NULL;

    printf("labal size: %d\n", labal.size);
    printf("labal position: %d\n", labal.position);

    if (labal.labal != NULL){
        printf("%s:\n", labal.labal->string.data);
    }

    while (copy != NULL){
        if (copy->node.kind == InstructionNodeKind){
            printf("    %s", copy->node.node.instructionNode.operation->string.data);

            if (copy->node.node.instructionNode.firstOperand != NULL)
                printf(" %s", copy->node.node.instructionNode.firstOperand->string.data);
        
            if (copy->node.node.instructionNode.secondOperand != NULL)
                printf(", %s", copy->node.node.instructionNode.secondOperand->string.data);

            printf("\n");
        }
        
        if (copy->node.kind == DataNodeKind){
            printf("    .data ");
            
            list = copy->node.node.dataNode.numbers;

            while (list != NULL){
                printf("%d", atoi(list->token->string.data));
                if (list->next != NULL) printf(", ");
                list = list->next;
            }

            printf("\n");
        } 
        
        if (copy->node.kind == StringNodeKind){
            if (copy->node.node.stringNode.token != NULL)
                printf("    .string %s\n", copy->node.node.stringNode.token->string.data);
        }

        if (copy->node.kind == ExternalNodeKind){
            printf("    .extern %s\n", copy->node.node.externalNode.token->string.data);
        }

        if (copy->node.kind == EntryNodeKind){
            printf("    .entry %s\n", copy->node.node.entryNode.token->string.data);
        }

        copy = copy->next;
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
    preprocessor_init(&preprocessor, lexerPreprocess, "test1");
    preprocessor_preprocess(&preprocessor, lexerPreprocess.string);
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
    parser_free_translation_unit(&translationUnit);
    parser_free_sentences(labal.nodes);

    return 0;
}