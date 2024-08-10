
#ifndef LABRATORY_C_FINAL_PROJECT_NODE
#define LABRATORY_C_FINAL_PROJECT_NODE

#include "error_handler.h"
#include "token.h"

typedef enum NodeKind {
    DataNodeKind,
    StringNodeKind,
    InstructionNodeKind,
    EntryNodeKind,
    ExternalNodeKind
}NodeKind;

typedef struct EntryNode {
    Token * token;  /* the token (labal) that is reference as entry */

    bool hasParserError; /* true if got a parser error, else, false */ 
}EntryNode;

typedef struct EntryNodeList {
    EntryNode node;  /* the current node */
    struct EntryNodeList * next; /* the next node */
}EntryNodeList;

typedef struct ExternalNode {
    Token * token;  /* the token (labal) that is reference as external */

    bool hasParserError; /* true if got a parser error, else, false */ 
}ExternalNode;

typedef struct ExternalNodeList {
    ExternalNode node;  /* the current node */
    struct ExternalNodeList * next; /* the next node */
}ExternalNodeList;

typedef struct DataNode {
    TokenRefrenceList * numbers; /* the number in the data node */

    bool hasParserError; /* true if got a parser error, else, false */ 
}DataNode;

typedef struct StringNode {
    Token * token; /* the that is the string */

    bool hasParserError; /* true if got a parser error, else, false */ 
}StringNode;

typedef struct InstructionOperand {
    Token * Operand; /* the operand token */
    bool isOperandDerefrenced; /* true if the first operand is derefrenced else false */
}InstructionOperand;


typedef struct InstructionNode {
    Token * operation; /* the operation token */
    Token * firstOperand; /* the first operand token */
    bool isFirstOperandDerefrenced; /* true if the first operand is derefrenced else false */
    Token * secondOperand; /* the second operand token */
    bool isSecondOperandDerefrenced; /* true if the second operand is derefrenced else false */
    
    bool hasParserError; /* true if got a parser error, else, false */ 
}InstructionNode;

typedef struct InstructionNodeList {
    InstructionNode node; /* the current instruction node */
    struct InstructionNodeList * next; /* the next node */
}InstructionNodeList;

typedef struct GuidanceNodeList {
    union {
        DataNode dataNode;
        StringNode stringNode;
    }node; /* the current node */

    NodeKind kind; /* the node type */
    struct GuidanceNodeList * next; /* the next node */
}GuidanceNodeList;

typedef struct Sentence { /* Sentence = one node */
    union {
        DataNode dataNode;
        StringNode stringNode;
        InstructionNode instructionNode;
        EntryNode entryNode;
        ExternalNode externalNode;
    }node; /* the current node */

    NodeKind kind; /* the node kind */
    bool hasParserError; /* true if got a parser error in the coresponding node else, false */ 
    unsigned int size; /* the size of the node */
    unsigned int position; /* the position of the node in its respective image */
}Sentence;

typedef struct Sentences {
    Sentence node; /* current sentence */
    struct Sentences * next; /* the next sentence in the sequence */
}Sentences;

typedef struct LabalNode {
    Token * labal; /* the labal identifier, for guidence node list that doesn't have a labal, that would be NULL */
    Sentences * nodes; /* the list of sentences, null doesn't have one */
    unsigned int size; /* the size of the labal in the memory */
    unsigned int position; /* the position of the labal in the memory (with out the 100 offset)*/
}LabalNode;

typedef struct LabalNodeList {
    LabalNode labal; /* the current labal */
    struct LabalNodeList * next; /* the next labal */
}LabalNodeList;

typedef struct TranslationUnit {
    ExternalNodeList * externalNodeList; /* the list of all the external nodes */
    EntryNodeList * entryNodeList; /* the list of all the entry nodes */
    LabalNodeList * labals; /* the labals list */

    ErrorHandler errorHandler; /* the error handler of the translation unit */
    TokenList * tokens; /* the token list refrence from the lexer */
}TranslationUnit;


#endif /*LABRATORY_C_FINAL_PROJECT_NODE*/


