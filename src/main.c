#include "stdint.h"
#include "../include/lexer.h"
#include "../include/string_util.h"

int main(){
    lexer_init("*,:##**,,-556,252:+6546");
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_number();
    peek_separator();
    peek_number();
    peek_separator();
    peek_number();
    print_token_list();
    lexer_free();
    print_token_list();

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