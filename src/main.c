#include "stdint.h"
#include "../include/lexer.h"
#include "../include/string_util.h"

void main(){
    lexer_init("*,:##**,,");
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    print_token_list();
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