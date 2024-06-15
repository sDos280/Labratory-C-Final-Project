#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init(&lexer, "mov\ncmp\nadd\nsub\nlea\nclr\nnot\ninc\ndec\njmp\nbne\nred\nprn\njsr\nrts\nstop\nmacr\nendmacr\nr0\nr1\nr2\nr3\nr4\nr5\nr6\nr7\nHelloThere123ForIsrael\nlove2IsRaEl\n");
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    
    lexer_print_token_list(&lexer);

    lexer_free(&lexer);

    return 0;
}