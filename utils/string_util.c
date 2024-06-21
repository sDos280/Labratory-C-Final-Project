#include <string.h>
#include <stdbool.h>
#include "../include/string_util.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static unsigned power_2_ceil(unsigned x);
/*static unsigned power_2_floor(unsigned x);*/

String string_init(){
    String str;
    str.size = 8; /* just a nice number, 2^3*/
    str.index = 0; /* we setup the string data to have no data so the first chari n the buffer will be \0*/
    str.data = (char *)calloc(8, sizeof(char));

    return str;
}

void string_free(String str){
    free(str.data);
}

String string_init_with_data(char * data){
    String str;
    int i;

    str.size = MAX(power_2_ceil(strlen(data) + 1), 8); /* strlen(data) for the data itself and + 1 for \0*/
    str.index = strlen(data);
    str.data = (char *)calloc(str.size, sizeof(char));

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
    int i;

    if (toAdd == NULL) return; /* i think i don't need to explain why we to check that */
    
    if (str->index + 1 + strlen(toAdd) > str->size){
        while (str->index + 1 + strlen(toAdd) > str->size) /* translate to: as long as we don't have enough memory*/
        {
            str->size *= 2;
        }

        str->data = realloc(str->data, str->size * 2); /* double the buffer size*/
    }

    for (i = 0; i < strlen(toAdd); i++){
        str->data[str->index + i] = toAdd[i];
    }

    str->index += strlen(toAdd);
    str->data[str->index] = '\0';
}

void string_add_string(String * str, String toAdd){
    string_add_char_pointer(str, toAdd.data);
}

char string_get_char(String str, unsigned int index){
    if (index >= str.index) return '\0';

    return str.data[index];
}

bool string_equals(String str1, String str2){
    int i;

    if (str1.index != str2.index) return false;

    for (i = 0; i < str1.index; i++){
        if (string_get_char(str1, i) != string_get_char(str2, i)) return false;
    }

    return true;
}

bool string_equals_char_pointer(String str1, char * str2){
    int i;
    /* check if the lengths aren't equal */
    if (str1.index != strlen(str2)) return false;
    
    for (i = 0; i < str1.index; i++){
        if (string_get_char(str1, i) != str2[i]) return false;
    }

    return true;
}

/*
 * (helper function) return the closest number to x in the form of 2^y (where y is a an integer and positive)
 *
 * @param x the number to be match with
 * @return closets number to x in the form of 2^y
*/
static unsigned power_2_ceil(unsigned x) {
    int power = 2;

    if (x <= 1) return 1;
    x--;
    while (x >>= 1) power <<= 1;
    return power;
}

int string_length(String str){
    return str.index; /* the string index of the \0 char is the length */
}

String string_get_slice(String str, int start, int end){
    String out = string_init();
    int i;

    for (i = start; i <= end; i++){
        string_add_char(&out, string_get_char(str, i));
    }

    return out;
}

void string_remove_slice(String * str, int start, int end){
    String strOut = string_init();

    int i;
    for (i = 0; i < string_length(*str); i++){
        if (i >= start && i <= end) continue;

        string_add_char(&strOut, string_get_char(*str, i));
    }

    /* free the string past buffer */
    free(str->data);

    /* make a shadow copy */
    str->data = strOut.data;
    str->index = strOut.index;
    str->size = strOut.size;
}

void string_replace_pattern(String * str, String pattern, String replaceWith){
    String strOut = string_init();
    int patternLength = string_length(pattern);

    int i;
    for (i = 0; i < string_length(*str); i++){
        if (strncmp(str->data + i, pattern.data, patternLength) == 0) {
            /* the pattern was found in this index */
            
            string_add_string(&strOut, replaceWith);

            i += patternLength;
        }else{
            string_add_char(&strOut, string_get_char(*str, i));
        }
    }
}