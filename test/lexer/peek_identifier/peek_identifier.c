#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("mov\ncmp\nadd\nsub\nlea\nclr\nnot\ninc\ndec\njmp\nbne\nred\nprn\njsr\nrts\nstop\nmacr\nendmacr\nr0\nr1\nr2\nr3\nr4\nr5\nr6\nr7\nHelloThere123ForIsrael\nlove2IsRaEl\n");
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