#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init(".string.data.entry.string");
    peek_non_op_instruction();
    peek_non_op_instruction();
    peek_non_op_instruction();
    peek_non_op_instruction();
    print_token_list();

    lexer_free();
    
    return 0;
}