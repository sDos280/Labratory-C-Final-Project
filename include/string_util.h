/*
 * The String util:
 * this string util provide the String struct and its helper functions for easy use of strings in c.
 * 
 * How the String struct works:
 * each string is allocated with memory and every time that we want to add stuff to it we just create a new buffer with double the size.
 * in the start we will allocate a buffer of 8 chars (inculding the \0 char) and if we see that the index that points to the \0 char in the
 * struct is in the end of the buffer we would allocate the buffer again with more memory.
*/

#ifndef LABRATORY_C_FINAL_PROJECT_STRING_UTIL
#define LABRATORY_C_FINAL_PROJECT_STRING_UTIL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct String {
    unsigned int size;  /* the size of the buffer allocated to storing the string data (including the \0 char) */
    unsigned int index; /* the index of the string that has the end of the string, the \0 char */
    char * data; /* a pointer to the data */
} String;

String string_init(); /* init string with a buffer of nothing */
void string_free(String * str); /* free a string buffer data (after freeing the string must not be use) (there string data like the size and the freed \0 index of the freed data is saved)*/
String string_init_with_data(char * data); /* init string with the data from "data" and with a buffer's size of the uppder cilling of 2^x */
void string_info(String str); /* print string information */
void string_add_char(String * str, char ch); /* add a char to the end of the string, if the buffer is full reallocated the buffer with double the size*/
void string_add_char_pointer(String * str, char * toAdd); /* add a char * data to the end of the string, if the buffer is full reallocated the buffer with double the size*/
void string_add_string(String * str, String toAdd); /* add a String to the end of the str */
char string_get_char(String str, unsigned int index); /* a get function for getting a char by index from the string, if the index exied the length of string (over or equal to the index of \0) returns \0*/
bool string_equals(String str1, String str2); /* ckeck if two strings are equals */

#endif /* LABRATORY_C_FINAL_PROJECT_STRING_UTIL */