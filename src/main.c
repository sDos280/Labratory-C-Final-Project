#include "stdint.h"
#include "../include/lexer.h"
#include "../include/preprocessor.h"
#include "../include/parser.h"
#include "../include/ast_checker.h"
#include "../include/emitter.h"
#include "../include/string_util.h"

int main(int argc, char *argv[]) {
    Lexer lexerPreprocess;
    Lexer lexerPostprocess;
    Preprocessor preprocessor;
    TranslationUnit translationUnit;
    AstChecker astChecker;
    Emitter emitter;
    int i = 0;

    for (i = 1; i < argc; i++) {
        /* preprocess lexer pass */
        if (lexer_init_file(&lexerPreprocess, argv[i]) == true) {
            lexer_lex(&lexerPreprocess);
            error_handler_flush_error_list(&lexerPreprocess.errorHandler);

            /*if (lexerPreprocess.errorHandler.errorList == NULL)*/ {
                /* preprocesser pass */
                preprocessor_init(&preprocessor, lexerPreprocess, argv[i]);
                preprocessor_preprocess(&preprocessor, lexerPreprocess.string);
                error_handler_flush_error_list(&preprocessor.errorHandler);

                if (preprocessor.errorHandler.errorList == NULL) {
                    /* postprocess lexer pass */
                    lexer_init_from_string(&lexerPostprocess, preprocessor.errorHandler.filePath, preprocessor.string);
                    lexer_lex(&lexerPostprocess);
                    error_handler_flush_error_list(&lexerPostprocess.errorHandler);

                    /*if (lexerPostprocess.errorHandler.errorList == NULL)*/ {
                        /* parser pass */
                        parser_init_translation_unit(&translationUnit, lexerPostprocess);
                        parser_parse_translation_unit(&translationUnit);
                        error_handler_flush_error_list(&translationUnit.errorHandler);

                        if (translationUnit.errorHandler.errorList == NULL) {
                            /* ast check pass */
                            ast_checker_init(&astChecker, &translationUnit, lexerPostprocess);
                            ast_checker_check_translation_unit(&astChecker, &translationUnit);
                            error_handler_flush_error_list(&astChecker.errorHandler);

                            if (astChecker.errorHandler.errorList == NULL) {
                                /* emitter chekc pass*/
                                emitter_init(&emitter, lexerPostprocess);
                                emitter_update_labals_size_and_position(&emitter, &translationUnit);
                                emitter_generate_entry_file_string(&emitter, &astChecker, &translationUnit);
                                emitter_emit(&emitter, &astChecker, &translationUnit, argv[i]);
                                error_handler_flush_error_list(&emitter.errorHandler);

                                emitter_free(&emitter);
                            }

                            ast_checker_free(&astChecker);
                        }

                        parser_free_translation_unit(&translationUnit);
                    }

                    lexer_free(&lexerPostprocess);
                }

                preprocessor_free(&preprocessor);
            }
        }

        lexer_free(&lexerPreprocess);
    }

    return 0;
}