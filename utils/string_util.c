#include <string.h>
#include <stdbool.h>
#include "../include/string_util.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static unsigned power_2_ceil(unsigned x);
static unsigned power_2_floor(unsigned x);

String string_init(){
    String str;
    str.size = 8; /* just a nice number, 2^3*/
    str.index = 0; /* we setup the string data to have no data so the first chari n the buffer will be \0*/
    str.data = (char *)calloc(8, sizeof(char));

    return str;
}

void string_free(String * str){
    free(str->data);
}

String string_init_with_data(char * data){
    String str;
    str.size = MAX(power_2_ceil(strlen(data) + 1), 8); /* strlen(data) for the data itself and + 1 for \0*/
    str.index = strlen(data);
    str.data = (char *)malloc(str.size /* *sizeof(char)*/);

    int i;
    for (i = 0; i < str.index; i++){
        str.data[i] = data[i];
    }
    str.data[str.index] = '\0';

    return str;
}

void string_add_char(String *str, char ch){
    if (ch == '\0') return; /* i think i don't need to explain why we to check that */
    
    if (str->index + 1 == str->size) /* check if we got to the end (check if we currently don't have enough memory to add a char for the string)*/
    {
        str->size *= 2;
        str->data = realloc(str->data, str->size * 2); /* double the buffer size*/
        
    }

    str->data[str->index] = ch;
    str->index++;
    str->data[str->index] = '\0';
}

void string_info(String str){
    printf("String size: %d \n", str.size);
    printf("String \\0 index: %d \n", str.index);
    printf("String size: %s \n", str.data);
}

void string_add_char_pointer(String * str, char * toAdd){
    if (toAdd == NULL) return; /* i think i don't need to explain why we to check that */
    
    if (str->index + 1 + strlen(toAdd) > str->size){
        while (str->index + 1 + strlen(toAdd) > str->size) /* translate to: as long as we don't have enough memory*/
        {
            str->size *= 2;
        }

        str->data = realloc(str->data, str->size * 2); /* double the buffer size*/
    }

    int i;
    for (i = 0; i < strlen(toAdd); i++){
        str->data[str->index + i] = toAdd[i];
    }

    str->index += strlen(toAdd);
    str->data[str->index] = '\0';
}

void string_add_string(String * str, String toAdd){
    return string_add_char_pointer(str, toAdd.data);
}

char string_get_char(String str, unsigned int index){
    if (index >= str.index) return '\0';

    return str.data[index];
}

bool string_equals(String str1, String str2){
    if (str1.index != str2.index) return false;

    int i;
    for (i = 0; i < str1.index; i++){
        if (string_get_char(str1, i) != string_get_char(str2, i)) return false;
    }

    return true;
}

static unsigned power_2_floor(unsigned x) {
    int power = 1;
    while (x >>= 1) power <<= 1;
    return power;
}

static unsigned power_2_ceil(unsigned x) {
    if (x <= 1) return 1;
    int power = 2;
    x--;
    while (x >>= 1) power <<= 1;
    return power;
}