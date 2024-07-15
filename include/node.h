
#ifndef LABRATORY_C_FINAL_PROJECT_NODE
#define LABRATORY_C_FINAL_PROJECT_NODE

#include "error_handler.h"
#include "token.h"

typedef enum NodeKind {
    EntryNodeKind,
    ExternalNodeKind,
    DataNodeKind,
    StringNodeKind,
    InstructionNodeKind
}NodeKind;

typedef struct EntryNode {
    Token * token;  /* the token (labal) that is reference as entry */
}EntryNode;

typedef struct EntryNodeList {
    EntryNode node;  /* the current node */
    struct EntryNodeList * next; /* the next node */
}EntryNodeList;

typedef struct ExternalNode {
    Token * token;  /* the token (labal) that is reference as external */
}ExternalNode;

typedef struct ExternalNodeList {
    ExternalNode node;  /* the current node */
    struct ExternalNodeList * next; /* the next node */
}ExternalNodeList;

typedef struct DataNode {
    TokenRefrenceList * numbers; /* the number in the data node */
}DataNode;

typedef struct StringNode {
    Token * token; /* the that is the string */
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

typedef struct LabalNode {
    Token * labal; /* the labal identifier, for guidence node list that doesn't have a labal, that would be NULL */
    InstructionNodeList * instructionNodeList; /* the list of instruction sentences, null if guidance sentences labal */
    GuidanceNodeList * guidanceNodeList; /* the list of guidance sentences, null if instruction sentences labal  */
}LabalNode;

typedef struct LabalNodeList {
    LabalNode labal; /* the current labal */
    struct LabalNodeList * next; /* the next labal */
}LabalNodeList;

typedef struct TranslationUnit {
    ExternalNodeList * externalNodeList; /* the list of all the external nodes */
    EntryNodeList * entryNodeList; /* the list of all the entry nodes */
    LabalNodeList * instructionLabalList; /* the instructions labal list */
    LabalNodeList * guidanceLabalList; /* the guidance labal list */

    ErrorHandler errorHandler; /* the error handler of the translation unit */
    TokenList * tokens; /* the token list refrence from the lexer */
}TranslationUnit;


#endif /*LABRATORY_C_FINAL_PROJECT_NODE*/


