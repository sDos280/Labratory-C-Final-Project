#include "../include/preprocessor.h"


void preprocessor_init(Preprocessor * preprocessor, Lexer lexer){
    preprocessor->string = string_init();
    preprocessor->macroList = NULL;
    /* preprocessor->errorHandler = (ErrorHandler){0}; */
    preprocessor->tokens = lexer.tokens;

    error_handler_init(&preprocessor->errorHandler, lexer.string, lexer.filePath);
}

void preprocessor_free(Preprocessor * preprocessor){
    MacroList * current = preprocessor->macroList;
    string_free(preprocessor->string);

    error_handler_free_error_list(&preprocessor->errorHandler);

    /* free the macro list */
    while (preprocessor->macroList != NULL){
        /* free the macro expansion */
        string_free(current->macro.expansion);

        /* free the current macro */
        free(current);

        /* move over to the next macro */
        preprocessor->macroList = preprocessor->macroList->next;
        current = preprocessor->macroList;
    }
}

void preprocessor_generate_macro(Preprocessor * preprocessor, String source){
    Macro macro;
    TokenError error;

    int expansion_start;
    int expansion_end;
    
    int start;
    int end;

    MacroList * macroList;
    TokenList * copy;

    bool wasGoodEndMacroFound = false;
    bool wasEndMacroFound = false;

    if (preprocessor->tokens->token.kind != MACR){
        error.message = string_init_with_data("A (none operative) identifier wasn't found after an macro token");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        return; /* there is truly no need to continue to look for other macroes */
    }

    start = preprocessor->tokens->token.index;

    /* move over the MACR token */
    preprocessor->tokens = preprocessor->tokens->next;

    if (preprocessor->tokens->token.kind != IDENTIFIER){
        error.message = string_init_with_data("A (none operative) identifier wasn't found after an macro token");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        return; /* there is truly no need to continue to look for other macroes */
    }

    macro.identifier = preprocessor->tokens->token;

    macroList = preprocessor->macroList;

    /* check if the macro identifier is already used in another macro */
    while (macroList != NULL){
        if (string_equals(macro.identifier.string, macroList->macro.identifier.string)){
            error.message = string_init_with_data("That identifer was already used in another macro, please change the macro identifier name");
            error.token = macro.identifier;

            error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        }

        macroList = macroList->next;
    }

    /* move over the IDENTIFIER token */
    preprocessor->tokens = preprocessor->tokens->next;

    if (preprocessor->tokens->token.kind != EOL){
        error.message = string_init_with_data("A new line is needed after a macro identifier");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        return; /* there is truly no need to continue to look for other macroes*/
    }

    expansion_start = preprocessor->tokens->token.index + 1;

    /* move over the EOL token */
    preprocessor->tokens = preprocessor->tokens->next;

    copy = preprocessor->tokens;

    /* look for '\n' endmacr '\n' */
    while ((copy != NULL && copy->token.kind != EOFT) && 
           (copy->next != NULL && copy->next->token.kind != EOFT) &&
           (copy->next->next != NULL)){

        if (copy->token.kind == EOL && 
            copy->next->token.kind == ENDMACR &&
            (copy->next->next->token.kind == EOL || copy->next->next->token.kind == EOFT)) {

            expansion_end = copy->token.index;
            end = copy->next->next->token.index;
            wasGoodEndMacroFound = true;
            break;
        } 
        else if (copy->token.kind == ENDMACR){
            wasEndMacroFound = true;
            break;
        }

        copy = copy->next;
    }

    if (wasGoodEndMacroFound == true){
        preprocessor->tokens = copy;
    } else {
        while (preprocessor->tokens->token.kind != EOFT) {
            if (preprocessor->tokens->token.kind == ENDMACR){
                wasEndMacroFound = true;
                break;
            }

            preprocessor->tokens = preprocessor->tokens->next;
        }
    }

    if ((wasGoodEndMacroFound == false) && (wasEndMacroFound == true)){
        error.message = string_init_with_data("Although an end of macro was found, the grammer isn't correct");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
    } else if ((wasGoodEndMacroFound == false) && (wasEndMacroFound == false)){
        error.message = string_init_with_data("No end of macro was found");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
    }

    macro.expansion = string_get_slice(source, expansion_start, expansion_end);
    macro.start = start;
    macro.end = end;

    preprocessor_add_macro_to_macro_list(preprocessor, macro);
}

void preprocessor_generate_macro_list(Preprocessor * preprocessor, String source){
    while (preprocessor->tokens != NULL && preprocessor->tokens->token.kind != EOFT){
        if (preprocessor->tokens->token.kind == MACR){
            preprocessor_generate_macro(preprocessor, source);
        }

        /* advance to the next token */
        preprocessor->tokens = preprocessor->tokens->next;
    }
}

void preprocessor_add_macro_to_macro_list(Preprocessor * preprocessor, Macro macro){
    MacroList * toAdd = malloc(sizeof(MacroList));
    toAdd->macro = macro;
    toAdd->next = NULL;

    if (preprocessor->macroList == NULL){
        /* we haven't added a token yet */
        preprocessor->macroList = toAdd;
    }else{
        MacroList * last = preprocessor->macroList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void preprocessor_print_macro_list(Preprocessor * preprocessor){
    MacroList * current = preprocessor->macroList;

    while (current != NULL){
        printf("macro expansion identifier: %s\n", current->macro.identifier.string.data);
        printf("macro expansion start:\n%s\n", current->macro.expansion.data);
        printf("macro expansion end!\n");

        current = current->next;
    }
}