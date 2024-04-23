#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("mov\ncmp\nadd\nsub\nlea\nclr\nnot\ninc\ndec\njmp\nbne\nred\nprn\njsr\nrts\nstop\n");
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    
    print_token_list();

    lexer_free();

    return 0;
}