#include "../include/AST_checker.h"
#include "../include/string_util.h"

#define MULTIPLIER (37)

unsigned long hash(String str) {
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) str.data;

    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 

    return h;
}