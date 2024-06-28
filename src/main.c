#include "stdint.h"
#include "../include/error_handler.h"
#include "../include/lexer.h"
#include "../include/string_util.h"

int main(){
    Lexer lexer;

    lexer_init_char_pointer(&lexer, "*,:##**,,-556,252:++6546");
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_number(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_number(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_number(&lexer);
    lexer_peek_number(&lexer);

    lexer_print_token_list(&lexer);
    error_handler_flush_error_list(&lexer.errorHandler);

    error_handler_free_error_list(&lexer.errorHandler);
    lexer_free(&lexer);

    return 0;
}

/*void main(){
    lexer_init("; adasdfasdf asdf \n");
    peek_comment();
    peek_next_line();
    print_token_list();
}*/

/*void main(){
    String str1 = string_init_with_data("Pleld");
    String str2 = string_init_with_data("Pleld");
    printf("%d\n", string_equals(str1, str2));
}*/