
#ifndef LABRATORY_C_FINAL_PROJECT_NODE
#define LABRATORY_C_FINAL_PROJECT_NODE

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

typedef struct ExternalNode {
    Token * token;  /* the token (labal) that is reference as external */
}ExternalNode;

typedef struct DataNode {
    TokenRefrenceList * numbers; /* the number in the data node */
}DataNode;

typedef struct StringNode {
    Token * token; /* the that is the string */
}StringNode;

typedef struct InstructionNode {
    Token * operation; /* the operation token */
    Token * source; /* the source operand token, may be NULL for one operand instruction */
    bool is_source_derefrenced; /* true if the source is derefrenced else false */
    Token * destination; /* the destination operand token */
    bool is_destination_derefrenced; /* true if the destination is derefrenced else false */
}InstructionNode;

typedef struct NodeList {
    union {
        DataNode dataNode;
        StringNode stringNode;
        InstructionNode instructionNode;
    }node; /* the current node */

    NodeKind * kind; /* the node type */
    struct NodeList * next; /* the next node */
    
}NodeList;

typedef struct LabalNode
{
    Token * labal; /* the labal identifier */
    NodeList * nodeList; /* the list of instruction\guidance sentences */
}LabalNode;


#endif /*LABRATORY_C_FINAL_PROJECT_NODE*/


