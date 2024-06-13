#ifndef LABRATORY_C_MAMAN22_STRING_UTIL
#define LABRATORY_C_MAMAN22_STRING_UTIL

/*
 * The String util:
 * this string util provide the String struct and its helper functions for easy use of strings in c.
 * 
 * How the String struct works:
 * each string is allocated with memory and every time that we want to add stuff to it we just create a new buffer with double the size.
 * in the start we will allocate a buffer of 8 chars (inculding the \0 char) and if we see that the index that points to the \0 char in the
 * struct is in the end of the buffer we would allocate the buffer again with more memory.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct String {
    unsigned int size;  /* the size of the buffer allocated to storing the string data (including the \0 char) */
    unsigned int index; /* the index of the String that has the end of the string, the \0 char (can be use to itself as the length of the string)*/
    char * data; /* a pointer to the data */
} String;

/*
 * initialize a String with a clean buffer of size 8 bytes
 *
 * @return the initialized String.
*/
String string_init(void);

/*
 * free the passed String buffer back to the system
 *
 * @param str the String to be freed
*/
void string_free(String str);

/*
 * initialize String with the data from "data" and with a buffer's size of the uppder cilling of 2^x 
 *
 * @param data the data to be copied
 * @return the String with the data
 * 
 * @note the data char pointer needs to be null terminated
*/
String string_init_with_data(char * data);

/*
 * print string information
 *
 * @param data the data to be copied from the char pointer to the String
 * @return the String with the data
 * 
 * @note the data char pointer needs to be null terminated
*/
void string_info(String str);

/*
 * add a char to the end of the String
 *
 * @param str the String
 * @param ch the char
 * @note if the buffer is full reallocated the buffer with double the size
*/
void string_add_char(String * str, char ch);

/*
 * add a char * data to the end of the string
 *
 * @param str the String
 * @param toAdd the string to add
 * @note if the buffer is full reallocated the buffer with double the size. the toAdd char * must be null terminated
*/
void string_add_char_pointer(String * str, char * toAdd);

/*
 * add a String to the end of the str
 *
 * @param str the String
 * @param toAdd the String to add
 * @note if the buffer is full reallocated the buffer with double the size. toAdd must be allocated
*/
void string_add_string(String * str, String toAdd);

/*
 * a get function for getting a char by index from the string
 *
 * @param str the String
 * @param index the index to look in
 * @note if the index exied the length of string (over or equal to the index of \0) returns \0
*/
char string_get_char(String str, unsigned int index);

/*
 * ckeck if two strings are equals
 *
 * @param str1 the first String
 * @param str2 the second String
*/
bool string_equals(String str1, String str2);

/*
 * ckeck if a String is equals to a string for a char pointer
 *
 * @param str1 the first String
 * @param str2 the second string as char *
*/
bool string_equals_char_pointer(String str1, char * str2);

#endif /* LABRATORY_C_MAMAN22_STRING_UTIL */