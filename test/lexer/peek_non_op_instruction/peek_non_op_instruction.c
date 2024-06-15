#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init(&lexer, ".string.data.entry.string");
    lexer_peek_non_op_instruction(&lexer);
    lexer_peek_non_op_instruction(&lexer);
    lexer_peek_non_op_instruction(&lexer);
    lexer_peek_non_op_instruction(&lexer);
    lexer_print_token_list(&lexer);

    lexer_free(&lexer);
    
    return 0;
}