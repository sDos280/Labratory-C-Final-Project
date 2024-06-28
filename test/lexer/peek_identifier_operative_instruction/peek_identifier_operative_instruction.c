#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init_char_pointer(&lexer, "mov\ncmp\nadd\nsub\nlea\nclr\nnot\ninc\ndec\njmp\nbne\nred\nprn\njsr\nrts\nstop\n");
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